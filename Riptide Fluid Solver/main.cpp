#ifdef _MAIN_TEST

#include <iostream>
#include <glm/gtx/transform.hpp>
//#include "Timer.h"
#include "ParticleVisualizer.h"
#include "SimulatorBase.h"
#include "RtxCompactSearch.h"
#include "RtxSPHFunctions.h"
#include "RtxCompactXSearch.h"
#include "RtxRiptideKernels.h"
#include "RtxParticleSystem.h"

float rdensity = 1000.f;
float pradius  = 0.02;
float pspacing = pradius * 2.f;
float _h       = pradius * 4.f;
float diam = pradius * 2.f;
float mass = diam * diam *diam * rdensity;
float dt   = 0.001;

static rtx::SimulatorBase simbase;
static rtx::CompactSearch compact_search;
static rtx::CompactXSearch xsearch;
static rtx::PrecomputedKernel<rtx::CubicKernel> kernel;
//static rtx::CubicKernel kernel;

rtx::ParticleSystem psystem;

uint8_t BoundaryMethod = rtx::AKINCI2012;

glm::vec3 gravity(0.f, -9.f, 0.f);
float viscosity = 0.01f;
float boundary_viscosity = 0.00f;
float vorticity = 0.02f;
float density_error = 0.01f;
float coherence_error = 0.1f;
float pressure_stiffness = 1.f;
float velocity_dampening = 0.000f;

float fps = 30.f;

