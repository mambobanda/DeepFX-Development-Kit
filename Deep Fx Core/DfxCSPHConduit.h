#pragma once

#pragma once

#include <glm\vec3.hpp>

namespace dfxcore {

	namespace sph {

		class Conduit {
		private:
			typedef float(*W_FPTR)(glm::vec3 &r);
			typedef glm::vec3(*GRAD_W_FPTR)(glm::vec3 &r);

			W_FPTR _W;
			GRAD_W_FPTR _gradW;

		public:
			Conduit();
			void setW(float(*W)(glm::vec3 &r));
			void setGradW(glm::vec3(*gradW)(glm::vec3 &r));
			float W(glm::vec3 &r);
			glm::vec3  gradW(glm::vec3 &r);
		};
	}
}
