/*
 * The Mailo Today Solutions Software License
 *
 * Copyright 2021 Mailo Today Solutions.
 *
 * No Permission is granted, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software.
 */
 /**
	* @file ...h
	*
	* @author Mambo Banda
	*
	*/

#pragma once


#include <amp.h>
#include <amp_math.h>

namespace xamp = concurrency;

namespace boom {

	namespace constants {
		enum
		{
			FLUID = 0, BOUNDARY = 1,
			STAM_ADVECTON = 100, MAC_CORMACK_ADVECTION = 101,
			NO_VORTICITY_CONFINEMENT = 200, WITH_VORTICITY_CONFINEMENT = 201,
		};
	}

	namespace emitters {
		struct Vec3 {
			float x, y, z;
		};
		struct FixedEmitterSettings {
			float density;
			float temperature;
			int useVelocity;
			Vec3 velocity;
		};
	}

	namespace fd {

		inline int index3d(int i, int j, int k, int GRID_SIZE_X, int GRID_SIZE_Y) restrict(amp) {

			int index = i + j * GRID_SIZE_X + GRID_SIZE_X * GRID_SIZE_Y * k;

			return index;
		}

		inline bool validIndex3d(int i, int j, int k, int size_i, int size_j, int size_k) restrict(amp) {

			int index = i + j * size_i + size_i * size_j * k;

			if (index >= (size_i * size_j * size_k) || index < 0) {
				return false;
			}

			return true;
		}

		inline float xMIN(float a, float b) restrict(amp) {
			if (a < b) return a;
			else return b;
		}

		inline float xMAX(float a, float b) restrict(amp) {
			if (a > b) return a;
			else return b;
		}

		inline void swapInt(xamp::array<int> &a, xamp::array<int> &b) {

			xamp::parallel_for_each(a.extent, [&a, &b](xamp::index<1> idx) restrict(amp) {
				int temp = a[idx];
				a[idx] = b[idx];
				b[idx] = temp;
			});
		}

		inline void swapFloat(xamp::array<float> &a, xamp::array<float> &b) {

			xamp::parallel_for_each(a.extent, [&a, &b](xamp::index<1> idx) restrict(amp) {
				float temp = a[idx];
				a[idx] = b[idx];
				b[idx] = temp;
			});
		}

		inline void fillInt(xamp::array<int> &a, int value) {

			xamp::parallel_for_each(a.extent, [&a, value](xamp::index<1> idx) restrict(amp) {
				a[idx] = value;
			});
		}

		inline void fillFloat(xamp::array<float> &a, float value) {

			xamp::parallel_for_each(a.extent, [&a, value](xamp::index<1> idx) restrict(amp) {
				a[idx] = value;
			});
		}