int STEPS = 4;
int frame = 0;
float globaLTime = 0;
void update_function00(){

	float totalTime = 0.f;
	//while(totalTime < (1.f / fps )) {
	for(int i=0; i < STEPS; i++) {
		// Calculate forces
		rtx::calc_gravity(
			simbase.sphSystem()->nbParticles(),
			gravity,
			simbase.sphSystem()->alive(),
			simbase.sphSystem()->type(),
			simbase.sphSystem()->forces());

		rtx::calc_viscosity(
			kernel,
			BoundaryMethod,
			simbase.sphSystem()->nbParticles(),
			simbase.sphSystem()->nbListSize(),
			mass,
			_h,
			simbase.sphSystem()->useViscosity(),
			simbase.sphSystem()->viscosity(),
			simbase.sphSystem()->positions(),
			simbase.sphSystem()->velocity(),
			simbase.sphSystem()->alive(),
			simbase.sphSystem()->type(),
			simbase.sphSystem()->nbCount(),
			simbase.sphSystem()->neighbours(), 
			simbase.sphSystem()->boundary_mass(),
			simbase.sphSystem()->forces(),
			simbase.sphSystem()->density());

		dt = rtx::updateTimeStepSizeCFL(
			simbase.sphSystem()->nbParticles(),
			simbase.sphSystem()->velocity(),
			simbase.sphSystem()->forces(),
			dt,
			pradius,
			0.001f,
			0.001f);

		//rtx::calc_vorticity_confinement(
		//	kernel,
		//	simbase.sphSystem()->nbParticles(),
		//	simbase.sphSystem()->nbListSize(),
		//	mass,
		//	vorticity,
		//	simbase.sphSystem()->positions(),
		//	simbase.sphSystem()->velocity(),
		//	simbase.sphSystem()->alive(),
		//	simbase.sphSystem()->type(),
		//	simbase.sphSystem()->nbCount(),
		//	simbase.sphSystem()->neighbours(),
		//	simbase.sphSystem()->forces());

		// Advance Velocity
		rtx::advance_velocity(
			simbase.sphSystem()->nbParticles(),
			dt,
			simbase.sphSystem()->velocity(),
			simbase.sphSystem()->velocity_i(),
			simbase.sphSystem()->alive(),
			simbase.sphSystem()->type(),
			simbase.sphSystem()->forces());

		// Correct Density Error
		rtx::correct_density_error(
			kernel,
			BoundaryMethod,
			simbase.sphSystem()->nbParticles(),
			simbase.sphSystem()->nbListSize(),
			mass,
			rdensity,
			density_error,
			dt,
			_h,
			pressure_stiffness,
			simbase.sphSystem()->positions(),
			simbase.sphSystem()->velocity_i(),
			simbase.sphSystem()->alive(),
			simbase.sphSystem()->type(),
			simbase.sphSystem()->nbCount(),
			simbase.sphSystem()->neighbours(),
			simbase.sphSystem()->boundary_mass(),
			simbase.sphSystem()->density(),
			simbase.sphSystem()->density_i(),
			simbase.sphSystem()->density_gradient(),
			simbase.sphSystem()->alpha(),
			simbase.sphSystem()->pressure_force(),
			simbase.sphSystem()->ki00());
		
		// Update Positions
		rtx::calc_position(
			simbase.sphSystem()->nbParticles(),
			dt,
			simbase.sphSystem()->frozen(),
			simbase.sphSystem()->positions(),
			simbase.sphSystem()->velocity_i(),
			simbase.sphSystem()->alive(),
			simbase.sphSystem()->type());

		xsearch.findNeighbours();

		rtx::calc_densities(
			kernel,
			BoundaryMethod,
			simbase.sphSystem()->nbParticles(),
			simbase.sphSystem()->nbListSize(),
			mass,
			simbase.sphSystem()->positions(),
			simbase.sphSystem()->alive(),
			simbase.sphSystem()->type(),
			simbase.sphSystem()->nbCount(),
			simbase.sphSystem()->neighbours(),
			simbase.sphSystem()->boundary_mass(),
			simbase.sphSystem()->density());

		rtx::calc_alpha(
			kernel,
			BoundaryMethod,
			simbase.sphSystem()->nbParticles(),
			simbase.sphSystem()->nbListSize(),
			mass,
			simbase.sphSystem()->positions(),
			simbase.sphSystem()->alive(),
			simbase.sphSystem()->type(),
			simbase.sphSystem()->nbCount(),
			simbase.sphSystem()->neighbours(),
			simbase.sphSystem()->boundary_mass(),
			simbase.sphSystem()->alpha());

		// Correct Divergence
		rtx::correct_divergence_error(
			kernel,
			BoundaryMethod,
			simbase.sphSystem()->nbParticles(),
			simbase.sphSystem()->nbListSize(),
			mass,
			rdensity,
			coherence_error,
			dt,
			_h,
			simbase.sphSystem()->positions(),
			simbase.sphSystem()->velocity_i(),
			simbase.sphSystem()->alive(),
			simbase.sphSystem()->type(),
			simbase.sphSystem()->nbCount(),
			simbase.sphSystem()->neighbours(),
			simbase.sphSystem()->boundary_mass(),
			simbase.sphSystem()->density_gradient(),
			simbase.sphSystem()->alpha(),
			simbase.sphSystem()->pressure_force(),
			simbase.sphSystem()->ki01());

		// Correct Velocites
		rtx::damp_velocity(
			simbase.sphSystem()->nbParticles(),
			velocity_dampening,
			simbase.sphSystem()->frozen(),
			simbase.sphSystem()->velocity(),
			simbase.sphSystem()->velocity_i(),
			simbase.sphSystem()->alive(),
			simbase.sphSystem()->type());

		totalTime += dt;
		globaLTime += dt;
	}

	//cout << "Dt: " << dt << std::endl;
	//cout << "Time: " << globaLTime << std::endl;
}

void update_function01() {
	psystem.integrate();
	psystem.collisionTests();
}

