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
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/


#include "Entropy.h"
#include "EntropyCore.h"

namespace boom {

	Entropy::Entropy(xamp::accelerator accelerator, int dimx, int dimy, int dimz) :
		extent(dimx * dimy * dimz),
		u(dimx * dimy * dimz, accelerator.default_view),
		v(dimx * dimy * dimz, accelerator.default_view),
		w(dimx * dimy * dimz, accelerator.default_view),
		p(dimx * dimy * dimz, accelerator.default_view),
		dens(dimx * dimy * dimz, accelerator.default_view),
		temperature(dimx * dimy * dimz, accelerator.default_view),
		floatTemp00(dimx * dimy * dimz, accelerator.default_view),
		floatTemp01(dimx * dimy * dimz, accelerator.default_view),
		floatTemp02(dimx * dimy * dimz, accelerator.default_view),
		udiv(dimx * dimy * dimz, accelerator.default_view),
		cellType(dimx * dimy * dimz, accelerator.default_view),
		fixedEmitters(dimx * dimy * dimz, accelerator.default_view) {

		this->accelerator = accelerator;

		advectionMethod = constants::MAC_CORMACK_ADVECTION;
		vorticityFlag = constants::WITH_VORTICITY_CONFINEMENT;

		GRID_SIZE_X = dimx;
		GRID_SIZE_Y = dimy;
		GRID_SIZE_Z = dimz;

		RDX = 1.0f;
		VISC = 0.05;
		VORT = 2.5;
		DENS_DISSIP = 0.06;
		BUOY_RISE = 0.5;
		BUOY_FALL = 2.0;
		AMB_TEMP = 25;

		DT = 0.133f;
		DIFF_ITER = 15;
		PROJ_ITER = 30;

		gravity_x = 0.f;
		gravity_y = -9.8f;
		gravity_z = 0.f;

		DIFFUSION_ALPHA = RDX / (VISC * DT);
		DIFFUSION_BETA_RECIPROCAL = RDX / (6.0f + DIFFUSION_ALPHA);
		PRESSURE_ALPHA = -RDX * RDX;
		PRESSURE_BETA_RECIPROCAL = 1.0f / 6.0f;
		HALF_RDX = 0.5f / RDX;

	}

	int Entropy::getGridSizeX() {
		return GRID_SIZE_X;
	}

	int Entropy::getGridSizeY() {
		return GRID_SIZE_Y;
	}

	int Entropy::getGridSizeZ() {
		return GRID_SIZE_X;
	}

	xamp::array<float> &Entropy::getU() {
		return u;
	}

	xamp::array<float> &Entropy::getV() {
		return v;
	}

	xamp::array<float> &Entropy::getW() {
		return w;
	}

	xamp::array<float> &Entropy::getP() {
		return p;
	}

	xamp::array<float> &Entropy::getDens() {
		return dens;
	}

	xamp::array<float> &Entropy::getTemperature() {
		return temperature;
	}

	xamp::array<float> &Entropy::getTemp00() {
		return floatTemp00;
	}

	xamp::array<float> &Entropy::getTemp01() {
		return floatTemp01;
	}

	xamp::array<float> &Entropy::getTemp02() {
		return floatTemp02;
	}

	xamp::array<float> &Entropy::getUDiv() {
		return udiv;
	}

	xamp::array<int> &Entropy::getCellType() {
		return cellType;
	}

	xamp::array<int> &Entropy::getFixedEmitters() {
		return fixedEmitters;
	}

	float Entropy::getRDX() {
		return RDX;
	}

	void Entropy::setRDX(float rdx) {
		RDX = rdx;
		syncSettings();
	}

	float Entropy::getViscosity() {
		return VISC;
	}

	void Entropy::setViscosity(float visc) {
		VISC = visc;
		syncSettings();
	}

	float Entropy::getTimeStep() {
		return DT;
	}

	void Entropy::setTimeStep(float dt) {
		DT = dt;
		syncSettings();
	}

	int Entropy::getDiffuseIterations() {
		return DIFF_ITER;
	}

	void Entropy::setDiffuseIterations(int iter) {
		DIFF_ITER = iter;
		syncSettings();
	}

	int Entropy::getProjectionIterations() {
		return PROJ_ITER;
	}

	void Entropy::setProjectionIterations(int iter) {
		PROJ_ITER = iter;
		syncSettings();
	}