		inline void qtyMinMax(float i, float j, float k, xamp::array<float> &qty, float &min, float &max,
			int GRID_SIZE_X, int GRID_SIZE_Y, int GRID_SIZE_Z) restrict(amp) {

			int i0 = (int)i; int i1 = i0 + 1;
			int j0 = (int)j; int j1 = j0 + 1;
			int k0 = (int)k; int k1 = k0 + 1;

			if (!validIndex3d(i0, j0, k0, GRID_SIZE_X, GRID_SIZE_Y, GRID_SIZE_Z) ||
				!validIndex3d(i1, j0, k0, GRID_SIZE_X, GRID_SIZE_Y, GRID_SIZE_Z) ||
				!validIndex3d(i0, j1, k0, GRID_SIZE_X, GRID_SIZE_Y, GRID_SIZE_Z) ||
				!validIndex3d(i1, j1, k0, GRID_SIZE_X, GRID_SIZE_Y, GRID_SIZE_Z) ||
				!validIndex3d(i0, j0, k1, GRID_SIZE_X, GRID_SIZE_Y, GRID_SIZE_Z) ||
				!validIndex3d(i1, j0, k1, GRID_SIZE_X, GRID_SIZE_Y, GRID_SIZE_Z) ||
				!validIndex3d(i0, j1, k1, GRID_SIZE_X, GRID_SIZE_Y, GRID_SIZE_Z) ||
				!validIndex3d(i1, j1, k1, GRID_SIZE_X, GRID_SIZE_Y, GRID_SIZE_Z)) {

				min = 0.f;
				max = 0.f;
				return;
			}

			float q3 = qty[index3d(i0, j0, k0, GRID_SIZE_X, GRID_SIZE_Y)];
			float q4 = qty[index3d(i1, j0, k0, GRID_SIZE_X, GRID_SIZE_Y)];
			float q1 = qty[index3d(i0, j1, k0, GRID_SIZE_X, GRID_SIZE_Y)];
			float q2 = qty[index3d(i1, j1, k0, GRID_SIZE_X, GRID_SIZE_Y)];

			float q7 = qty[index3d(i0, j0, k1, GRID_SIZE_X, GRID_SIZE_Y)];
			float q8 = qty[index3d(i1, j0, k1, GRID_SIZE_X, GRID_SIZE_Y)];
			float q5 = qty[index3d(i0, j1, k1, GRID_SIZE_X, GRID_SIZE_Y)];
			float q6 = qty[index3d(i1, j1, k1, GRID_SIZE_X, GRID_SIZE_Y)];

			float _minA = xMIN(q1, xMIN(q2, xMIN(q3, q4)));
			float _maxA = xMAX(q1, xMAX(q2, xMAX(q3, q4)));

			float _minB = xMIN(q5, xMIN(q6, xMIN(q7, q8)));
			float _maxB = xMAX(q5, xMAX(q6, xMAX(q7, q8)));

			float _min = xMIN(_minA, _minB);
			float _max = xMAX(_maxA, _maxB);

			min = _min;
			max = _max;

		}

		inline float biInterpolate(float i, float j, float k, xamp::array<float> &qty, int GRID_SIZE_X, int GRID_SIZE_Y, int GRID_SIZE_Z) restrict(amp) {

			int i0 = (int)i; int i1 = i0 + 1;
			int j0 = (int)j; int j1 = j0 + 1;
			int k0 = (int)k; int k1 = k0 + 1;

			float x = i;
			float y = j;
			float z = k;

			if (!validIndex3d(i0, j0, k0, GRID_SIZE_X, GRID_SIZE_Y, GRID_SIZE_Z) ||
				!validIndex3d(i1, j0, k0, GRID_SIZE_X, GRID_SIZE_Y, GRID_SIZE_Z) ||
				!validIndex3d(i0, j1, k0, GRID_SIZE_X, GRID_SIZE_Y, GRID_SIZE_Z) ||
				!validIndex3d(i1, j1, k0, GRID_SIZE_X, GRID_SIZE_Y, GRID_SIZE_Z) ||
				!validIndex3d(i0, j0, k1, GRID_SIZE_X, GRID_SIZE_Y, GRID_SIZE_Z) ||
				!validIndex3d(i1, j0, k1, GRID_SIZE_X, GRID_SIZE_Y, GRID_SIZE_Z) ||
				!validIndex3d(i0, j1, k1, GRID_SIZE_X, GRID_SIZE_Y, GRID_SIZE_Z) ||
				!validIndex3d(i1, j1, k1, GRID_SIZE_X, GRID_SIZE_Y, GRID_SIZE_Z)) {
				return 0.f;
			}

			float q3 = qty[index3d(i0, j0, k0, GRID_SIZE_X, GRID_SIZE_Y)];
			float q4 = qty[index3d(i1, j0, k0, GRID_SIZE_X, GRID_SIZE_Y)];
			float q1 = qty[index3d(i0, j1, k0, GRID_SIZE_X, GRID_SIZE_Y)];
			float q2 = qty[index3d(i1, j1, k0, GRID_SIZE_X, GRID_SIZE_Y)];

			float q7 = qty[index3d(i0, j0, k1, GRID_SIZE_X, GRID_SIZE_Y)];
			float q8 = qty[index3d(i1, j0, k1, GRID_SIZE_X, GRID_SIZE_Y)];
			float q5 = qty[index3d(i0, j1, k1, GRID_SIZE_X, GRID_SIZE_Y)];
			float q6 = qty[index3d(i1, j1, k1, GRID_SIZE_X, GRID_SIZE_Y)];

			float x1 = i0; float y1 = j0; float z1 = k0;
			float x2 = i1; float y2 = j1; float z2 = k1;

			float r1 = ((x2 - x) / (x2 - x1))*q3 + ((x - x1) / (x2 - x1))*q4;
			float r2 = ((x2 - x) / (x2 - x1))*q1 + ((x - x1) / (x2 - x1))*q2;

			float f1 = ((y2 - y) / (y2 - y1))*r1 + ((y - y1) / (y2 - y1))*r2;

			float r3 = ((x2 - x) / (x2 - x1))*q7 + ((x - x1) / (x2 - x1))*q8;
			float r4 = ((x2 - x) / (x2 - x1))*q5 + ((x - x1) / (x2 - x1))*q6;

			float f2 = ((y2 - y) / (y2 - y1))*r3 + ((y - y1) / (y2 - y1))*r4;

			return ((z2 - z) / (z2 - z1))*f1 + ((z - z1) / (z2 - z1))*f2;

		}