int main(int argc, char **argv) {

	kernel.setRadius(_h);

	// simbase.createDamBreakParticleSystem3D(vec3(-0.3, -0.3, -0.28), 80, 80, 80, pradius, pspacing); 
	simbase.createDamBreakParticleSystem3D(vec3(-0.3, 0.0, -0.28), 9, 9, 9, pradius, pspacing);
	// simbase.createDamBreakParticleSystem3D(vec3(-0.3, -0.f, -0.28), 1, 1, 1, pradius, pspacing);
	// simbase.createDamBreakParticleSystem3D(vec3(-0.1, -0.1, -0.24), 100, 100, 100, pradius, pspacing); 
	simbase.createObstacle3D(vec3(-0.6, -0.2, -0.5), 25, 1, 25, 0.02, 0.04f);
	// simbase.createObstacle3D(vec3(-0.6, -0.2, -1.), 35, 1, 35, 0.02, 0.035);
	// simbase.createBoundaryBox3D((pradius * 1.5f) * 0.5f, 1, 1, 1, 1);
	simbase.createParticleSystem();
	simbase.sphSystem()->initNeighbourList();
	simbase.sphSystem()->clearNeighbourList();


	xsearch.reset(simbase.sphSystem());
	xsearch.setRadius(_h);
	xsearch.findNeighbours();

	psystem.init(simbase.positions().size());
	psystem.addParticles(simbase.positions());
	psystem.getSystem()->initNeighbourList();
	psystem.getSystem()->clearNeighbourList();

	auto xform = glm::translate(glm::mat4(1.f), glm::vec3(-0.2f, -0.3f, 0.f));
	xform *= glm::scale(glm::vec3(0.6f, 0.07f, 0.6f));

	auto cube = rtx::cube();
	cube->transform(xform);
	rtx::ParticleVisualizer::addObject(cube);

	psystem.addCollisionObject(cube, 0.04f);
	//psystem.collisionTests();

	// Calculate Densities and Alpha
	rtx::calc_boundary_masses(
		kernel, 
		simbase.sphSystem()->nbParticles(), 
		simbase.sphSystem()->nbListSize(), 
		rdensity, 
		simbase.sphSystem()->positions(),
		simbase.sphSystem()->alive(),
		simbase.sphSystem()->type(),
		simbase.sphSystem()->nbCount(),
		simbase.sphSystem()->neighbours(),
		simbase.sphSystem()->boundary_mass());

	rtx::calc_densities(
		kernel, 
		BoundaryMethod,
		simbase.sphSystem()->nbParticles(),
		simbase.sphSystem()->nbListSize(), 
		mass,
		simbase.sphSystem()->positions(),
		simbase.sphSystem()->alive(),
		simbase.sphSystem()->type(),
		simbase.sphSystem()->nbCount(),
		simbase.sphSystem()->neighbours(), 
		simbase.sphSystem()->boundary_mass(),
		simbase.sphSystem()->density());
	 
	rtx::calc_alpha(
		kernel,
		BoundaryMethod,
		simbase.sphSystem()->nbParticles(),
		simbase.sphSystem()->nbListSize(),
		mass,
		simbase.sphSystem()->positions(),
		simbase.sphSystem()->alive(),
		simbase.sphSystem()->type(),
		simbase.sphSystem()->nbCount(),
		simbase.sphSystem()->neighbours(),
		simbase.sphSystem()->boundary_mass(),
		simbase.sphSystem()->alpha());

	// Quick Benchmark
	/*int frames = 30;
	rtx::Timer totalTime;
	totalTime.startTimer();
	for (int i = 0; i < frames; i++) {
		update_function();
	}
	totalTime.stopTimer();
	std::cout
		<< "\n================================\n"
		<< "Total Time: " << totalTime.getTime() << "(ms)" << std::endl
		<< "==================================\n";*/

	rtx::ParticleVisualizer::setSPHSystem(simbase.sphSystem());
	// rtx::ParticleVisualizer::setSPHSystem(psystem.getSystem());
	rtx::ParticleVisualizer::initParticleColors();
	rtx::ParticleVisualizer::setParticleRadius(pradius);
	rtx::ParticleVisualizer::setWidth(1280);
	rtx::ParticleVisualizer::setHeight(720);
	rtx::ParticleVisualizer::setShowNeighbourParticles(true);
	rtx::ParticleVisualizer::setInitfunction(update_function00);
	// rtx::ParticleVisualizer::setInitfunction(update_function01);
	rtx::ParticleVisualizer::show(argc, argv);

}

#endif