	float Entropy::getDiffusionAlpha() {
		return DIFFUSION_ALPHA;
	}

	void Entropy::setDiffusionAlpha(float alpha) {
		DIFFUSION_ALPHA = alpha;
		syncSettings();
	}

	float Entropy::getDiffusionBeta() {
		return DIFFUSION_BETA_RECIPROCAL;
	}

	void Entropy::setDiffusionBeta(float diffBeta) {
		DIFFUSION_BETA_RECIPROCAL = diffBeta;
		syncSettings();
	}

	float Entropy::getPressureAlpha() {
		return PRESSURE_ALPHA;
	}

	void Entropy::setPressureAlpha(float val) {
		PRESSURE_ALPHA = val;
		syncSettings();
	}

	float Entropy::getPressureBeta() {
		return PRESSURE_BETA_RECIPROCAL;
	}

	void Entropy::setPressureBeta(float val) {
		PRESSURE_BETA_RECIPROCAL = val;
		syncSettings();
	}

	float Entropy::getHalfRDX() {
		return HALF_RDX;
	}

	void Entropy::setHalfRDX(float rdx) {
		HALF_RDX = rdx;
		syncSettings();
	}

	float Entropy::getVorticity() {
		return VORT;
	}

	void Entropy::setVorticity(float val) {
		VORT = val;
	}

	float Entropy::getDensityDissipation() {
		return DENS_DISSIP;
	}

	void Entropy::setDensityDissipation(float val) {
		DENS_DISSIP = val;
	}

	float Entropy::getBuoyancyRise() {
		return BUOY_RISE;
	}

	void Entropy::setBuoyancyRise(float val) {
		BUOY_RISE = val;
	}

	float Entropy::getBuoyancyFall() {
		return BUOY_FALL;
	}

	void Entropy::setBuoyancyFall(float val) {
		BUOY_FALL = val;
	}

	float Entropy::getAmbiantTemperature() {
		return AMB_TEMP;
	}

	void Entropy::setAmbiantTemperature(float val) {
		AMB_TEMP = val;
	}	

	int Entropy::getAdvectionType() {
		return advectionMethod;
	}

	void Entropy::setAdvectionType(int val) {
		advectionMethod = val;
	}

	int Entropy::getVorticityConfinementFlag() {
		return vorticityFlag;
	}

	void Entropy::setVorticityConfinementFlag(int val) {
		vorticityFlag = val;
	}

	void Entropy::syncSettings() {

		DIFFUSION_ALPHA = RDX / (VISC * DT);
		DIFFUSION_BETA_RECIPROCAL = RDX / (6.0f + DIFFUSION_ALPHA);
		PRESSURE_ALPHA = -RDX * RDX;
		PRESSURE_BETA_RECIPROCAL = 1.0f / 6.0f;
		HALF_RDX = 0.5f / RDX;

	}

	std::vector<float> Entropy::getGravity() {
		return std::vector<float> { gravity_x, gravity_y, gravity_z };
	}

	void Entropy::setGravity(float x, float y, float z) {
		gravity_x = x;
		gravity_y = y;
		gravity_z = z;
	}

	// Step
	void Entropy::step() {

		Timer st;
		st.startTimer();

		emitFixedEmitters();

		enforceBoundary();

		if(advectionMethod == constants::STAM_ADVECTON) advectVelocity();
		else if (advectionMethod == constants::MAC_CORMACK_ADVECTION) macCormackAdvectVelocity();

		buoyancy();
		if(vorticityFlag == constants::WITH_VORTICITY_CONFINEMENT) vorticity();

		enforceBoundary();

		diffuseVelocity();
		diffuseDensity();

		if (advectionMethod == constants::STAM_ADVECTON) {
			advectDensity();
			advectTemperature();
		}
		else if (advectionMethod == constants::MAC_CORMACK_ADVECTION) {
			macCormackAdvectDensity();
			macCormackAdvectTemperature();
		}

		dissipation();
		
		enforceBoundary();
		
		fd::fillFloat(getUDiv(), 0.f);
		fd::fillFloat(getP(), 0.f);
		
		divergence();

		pressure();

		subtractPressureVelocity();

		accelerator.get_default_view().wait();

		st.stopTimer();
		std::cout << std::fixed << "Step time: " << st.getTime() << "ms\n";
	}