		inline float advect(xamp::index<1> &index, int i, int j, int k, xamp::array<float> &u_vel, xamp::array<float> &v_vel, xamp::array<float> &w_vel,
			xamp::array<float> &qty, float dt, int GRID_SIZE_X, int GRID_SIZE_Y, int GRID_SIZE_Z) restrict(amp) {

			float rdx = 1.f / GRID_SIZE_X;
			float rdy = 1.f / GRID_SIZE_Y;
			float rdz = 1.f / GRID_SIZE_Z;

			float x = i - dt * rdx * u_vel[index];
			float y = j - dt * rdy * v_vel[index];
			float z = k - dt * rdz * w_vel[index];

			return biInterpolate(x, y, z, qty, GRID_SIZE_X, GRID_SIZE_Y, GRID_SIZE_Z);
		}

		inline void macCormackAdvection(xamp::array<float> &u_vel, xamp::array<float> &v_vel, xamp::array<float> &w_vel,
			xamp::array<float> &qty, xamp::array<int> &cell_type, xamp::array<float> &floatTemp00, xamp::array<float> &floatTemp01, float dt, 
			int GRID_SIZE_X, int GRID_SIZE_Y, int GRID_SIZE_Z){

			auto &_u = u_vel;
			auto &_v = v_vel;
			auto &_w = w_vel;

			auto &_qty = qty;

			auto &_floatTemp00 = floatTemp00;
			auto &_floatTemp01 = floatTemp01;

			auto &_cellType = cell_type;

			auto _DT = dt;

			int _GRID_SIZE_X = GRID_SIZE_X;
			int _GRID_SIZE_Y = GRID_SIZE_Y;
			int _GRID_SIZE_Z = GRID_SIZE_Z;

			xamp::parallel_for_each(u_vel.extent, [_GRID_SIZE_X, _GRID_SIZE_Y, _GRID_SIZE_Z, _DT, &_cellType, &_u, &_v, &_w, &_floatTemp00, &_qty](xamp::index<1> idx) restrict(amp) {

				if (_cellType[idx] != constants::BOUNDARY) {

					int a = (_GRID_SIZE_X * _GRID_SIZE_Y);
					int k = idx[0] / a;
					int b = idx[0] - a * k;
					int j = b / _GRID_SIZE_X;
					int i = b % _GRID_SIZE_X;

					_floatTemp00[idx] = advect(idx, i, j, k, _u, _v, _w, _qty, _DT, _GRID_SIZE_X, _GRID_SIZE_Y, _GRID_SIZE_Y);
				}

			});
				

			xamp::parallel_for_each(u_vel.extent, [_GRID_SIZE_X, _GRID_SIZE_Y, _GRID_SIZE_Z, _DT, &_cellType, &_u, &_v, &_w, &_floatTemp00, &_floatTemp01](xamp::index<1> idx) restrict(amp) {

				if (_cellType[idx] != constants::BOUNDARY) {

					int a = (_GRID_SIZE_X * _GRID_SIZE_Y);
					int k = idx[0] / a;
					int b = idx[0] - a * k;
					int j = b / _GRID_SIZE_X;
					int i = b % _GRID_SIZE_X;

					_floatTemp01[idx] = advect(idx, i, j, k, _u, _v, _w, _floatTemp00, -_DT, _GRID_SIZE_X, _GRID_SIZE_Y, _GRID_SIZE_Y);
				}

			});
				


			xamp::parallel_for_each(u_vel.extent, [_GRID_SIZE_X, _GRID_SIZE_Y, _GRID_SIZE_Z, _DT, &_cellType, &_u, &_v, &_w, &_floatTemp00, &_floatTemp01, &_qty](xamp::index<1> idx) restrict(amp) {

				if (_cellType[idx] != constants::BOUNDARY) {

					int a = (_GRID_SIZE_X * _GRID_SIZE_Y);
					int k = idx[0] / a;
					int b = idx[0] - a * k;
					int j = b / _GRID_SIZE_X;
					int i = b % _GRID_SIZE_X;


					float rdx = 1.f / _GRID_SIZE_X;
					float rdy = 1.f / _GRID_SIZE_Y;
					float rdz = 1.f / _GRID_SIZE_Z;


					float x = i - _DT * rdx * _u[index3d(i, j, k, _GRID_SIZE_X, _GRID_SIZE_Y)];
					float y = j - _DT * rdy * _v[index3d(i, j, k, _GRID_SIZE_X, _GRID_SIZE_Y)];
					float z = k - _DT * rdz * _w[index3d(i, j, k, _GRID_SIZE_X, _GRID_SIZE_Y)];

					float min = 0.f;
					float max = 0.f;
					qtyMinMax(x, y, z, _qty, min, max, _GRID_SIZE_X, _GRID_SIZE_Y, _GRID_SIZE_Z);
					_floatTemp00[idx] = _floatTemp00[idx] + 0.5f * (_qty[idx] - _floatTemp01[idx]);
					_floatTemp00[idx] = xMAX(xMIN(_floatTemp00[idx], max), min);
				}
			});
				
			swapFloat(qty, floatTemp00);
		}

