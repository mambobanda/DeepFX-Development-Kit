/*
SPlisHSPlasH is an open-source library for the physically-based simulation of fluids.

The MIT License (MIT)

Copyright (c) 2016 Jan Bender

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


*/

#ifndef __RegularTriangleSampling_H__
#define __RegularTriangleSampling_H__

#include <Eigen/Dense>

#include <vector>

namespace SPH
{
	typedef float Real;
	using Vector3r = Eigen::Matrix<Real, 3, 1, Eigen::DontAlign>;
	/** \brief This class implements a per-triangle regular sampling for the surface
	* of 3D models. 
	*/
	class RegularTriangleSampling
	{
	public:
		RegularTriangleSampling();

		/** Performs the poisson sampling with the
		* respective parameters. Compare
		* http://graphics.cs.umass.edu/pubs/sa_2010.pdf
		*
		* @param numVertices number of mesh vertices
		* @param vertices vertex data of sampled data
		* @param numFaces number of faces in the mesh
		* @param faces face data of sampled mesh
		* @param maxDistance maximal distance of sampled vertices
		* @param samples vector to store the samples
		*/
		static void sampleMesh(const unsigned int numVertices, const Vector3r *vertices, const unsigned int numFaces, const unsigned int *faces,
			const Real maxDistance, std::vector<Vector3r> &samples);
		
	private:
		using Vector2ui = Eigen::Matrix<unsigned int, 2, 1, Eigen::DontAlign>;

		static void appendVertexSamples(const unsigned int numVertices, const Vector3r * vertices, std::vector<Vector3r> &samples);
		static void appendEdgeSamples(const Real d, const Vector3r * vertices, const std::vector<Vector2ui> & edges, std::vector<Vector3r> &samples, bool skipVertices = true);
		static void appendFaceSamples(const Real d, const Vector3r * vertices, const unsigned int numFaces, const unsigned int * faces, std::vector<Vector3r> &samples, bool skipEdges = true);
		
		static std::vector<Vector2ui> uniqueEdges(unsigned int numFaces, const unsigned int *faces);
		
	};
}

#endif // __RegularTriangleSampling_H__