	// AMP Kernels
	void Entropy::advectVelocity() {

		auto &_u = getU();
		auto &_v = getV();
		auto &_w = getW();

		auto &_u_prev = getTemp00();
		auto &_v_prev = getTemp01();
		auto &_w_prev = getTemp02();

		auto &_cellType = getCellType();

		auto _DT = DT;

		auto _GRID_SIZE_X = GRID_SIZE_X;
		auto _GRID_SIZE_Y = GRID_SIZE_Y;
		auto _GRID_SIZE_Z = GRID_SIZE_Z;

		xamp::parallel_for_each(extent, [_GRID_SIZE_X, _GRID_SIZE_Y, _GRID_SIZE_Z, _DT, &_cellType, &_u, &_v, &_w, &_u_prev, &_v_prev, &_w_prev](xamp::index<1> idx) restrict(amp) {
			if (_cellType[idx] != boom::constants::BOUNDARY) {

				int a = (_GRID_SIZE_X * _GRID_SIZE_Y);
				int k = idx[0] / a;
				int b = idx[0] - a * k;
				int j = b / _GRID_SIZE_X;
				int i = b % _GRID_SIZE_X;

				_u_prev[idx] = fd::advect(idx, i, j, k, _u, _v, _w, _u, _DT, _GRID_SIZE_X, _GRID_SIZE_Y, _GRID_SIZE_Z);
				_v_prev[idx] = fd::advect(idx, i, j, k, _u, _v, _w, _v, _DT, _GRID_SIZE_X, _GRID_SIZE_Y, _GRID_SIZE_Z);
				_w_prev[idx] = fd::advect(idx, i, j, k, _u, _v, _w, _w, _DT, _GRID_SIZE_X, _GRID_SIZE_Y, _GRID_SIZE_Z);
			}
		});

		fd::swapFloat(_u, _u_prev);
		fd::swapFloat(_v, _v_prev);
		fd::swapFloat(_w, _w_prev);
	}

	void Entropy::macCormackAdvectVelocity() {
		fd::macCormackAdvection(getU(), getV(), getW(), getU(), getCellType(), getTemp00(), getTemp01(), DT, GRID_SIZE_X, GRID_SIZE_Y, GRID_SIZE_Z);
		fd::macCormackAdvection(getU(), getV(), getW(), getV(), getCellType(), getTemp00(), getTemp01(), DT, GRID_SIZE_X, GRID_SIZE_Y, GRID_SIZE_Z);
		fd::macCormackAdvection(getU(), getV(), getW(), getW(), getCellType(), getTemp00(), getTemp01(), DT, GRID_SIZE_X, GRID_SIZE_Y, GRID_SIZE_Z);
	}

	void Entropy::advectDensity() {

		auto _DT = DT;

		auto &_u = getU();
		auto &_v = getV();
		auto &_w = getW();

		auto &_dens = getDens();
		auto &_densPrev = getTemp00();

		auto &_cellType = getCellType();

		auto _GRID_SIZE_X = GRID_SIZE_X;
		auto _GRID_SIZE_Y = GRID_SIZE_Y;
		auto _GRID_SIZE_Z = GRID_SIZE_Z;

		xamp::parallel_for_each(extent, [_GRID_SIZE_X, _GRID_SIZE_Y, _GRID_SIZE_Z, _DT, &_cellType, &_u, &_v, &_w, &_dens, &_densPrev](xamp::index<1> idx) restrict(amp) {

			if (_cellType[idx] != boom::constants::BOUNDARY) {

				int a = (_GRID_SIZE_X * _GRID_SIZE_Y);
				int k = idx[0] / a;
				int b = idx[0] - a * k;
				int j = b / _GRID_SIZE_X;
				int i = b % _GRID_SIZE_X;

				_densPrev[idx] = fd::advect(idx, i, j, k, _u, _v, _w, _dens, _DT, _GRID_SIZE_X, _GRID_SIZE_Y, _GRID_SIZE_Y);
			}

		});
		
		fd::swapFloat(_dens, _densPrev);
	}

	void Entropy::macCormackAdvectDensity() {
		fd::macCormackAdvection(getU(), getV(), getW(), getDens(), getCellType(), getTemp00(), getTemp01(), DT, GRID_SIZE_X, GRID_SIZE_Y, GRID_SIZE_Z);
	}