		inline float jacobi(xamp::index<1> &index, int i, int j, int k, xamp::array<float> &x, xamp::array<float> &b, float alpha, float rbeta, int GRID_SIZE_X, int GRID_SIZE_Y) restrict(amp) {

			// left, right, bottom, and top x samples
			float xL = x[index3d(i - 1, j, k, GRID_SIZE_X, GRID_SIZE_Y)];
			float xR = x[index3d(i + 1, j, k, GRID_SIZE_X, GRID_SIZE_Y)];
			float xB = x[index3d(i, j - 1, k, GRID_SIZE_X, GRID_SIZE_Y)];
			float xT = x[index3d(i, j + 1, k, GRID_SIZE_X, GRID_SIZE_Y)];
			float vBk = x[index3d(i, j, k - 1, GRID_SIZE_X, GRID_SIZE_Y)];
			float vFt = x[index3d(i, j, k + 1, GRID_SIZE_X, GRID_SIZE_Y)];

			// b sample, from center
			float bC = b[index];

			// evaluate Jacobi iteration
			return (xL + xR + xB + xT + vBk + vFt + alpha * bC) * rbeta;
		}

		inline float divergence(int i, int j, int k, xamp::array<float> &u_vel, xamp::array<float> &v_vel, xamp::array<float> &w_vel, 
			float halfrdx, int GRID_SIZE_X, int GRID_SIZE_Y) restrict(amp) {

			float uL = u_vel[index3d(i - 1, j, k, GRID_SIZE_X, GRID_SIZE_Y)];
			float uR = u_vel[index3d(i + 1, j, k, GRID_SIZE_X, GRID_SIZE_Y)];

			float vB = v_vel[index3d(i, j - 1, k, GRID_SIZE_X, GRID_SIZE_Y)];
			float vT = v_vel[index3d(i, j + 1, k, GRID_SIZE_X, GRID_SIZE_Y)];

			float vBk = w_vel[index3d(i, j, k - 1, GRID_SIZE_X, GRID_SIZE_Y)];
			float vFt = w_vel[index3d(i, j, k + 1, GRID_SIZE_X, GRID_SIZE_Y)];

			return  halfrdx * ((uR - uL) + (vT - vB) + (vFt - vBk));
		}

