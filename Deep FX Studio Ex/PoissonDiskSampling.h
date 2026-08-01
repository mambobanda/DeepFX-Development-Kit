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

#ifndef PoissonDiskSampling_H
#define PoissonDiskSampling_H

#include <Eigen/Dense>

#include <random>
#include <unordered_map>
#include <string>

namespace SPH
{
	typedef float Real;
	using Vector3r = Eigen::Matrix<Real, 3, 1, Eigen::DontAlign>;

	/** \brief This class implements a Poisson disk sampling for the surface
	* of 3D models. 
	*/
	class xPoissonDiskSampling
	{
		typedef Eigen::Matrix<int, 3, 1, Eigen::DontAlign> CellPos;

		struct CellPosHasher
		{
			std::size_t operator()(const CellPos& k) const
			{
				const int p1 = 73856093 * k[0];
				const int p2 = 19349663 * k[1];
				const int p3 = 83492791 * k[2];
				return (size_t)(p1 + p2 + p3);
			}
		};
	public:
		xPoissonDiskSampling();

		/** \brief Struct to store the information of the initial points
		*/
		struct InitialPointInfo
		{
			CellPos cP;
			Vector3r pos;
			unsigned int ID;
		};

		/** \brief Struct to store the hash entry (spatial hashing)
		*/
		struct HashEntry
		{
			HashEntry() { startIndex = 0; };
			std::vector<unsigned int> samples;
			unsigned int startIndex;
		};

		inline static int floor(const Real v)
		{
			return (int)(v + 32768.f) - 32768;			// Shift to get positive values 
		}

		/** Performs the poisson sampling with the
		* respective parameters. Compare
		* http://graphics.cs.umass.edu/pubs/sa_2010.pdf
		*
		* @param mesh mesh data of sampled body
		* @param vertices vertex data of sampled data
		* @param sampledVertices sampled vertices that will be returned
		* @param minRadius minimal distance of sampled vertices
		* @param numTestpointsPerFace # of generated test points per face of body
		* @param distanceNorm 0: euclidean norm, 1: approx geodesic distance
		* @param numTrials # of iterations used to find samples
		*/
		void sampleMesh(const unsigned int numVertices, const Vector3r *vertices, const unsigned int numFaces, const unsigned int *faces,
			const Real minRadius, const unsigned int numTrials,
			unsigned int distanceNorm, std::vector<Vector3r> &samples);
		
	private:
		Real m_r;
		unsigned int m_numTrials;
		unsigned int m_numTestpointsPerFace;
		unsigned int m_distanceNorm;
		std::vector<Vector3r> m_faceNormals;
		std::vector<Real> m_areas;
		Real m_totalArea;

		Real m_cellSize;
		Vector3r m_minVec;
		Vector3r m_maxVec;

		std::vector<InitialPointInfo> m_initialInfoVec;
		std::vector<std::vector<CellPos>> m_phaseGroups;

		Real m_maxArea;

		void computeFaceNormals(const unsigned int numVertices, const Vector3r *vertices, const unsigned int numFaces, const unsigned int *faces);
		void determineTriangleAreas(const unsigned int numVertices, const Vector3r *vertices, const unsigned int numFaces, const unsigned int *faces);
		void generateInitialPointSet(const unsigned int numVertices, const Vector3r *vertices, const unsigned int numFaces, const unsigned int *faces);
		unsigned int getAreaIndex(const std::vector<Real>& areas, const Real totalArea, std::default_random_engine &generator, std::uniform_real_distribution<Real> &distribution);
		void parallelUniformSurfaceSampling(std::vector<Vector3r> &samples);

		void quickSort(int left, int right);
		int partition(int left, int right);
		bool compareCellID(CellPos& a, CellPos& b);

		void determineMinX(const unsigned int numVertices, const Vector3r *vertices);

		bool nbhConflict(const std::unordered_map<CellPos, HashEntry, CellPosHasher>& kvMap, const InitialPointInfo& iPI);
		bool checkCell(const std::unordered_map<CellPos, HashEntry, CellPosHasher>& kvMap, const CellPos& cell, const InitialPointInfo& iPI);
		

	};
}

#endif // PoissonDiskSampling_H