	void Entropy::advectTemperature() {
		auto _DT = DT;

		auto &_u = getU();
		auto &_v = getV();
		auto &_w = getW();

		auto &_temp = getTemperature();
		auto &_tempPrev = getTemp00();

		auto &_cellType = getCellType();

		auto _GRID_SIZE_X = GRID_SIZE_X;
		auto _GRID_SIZE_Y = GRID_SIZE_Y;
		auto _GRID_SIZE_Z = GRID_SIZE_Z;

		xamp::parallel_for_each(extent, [_GRID_SIZE_X, _GRID_SIZE_Y, _GRID_SIZE_Z, _DT, &_cellType, &_u, &_v, &_w, &_temp, &_tempPrev](xamp::index<1> idx) restrict(amp) {

			if (_cellType[idx] != boom::constants::BOUNDARY) {

				int a = (_GRID_SIZE_X * _GRID_SIZE_Y);
				int k = idx[0] / a;
				int b = idx[0] - a * k;
				int j = b / _GRID_SIZE_X;
				int i = b % _GRID_SIZE_X;

				_tempPrev[idx] = fd::advect(idx, i, j, k, _u, _v, _w, _temp, _DT, _GRID_SIZE_X, _GRID_SIZE_Y, _GRID_SIZE_Y);
			}

		});

		fd::swapFloat(_temp, _tempPrev);
	}

	void Entropy::macCormackAdvectTemperature() {
		fd::macCormackAdvection(getU(), getV(), getW(), getTemperature(), getCellType(), getTemp00(), getTemp01(), DT, GRID_SIZE_X, GRID_SIZE_Y, GRID_SIZE_Z);
	}

	void Entropy::divergence() {

		auto &_u = getU();
		auto &_v = getV();
		auto &_w = getW();

		auto &_cellType = getCellType();

		auto &_udiv = getUDiv();

		auto _DT = DT;

		auto &_HALF_RDX = HALF_RDX;

		auto _GRID_SIZE_X = GRID_SIZE_X;
		auto _GRID_SIZE_Y = GRID_SIZE_Y;
		auto _GRID_SIZE_Z = GRID_SIZE_Z;

		xamp::parallel_for_each(extent, [_GRID_SIZE_X, _GRID_SIZE_Y, _GRID_SIZE_Z, _DT, &_cellType, &_u, &_v, &_w, &_udiv, _HALF_RDX](xamp::index<1> idx) restrict(amp) {

			if (_cellType[idx] != boom::constants::BOUNDARY) {

				int a = (_GRID_SIZE_X * _GRID_SIZE_Y);
				int k = idx[0] / a;
				int b = idx[0] - a * k;
				int j = b / _GRID_SIZE_X;
				int i = b % _GRID_SIZE_X;

				_udiv[idx] = fd::divergence(i, j, k, _u, _v, _w, _HALF_RDX, _GRID_SIZE_X, _GRID_SIZE_Y);
			}
		});
	}

	void Entropy::pressure() {

		auto &_cellType = getCellType();

		auto &_p = getP();
		auto &_pPrev = getTemp00();

		auto _DT = DT;

		auto &_udiv = getUDiv();

		auto &_HALF_RDX = HALF_RDX;

		auto &_PRESSURE_ALPHA = PRESSURE_ALPHA; 
		auto &_PRESSURE_BETA_RECIPROCAL = PRESSURE_BETA_RECIPROCAL;

		auto _GRID_SIZE_X = GRID_SIZE_X;
		auto _GRID_SIZE_Y = GRID_SIZE_Y;
		auto _GRID_SIZE_Z = GRID_SIZE_Z;

		for (long int iter = 0; iter < PROJ_ITER; iter++) {

			xamp::parallel_for_each(extent, [_GRID_SIZE_X, _GRID_SIZE_Y, _DT, &_cellType, &_p, &_pPrev, &_udiv, 
				_HALF_RDX, _PRESSURE_ALPHA, _PRESSURE_BETA_RECIPROCAL](xamp::index<1> idx) restrict(amp) {

				if (_cellType[idx] != boom::constants::BOUNDARY) {

					int a = (_GRID_SIZE_X * _GRID_SIZE_Y);
					int k = idx[0] / a;
					int b = idx[0] - a * k;
					int j = b / _GRID_SIZE_X;
					int i = b % _GRID_SIZE_X;

					_pPrev[idx] = fd::jacobi(idx, i, j, k, _p, _udiv, _PRESSURE_ALPHA, _PRESSURE_BETA_RECIPROCAL, _GRID_SIZE_X, _GRID_SIZE_Y);
				}

			});
			
			fd::swapFloat(_p, _pPrev);

			enforceBoundary();
		}

	}