		inline void gradient(int i, int j, int k, float halfrdx, float &u_out, float &v_out, float &w_out, xamp::array<float> &p, int GRID_SIZE_X, int GRID_SIZE_Y) restrict(amp) {

			float pL = p[index3d(i - 1, j, k,  GRID_SIZE_X, GRID_SIZE_Y)];
			float pR = p[index3d(i + 1, j, k, GRID_SIZE_X, GRID_SIZE_Y)];
			float pB = p[index3d(i, j - 1, k, GRID_SIZE_X, GRID_SIZE_Y)];
			float pT = p[index3d(i, j + 1, k, GRID_SIZE_X, GRID_SIZE_Y)];
			float pBk = p[index3d(i, j, k - 1, GRID_SIZE_X, GRID_SIZE_Y)];
			float pFt = p[index3d(i, j, k + 1, GRID_SIZE_X, GRID_SIZE_Y)];

			u_out -= halfrdx * (pR - pL);
			v_out -= halfrdx * (pT - pB);
			w_out -= halfrdx * (pFt - pBk);
		}

		inline void curl(int i, int j, int k, xamp::array<float> &u_vel, xamp::array<float> &v_vel, xamp::array<float> &w_vel, float &wx, float &wy, float &wz,
			int GRID_SIZE_X, int GRID_SIZE_Y, int GRID_SIZE_Z)restrict(amp) {
			
			float left_x = 0;
			float left_y = 0;
			float left_z = 0;

			float right_x = 0;
			float right_y = 0;
			float right_z = 0;

			float down_x = 0;
			float down_y = 0;
			float down_z = 0;

			float up_x = 0;
			float up_y = 0;
			float up_z = 0;

			float back_x = 0;
			float back_y = 0;
			float back_z = 0;

			float front_x = 0;
			float front_y = 0;
			float front_z = 0;

			if (validIndex3d(i - 1, j, k, GRID_SIZE_X, GRID_SIZE_Y, GRID_SIZE_Z)) {
				int idx = index3d(i - 1, j, k, GRID_SIZE_X, GRID_SIZE_Y);
				left_x = u_vel[idx];
				left_y = v_vel[idx];
				left_z = w_vel[idx];
			}

			if (validIndex3d(i + 1, j, k, GRID_SIZE_X, GRID_SIZE_Y, GRID_SIZE_Z)) {
				int idx = index3d(i + 1, j, k, GRID_SIZE_X, GRID_SIZE_Y);
				right_x = u_vel[idx];
				right_y = v_vel[idx];
				right_z = w_vel[idx];
			}

			if (validIndex3d(i, j - 1, k, GRID_SIZE_X, GRID_SIZE_Y, GRID_SIZE_Z)) {
				int idx = index3d(i, j - 1, k, GRID_SIZE_X, GRID_SIZE_Y);
				down_x = u_vel[idx];
				down_y = v_vel[idx];
				down_z = w_vel[idx];
			}

			if (validIndex3d(i, j + 1, k, GRID_SIZE_X, GRID_SIZE_Y, GRID_SIZE_Z)) {
				int idx = index3d(i, j + 1, k, GRID_SIZE_X, GRID_SIZE_Y);
				up_x = u_vel[idx];
				up_y = v_vel[idx];
				up_z = w_vel[idx];
			}

			if (validIndex3d(i, j, k - 1, GRID_SIZE_X, GRID_SIZE_Y, GRID_SIZE_Z)) {
				int idx = index3d(i, j, k - 1, GRID_SIZE_X, GRID_SIZE_Y);
				back_x = u_vel[idx];
				back_y = v_vel[idx];
				back_z = w_vel[idx];
			}

			if (validIndex3d(i, j, k + 1, GRID_SIZE_X, GRID_SIZE_Y, GRID_SIZE_Z)) {
				int idx = index3d(i, j, k + 1, GRID_SIZE_X, GRID_SIZE_Y);
				front_x = u_vel[idx];
				front_y = v_vel[idx];
				front_z = w_vel[idx];
			}


			// curlx = dw/dy - dv/dz
			wx = (up_z - down_z) * 0.5f -
				(front_y - back_y) * 0.5f;

			// curly = du/dz - dw/dx
			wy = (front_x - back_x) * 0.5f -
				(right_z - left_z) * 0.5f;

			// curlz = dv/dx - du/dy
			wz = (right_y - left_y) * 0.5f -
				(up_x - down_x) * 0.5f;

		}

