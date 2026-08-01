/*
* The Mailo Today Solutions Software License
*
* Copyright 2016 Mailo Today Solutions.
*
* No Permission is granted, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software.
*/

#pragma once

#include <glm\vec3.hpp>
#include <glm\geometric.hpp>
#include <glm\gtx\norm.hpp>
#include "Util.h"

static float cubicSpline_radius = 0.0f;
static float cubicSpline_k = 0.0f;
static float cubicSpline_l = 0.0f;

namespace dfxcore {

	namespace sph {

		class CubicSplineKernel {
		private:
			float cubicSpline_radius;
			float cubicSpline_k;
			float cubicSpline_l;
		public:
			CubicSplineKernel() {
				cubicSpline_radius = 0.0f;
				cubicSpline_k = 0.0f;
				cubicSpline_l = 0.0f;
			}

			inline void initCubicSplineKernel(float _h) {

				this->cubicSpline_radius = _h;
				static const float pi = static_cast<float>(xPI);

				const float h3 = this->cubicSpline_radius*this->cubicSpline_radius*this->cubicSpline_radius;
				this->cubicSpline_k = 8.0f / (pi*h3);
				this->cubicSpline_l = 48.0f / (pi*h3);
			}

			inline float cubicSplineW(glm::vec3 &r) {

				float res = 0.0f;
				const float rl = glm::length(r);
				const float q = rl / this->cubicSpline_radius;

				if (q <= 1.f) {
					if (q <= 0.5)
					{
						const float q2 = q*q;
						const float q3 = q2*q;
						res = this->cubicSpline_k * (6.0f*q3 - 6.0f*q2 + 1.0f);
					}
					else
					{
						res = this->cubicSpline_k * (2.0f*pow3(1.0f - q));
					}
				}

				return res;
			}

			inline float cubicSplineW2(float r) {

				float res = 0.0f;
				const float rl = r;
				const float q = rl / this->cubicSpline_radius;

				if (q <= 1.f) {
					if (q <= 0.5f)
					{
						const float q2 = q*q;
						const float q3 = q2*q;
						res = (float)this->cubicSpline_k * (6.0f*q3 - 6.0f*q2 + 1.0f);
					}
					else
					{
						res = (float)this->cubicSpline_k * (2.0f*pow3(1.0f - q));
					}
				}

				return res;
			}

			inline glm::vec3 cubicSplineGradW(glm::vec3 &r) {
				glm::vec3 res(0.0f);
				const float rl = glm::length(r);
				const float q = rl / this->cubicSpline_radius;

				if (q <= 1.f) {
					if (rl > 1.0e-6)
					{
						const glm::vec3 gradq = r * ((float) 1.0f / (rl*this->cubicSpline_radius));
						if (q <= 0.5f)
						{
							res = this->cubicSpline_l*q*((float) 3.0f*q - (float) 2.0f)*gradq;
						}
						else
						{
							const float factor = 1.0f - q;
							res = this->cubicSpline_l*(-factor*factor)*gradq;
						}
					}
				}

				return res;
			}
		};

		static void initCubicSplineKernel(float _h) {
			cubicSpline_radius = _h;
			static const float pi = static_cast<float>(xPI);

			const float h3 = cubicSpline_radius*cubicSpline_radius*cubicSpline_radius;
			cubicSpline_k = 8.0f / (pi*h3);
			cubicSpline_l = 48.0f / (pi*h3);
		}

		static float cubicSplineW(glm::vec3 &r) {

			float res = 0.0f;
			const float rl = glm::length(r);
			const float q = rl / cubicSpline_radius;

			if (q <= 0.5)
			{
				const float q2 = q*q;
				const float q3 = q2*q;
				res = cubicSpline_k * (6.0f*q3 - 6.0f*q2 + 1.0f);
			}
			else
			{
				res = cubicSpline_k * (2.0f*pow3(1.0f - q));
			}

			return res;
		}

		static glm::vec3 cubicSplineGradW(glm::vec3 &r) {

			glm::vec3 res(0.0f);
			const float rl = glm::length(r);
			const float q = rl / cubicSpline_radius;

			if (rl > 1.0e-6)
			{
				const glm::vec3 gradq = r * ((float) 1.0f / (rl*cubicSpline_radius));
				if (q <= 0.5f)
				{
					res = cubicSpline_l*q*((float) 3.0f*q - (float) 2.0f)*gradq;
				}
				else
				{
					const float factor = 1.0f - q;
					res = cubicSpline_l*(-factor*factor)*gradq;
				}
			}

			return res;
		}
	}
}