	void Entropy::subtractPressureVelocity() {

		auto &_u = getU();
		auto &_v = getV();
		auto &_w = getW();

		auto &_cellType = getCellType();

		auto &_p = getP();

		auto _DT = DT;

		auto &_HALF_RDX = HALF_RDX;

		auto &_PRESSURE_ALPHA = PRESSURE_ALPHA;
		auto &_PRESSURE_BETA_RECIPROCAL = PRESSURE_BETA_RECIPROCAL;

		auto _GRID_SIZE_X = GRID_SIZE_X;
		auto _GRID_SIZE_Y = GRID_SIZE_Y;

		xamp::parallel_for_each(extent, [_GRID_SIZE_X, _GRID_SIZE_Y, _DT, &_cellType, &_u, &_v, &_w, &_p, _HALF_RDX](xamp::index<1> idx) restrict(amp) {

			if (_cellType[idx] != boom::constants::BOUNDARY) {

				int a = (_GRID_SIZE_X * _GRID_SIZE_Y);
				int k = idx[0] / a;
				int b = idx[0] - a * k;
				int j = b / _GRID_SIZE_X;
				int i = b % _GRID_SIZE_X;

				float newU = _u[idx];
				float newV = _v[idx];
				float newW = _w[idx];

				fd::gradient(i, j, k, _HALF_RDX, newU, newV, newW, _p, _GRID_SIZE_X, _GRID_SIZE_Y);
				_u[idx] = newU;
				_v[idx] = newV;
				_w[idx] = newW;
			}
		});
	}

	void Entropy::diffuseVelocity() {

		auto _GRID_SIZE_X = GRID_SIZE_X;
		auto _GRID_SIZE_Y = GRID_SIZE_Y;

		auto _DIFFUSION_ALPHA = DIFFUSION_ALPHA;
		auto _DIFFUSION_BETA_RECIPROCAL = DIFFUSION_BETA_RECIPROCAL;

		auto &_u = getU();
		auto &_v = getV();
		auto &_w = getW();

		auto &_u_prev = getTemp00();
		auto &_v_prev = getTemp01();
		auto &_w_prev = getTemp02();

		auto &_cellType = getCellType();

		// Diffuse Velocity
		for (long int iter = 0; iter < DIFF_ITER; iter++) {

			xamp::parallel_for_each(extent, [_DIFFUSION_ALPHA, _DIFFUSION_BETA_RECIPROCAL, _GRID_SIZE_X, _GRID_SIZE_Y,
				&_cellType, &_u, &_v, &_w, &_u_prev, &_w_prev, &_v_prev](xamp::index<1> idx) restrict(amp) {

				if (_cellType[idx] != boom::constants::BOUNDARY) {

					int a = (_GRID_SIZE_X * _GRID_SIZE_Y);
					int k = idx[0] / a;
					int b = idx[0] - a * k;
					int j = b / _GRID_SIZE_X;
					int i = b % _GRID_SIZE_X;

					_u_prev[idx] = fd::jacobi(idx, i, j, k, _u, _u, _DIFFUSION_ALPHA, _DIFFUSION_BETA_RECIPROCAL, _GRID_SIZE_X, _GRID_SIZE_Y);
					_v_prev[idx] = fd::jacobi(idx, i, j, k, _v, _v, _DIFFUSION_ALPHA, _DIFFUSION_BETA_RECIPROCAL, _GRID_SIZE_X, _GRID_SIZE_Y);
					_w_prev[idx] = fd::jacobi(idx, i, j, k, _w, _w, _DIFFUSION_ALPHA, _DIFFUSION_BETA_RECIPROCAL, _GRID_SIZE_X, _GRID_SIZE_Y);
				}
			});

			fd::swapFloat(_u, _u_prev);
			fd::swapFloat(_v, _v_prev);
			fd::swapFloat(_w, _w_prev);

			enforceBoundary();
		}
	}