		inline void vorticityConfinement(float vorticity, float dt, xamp::array<float> &u_vel, xamp::array<float> &u_vel_prev, xamp::array<float> &v_vel, xamp::array<float> &v_vel_prev,
			xamp::array<float> &w_vel, xamp::array<float> &w_vel_prev , xamp::array<int> &cell_type, int GRID_SIZE_X, int GRID_SIZE_Y, int GRID_SIZE_Z)  {

			auto &_u = u_vel;
			auto &_v = v_vel;
			auto &_w = w_vel;

			auto _vorticity = vorticity;
			auto &_u_vel_prev = u_vel_prev;
			auto &_v_vel_prev = v_vel_prev;
			auto &_w_vel_prev = w_vel_prev;

			auto &_cellType = cell_type;

			auto _DT = dt;

			int _GRID_SIZE_X = GRID_SIZE_X;
			int _GRID_SIZE_Y = GRID_SIZE_Y;
			int _GRID_SIZE_Z = GRID_SIZE_Z;

			xamp::parallel_for_each(u_vel.extent, [_vorticity, _GRID_SIZE_X, _GRID_SIZE_Y, _GRID_SIZE_Z, _DT, &_cellType, &_u, &_v, &_w, &_u_vel_prev, &_v_vel_prev, &_w_vel_prev](xamp::index<1> idx) restrict(amp) {

				if (_cellType[idx] != constants::BOUNDARY) {

					int a = (_GRID_SIZE_X * _GRID_SIZE_Y);
					int k = idx[0] / a;
					int b = idx[0] - a * k;
					int j = b / _GRID_SIZE_X;
					int i = b % _GRID_SIZE_X;

					float btCurl_x = 0;
					float btCurl_y = 0;
					float btCurl_z = 0;
					
					float tpCurl_x = 0;
					float tpCurl_y = 0;
					float tpCurl_z = 0;

					float rtCurl_x = 0;
					float rtCurl_y = 0;
					float rtCurl_z = 0;

					float lfCurl_x = 0;
					float lfCurl_y = 0;
					float lfCurl_z = 0;

					float bkCurl_x = 0;
					float bkCurl_y = 0;
					float bkCurl_z = 0;

					float ftCurl_x = 0;
					float ftCurl_y = 0;
					float ftCurl_z = 0;

					float ctCurl_x = 0;
					float ctCurl_y = 0;
					float ctCurl_z = 0;

					curl(i, j, k, _u, _v, _w, ctCurl_x, ctCurl_y, ctCurl_z, _GRID_SIZE_X, _GRID_SIZE_Y, _GRID_SIZE_Z);

					curl(i, j - 1, k, _u, _v, _w, btCurl_x, btCurl_y, btCurl_z, _GRID_SIZE_X, _GRID_SIZE_Y, _GRID_SIZE_Z);
					curl(i, j + 1, k, _u, _v, _w, tpCurl_x, tpCurl_y, tpCurl_z, _GRID_SIZE_X, _GRID_SIZE_Y, _GRID_SIZE_Z);
					curl(i + 1, j, k, _u, _v, _w, rtCurl_x, rtCurl_y, rtCurl_z, _GRID_SIZE_X, _GRID_SIZE_Y, _GRID_SIZE_Z);
					curl(i - 1, j, k, _u, _v, _w, lfCurl_x, lfCurl_y, lfCurl_z, _GRID_SIZE_X, _GRID_SIZE_Y, _GRID_SIZE_Z);
					curl(i, j, k - 1, _u, _v, _w, bkCurl_x, bkCurl_y, bkCurl_z, _GRID_SIZE_X, _GRID_SIZE_Y, _GRID_SIZE_Z);
					curl(i, j, k + 1, _u, _v, _w, ftCurl_x, ftCurl_y, ftCurl_z, _GRID_SIZE_X, _GRID_SIZE_Y, _GRID_SIZE_Z);

					float btCurlMag = xamp::fast_math::sqrtf(btCurl_x*btCurl_x + btCurl_y * btCurl_y + btCurl_z * btCurl_z);
					float tpCurlMag = xamp::fast_math::sqrtf(tpCurl_x*tpCurl_x + tpCurl_y * tpCurl_y + tpCurl_z * tpCurl_z);
					float rtCurlMag = xamp::fast_math::sqrtf(rtCurl_x*rtCurl_x + rtCurl_y * rtCurl_y + rtCurl_z * rtCurl_z);
					float lfCurlMag = xamp::fast_math::sqrtf(lfCurl_x*lfCurl_x + lfCurl_y * lfCurl_y + lfCurl_z * lfCurl_z);
					float bkCurlMag = xamp::fast_math::sqrtf(bkCurl_x*bkCurl_x + bkCurl_y * bkCurl_y + bkCurl_z * bkCurl_z);
					float ftCurlMag = xamp::fast_math::sqrtf(ftCurl_x*ftCurl_x + ftCurl_y * ftCurl_y + ftCurl_z * ftCurl_z);

					float Nx = (rtCurlMag - lfCurlMag) * 0.5f;
					float Ny = (tpCurlMag - btCurlMag) * 0.5f;
					float Nz = (ftCurlMag - bkCurlMag) * 0.5f;
					float len1 = 1.0f / (xamp::fast_math::sqrtf(Nx*Nx + Ny * Ny + Nz * Nz) + 0.0000001f);
					Nx *= len1;
					Ny *= len1;
					Nz *= len1;
					_u_vel_prev[idx] = _u[idx] + _vorticity * (Ny*ctCurl_z - Nz * ctCurl_y) * _DT;
					_v_vel_prev[idx] = _v[idx] + _vorticity * (Nz*ctCurl_x - Nx * ctCurl_z) * _DT;
					_w_vel_prev[idx] = _w[idx] + _vorticity * (Nx*ctCurl_y - Ny * ctCurl_x) * _DT;

				}

			});
				

			swapFloat(u_vel, u_vel_prev);
			swapFloat(v_vel, v_vel_prev);
			swapFloat(w_vel, w_vel_prev);
		}

		inline void dissipation(float dissipationRate, xamp::array<float> &u_vel, xamp::array<float> &v_vel, xamp::array<float> &w_vel , xamp::array<float> &qty, xamp::array<int> &cell_type, float dt) {
	
			auto &_u = u_vel;
			auto &_v = v_vel;
			auto &_w = w_vel;

			auto &_qty = qty;
			auto &_cellType = cell_type;

			auto _DT = dt;
			auto _dissipationRate = dissipationRate;

			xamp::parallel_for_each(u_vel.extent, [_DT, _dissipationRate, &_u, &_v, &_w, &_qty, &_cellType](xamp::index<1> idx) restrict(amp) {

				if (_cellType[idx] != constants::BOUNDARY) {

					float speed = xamp::fast_math::sqrtf(_u[idx]*_u[idx] + _v[idx]*_v[idx] + _w[idx]*_w[idx]);
					if (_qty[idx] > 0.f  && speed > 0.f)
						_qty[idx] = xMAX(0.f, _qty[idx] - _dissipationRate * _DT);
				}
			});
			
		}
	}

}