	void Entropy::diffuseDensity() {

		auto _GRID_SIZE_X = GRID_SIZE_X;
		auto _GRID_SIZE_Y = GRID_SIZE_Y;

		auto _DIFFUSION_ALPHA = DIFFUSION_ALPHA;
		auto _DIFFUSION_BETA_RECIPROCAL = DIFFUSION_BETA_RECIPROCAL;

		auto &_dens = getDens();

		auto &_dens_prev = getTemp00();


		auto &_cellType = getCellType();

		// Diffuse Velocity
		for (long int iter = 0; iter < DIFF_ITER; iter++) {

			xamp::parallel_for_each(extent, [_DIFFUSION_ALPHA, _DIFFUSION_BETA_RECIPROCAL, _GRID_SIZE_X, _GRID_SIZE_Y,
				&_cellType, &_dens, &_dens_prev](xamp::index<1> idx) restrict(amp) {

				if (_cellType[idx] != boom::constants::BOUNDARY) {

					int a = (_GRID_SIZE_X * _GRID_SIZE_Y);
					int k = idx[0] / a;
					int b = idx[0] - a * k;
					int j = b / _GRID_SIZE_X;
					int i = b % _GRID_SIZE_X;

					_dens_prev[idx] = fd::jacobi(idx, i, j, k, _dens, _dens, _DIFFUSION_ALPHA, _DIFFUSION_BETA_RECIPROCAL, _GRID_SIZE_X, _GRID_SIZE_Y);
					
				}
			});

			fd::swapFloat(_dens, _dens_prev);


			enforceBoundary();
		}
	}
	void Entropy::buoyancy() {

		auto &_u = getU();
		auto &_v = getV();
		auto &_w = getW();

		auto _dens = getDens();
		auto _temp = getTemperature();

		auto _rise = BUOY_RISE;
		auto _fall = BUOY_FALL;
		auto _amb_temp = AMB_TEMP;

		auto _DT = DT;

		auto _gravity_x = gravity_x;
		auto _gravity_y = gravity_y;
		auto _gravity_z = gravity_z;

		auto &_cellType = getCellType();

		xamp::parallel_for_each(extent, [&_cellType, &_u, &_v, &_w, &_dens, &_temp, _DT, _rise, _fall, _amb_temp, _gravity_x, _gravity_y, _gravity_z](xamp::index<1> idx) restrict(amp) {

			if (_cellType[idx] != boom::constants::BOUNDARY) {

				float gx = _gravity_x;
				float gy = _gravity_y;
				float gz = _gravity_z;

				float length = xamp::fast_math::sqrtf(gx*gx + gy*gy + gz*gz);

				float ngravx = gx / length;
				float ngravy = gy / length;
				float ngravz = gz / length;
				float alpha = (_fall * _dens[idx] - _rise * (_temp[idx] - _amb_temp));

				float fbuoyx = alpha * ngravx;
				float fbuoyy = alpha * ngravy;
				float fbuoyz = alpha * ngravz;

				_u[idx] = _u[idx] + _DT * fbuoyx;
				_v[idx] = _v[idx] + _DT * fbuoyy;
				_w[idx] = _w[idx] + _DT * fbuoyz;
			}
		});
	}

	void Entropy::vorticity() {
		fd::vorticityConfinement(getVorticity(), getTimeStep(), getU(), getTemp00(), getV(), getTemp01(), getW(), getTemp02(), getCellType(), GRID_SIZE_X, GRID_SIZE_Y, GRID_SIZE_Z);
	}

	void Entropy::dissipation() {
		fd::dissipation(getDensityDissipation(), getU(), getV(), getW(), getDens(), getCellType(), getTimeStep());
	}

	void Entropy::initBoundary() {

		auto _GRID_SIZE_X = GRID_SIZE_X;
		auto _GRID_SIZE_Y = GRID_SIZE_Y;
		auto _GRID_SIZE_Z = GRID_SIZE_Z;

		auto &_cellType = getCellType();

		xamp::parallel_for_each(extent, [_GRID_SIZE_X, _GRID_SIZE_Y, _GRID_SIZE_Z, &_cellType](xamp::index<1> idx) restrict(amp)  {
			int a = (_GRID_SIZE_X * _GRID_SIZE_Y);
			int k = idx[0] / a;
			int b = idx[0] - a * k;
			int j = b / _GRID_SIZE_X;
			int i = b % _GRID_SIZE_X;

			if (i == 0 || i == _GRID_SIZE_X - 1 || j == 0 || j == _GRID_SIZE_Y - 1 || k == 0 || k == _GRID_SIZE_Z - 1) {
				_cellType[idx] = boom::constants::BOUNDARY;
			}
		});
	}

	void Entropy::enforceBoundary() {

		auto &_u = getU();
		auto &_v = getV();
		auto &_w = getW();
		auto &_p = getP();
		auto &_dens = getDens();

		auto _GRID_SIZE_X = GRID_SIZE_X;
		auto _GRID_SIZE_Y = GRID_SIZE_Y;
		auto _GRID_SIZE_Z = GRID_SIZE_Z;

		xamp::parallel_for_each(extent, [_GRID_SIZE_X, _GRID_SIZE_Y, _GRID_SIZE_Z, &_u, &_v, &_w, &_p, &_dens](xamp::index<1> idx) restrict(amp) {
			int rtBnd = _GRID_SIZE_X - 1;
			int tpBnd = _GRID_SIZE_Y - 1;
			int ftBnd = _GRID_SIZE_Z - 1;

			int a = (_GRID_SIZE_X * _GRID_SIZE_Y);
			int k = idx[0] / a;
			int b = idx[0] - a * k;
			int j = b / _GRID_SIZE_X;
			int i = b % _GRID_SIZE_X;

			// Left Boundary
			if (i == 0) {
				_u[idx] = -1.f * _u[fd::index3d(1, j, k, _GRID_SIZE_X, _GRID_SIZE_Y)];
				_v[idx] = -1.f * _v[fd::index3d(1, j, k, _GRID_SIZE_X, _GRID_SIZE_Y)];
				_w[idx] = -1.f * _w[fd::index3d(1, j, k, _GRID_SIZE_X, _GRID_SIZE_Y)];
				_p[idx] = 1.f * _p[fd::index3d(1, j, k, _GRID_SIZE_X, _GRID_SIZE_Y)];
				_dens[idx] = 1.f * _dens[fd::index3d(1, j, k, _GRID_SIZE_X, _GRID_SIZE_Y)];
			}

			// Right Boundary
			if (i == rtBnd) {
				_u[idx] = -1.f * _u[fd::index3d(rtBnd - 1, j, k, _GRID_SIZE_X, _GRID_SIZE_Y)];
				_v[idx] = -1.f * _v[fd::index3d(rtBnd - 1, j, k, _GRID_SIZE_X, _GRID_SIZE_Y)];
				_w[idx] = -1.f * _w[fd::index3d(rtBnd - 1, j, k, _GRID_SIZE_X, _GRID_SIZE_Y)];
				_p[idx] = 1.f * _p[fd::index3d(rtBnd - 1, j, k, _GRID_SIZE_X, _GRID_SIZE_Y)];
				_dens[idx] = 1.f * _dens[fd::index3d(rtBnd - 1, j, k, _GRID_SIZE_X, _GRID_SIZE_Y)];
			}

			// Bottom Boundary
			if (j == 0) {
				_u[idx] = -1.f * _u[fd::index3d(i, 1, k, _GRID_SIZE_X, _GRID_SIZE_Y)];
				_v[idx] = -1.f * _v[fd::index3d(i, 1, k, _GRID_SIZE_X, _GRID_SIZE_Y)];
				_w[idx] = -1.f * _w[fd::index3d(i, 1, k, _GRID_SIZE_X, _GRID_SIZE_Y)];
				_p[idx] = 1.f * _p[fd::index3d(i, 1, k, _GRID_SIZE_X, _GRID_SIZE_Y)];
				_dens[idx] = 1.f * _dens[fd::index3d(i, 1, k, _GRID_SIZE_X, _GRID_SIZE_Y)];
			}

			// Top Boundary
			if (j == tpBnd) {
				_u[idx] = -1.f * _u[fd::index3d(i, tpBnd - 1, k, _GRID_SIZE_X, _GRID_SIZE_Y)];
				_v[idx] = -1.f * _v[fd::index3d(i, tpBnd - 1, k, _GRID_SIZE_X, _GRID_SIZE_Y)];
				_w[idx] = -1.f * _w[fd::index3d(i, tpBnd - 1, k, _GRID_SIZE_X, _GRID_SIZE_Y)];
				_p[idx] = 1.f * _p[fd::index3d(i, tpBnd - 1, k, _GRID_SIZE_X, _GRID_SIZE_Y)];
				_dens[idx] = 1.f * _dens[fd::index3d(i, tpBnd - 1, k, _GRID_SIZE_X, _GRID_SIZE_Y)];
			}

			// Front Boundary
			if (k == 0) {
				_u[idx] = -1.f * _u[fd::index3d(i, j, 1, _GRID_SIZE_X, _GRID_SIZE_Y)];
				_v[idx] = -1.f * _v[fd::index3d(i, j, 1, _GRID_SIZE_X, _GRID_SIZE_Y)];
				_w[idx] = -1.f * _w[fd::index3d(i, j, 1, _GRID_SIZE_X, _GRID_SIZE_Y)];
				_p[idx] = 1.f *  _p[fd::index3d(i, j, 1, _GRID_SIZE_X, _GRID_SIZE_Y)];
				_dens[idx] = 1.f * _dens[fd::index3d(i, j, 1, _GRID_SIZE_X, _GRID_SIZE_Y)];
			}

			// Back Boundary
			if (k == ftBnd) {
				_u[idx] = -1.f * _u[fd::index3d(i, j, ftBnd - 1, _GRID_SIZE_X, _GRID_SIZE_Y)];
				_v[idx] = -1.f * _v[fd::index3d(i, j, ftBnd - 1, _GRID_SIZE_X, _GRID_SIZE_Y)];
				_w[idx] = -1.f * _w[fd::index3d(i, j, ftBnd - 1, _GRID_SIZE_X, _GRID_SIZE_Y)];
				_p[idx] = 1.f * _p[fd::index3d(i, j, ftBnd - 1, _GRID_SIZE_X, _GRID_SIZE_Y)];
				_dens[idx] = 1.f * _dens[fd::index3d(i, j, ftBnd - 1, _GRID_SIZE_X, _GRID_SIZE_Y)];
			}
		});
	}

	void Entropy::emitFixedEmitters() {
		if (fixedEmitterSettings.size() > 0) {

			auto &_u = getU();
			auto &_v = getV();
			auto &_w = getW();

			auto &_dens = getDens();
			auto &_temp = getTemperature();
			auto &_fixedEmitters = getFixedEmitters();

			xamp::array<emitters::FixedEmitterSettings> _emitterSettings(fixedEmitterSettings.size());
			xamp::copy(fixedEmitterSettings.begin(), _emitterSettings);

			xamp::parallel_for_each(extent, [&_fixedEmitters, &_dens, &_temp, &_emitterSettings, &_u, &_v, &_w](xamp::index<1> idx) restrict(amp) {

				int flag = _fixedEmitters[idx];
				if (flag != -1) {
					int emitterID = _fixedEmitters[idx];

					_dens[idx] = _emitterSettings[emitterID].density;
					_temp[idx] = _emitterSettings[emitterID].temperature;

					if (_emitterSettings[emitterID].useVelocity) {
						_u[idx] += _emitterSettings[emitterID].velocity.x;
						_v[idx] += _emitterSettings[emitterID].velocity.y;
						_w[idx] += _emitterSettings[emitterID].velocity.z;
					}
				}
			});

		}
	}

	void Entropy::addFixedEmitter(std::vector<int> &emitterMap, emitters::FixedEmitterSettings settings) {

		int _emitterID = fixedEmitterSettings.size();
		fixedEmitterSettings.push_back(settings);

		auto &_fixedEmitters = getFixedEmitters();

		xamp::array<int> _emitterMap(emitterMap.size());
		xamp::copy(emitterMap.begin(), _emitterMap);

		xamp::parallel_for_each(extent, [_emitterID, &_fixedEmitters, &_emitterMap](xamp::index<1> idx) restrict(amp) {

			int flag = _emitterMap[idx];
			if (flag != -1) {
				_fixedEmitters[idx] = _emitterID;
			}
		});
	}

	int Entropy::index3d(int i, int j, int k, int GRID_SIZE_X, int GRID_SIZE_Y) {

		int index = i + j * GRID_SIZE_X + GRID_SIZE_X * GRID_SIZE_Y * k;

		return index;
	}
}