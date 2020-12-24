//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef PARTICLEEMITTEROLD_H
#define PARTICLEEMITTEROLD_H

#include "Math\Matrix.h"
#include "Math\Vect4D.h"
#include "Particle.h"
#include "Memory/Mem.h"

#include <list>

class ParticleEmitterOld
{
public:
	ParticleEmitterOld();
	~ParticleEmitterOld();

	void SpawnParticle();

	void update();
	void draw();
	void renderParticle(std::list<Particle>::iterator it);

	void addParticleToList(Particle *p);
	void removeParticleFromList(Particle *p);

	void ApplyVariance(Vect4D& pos, Vect4D& vel, Vect4D& sc);
	void GoodBye();

private:
	std::list<Particle> drawBuffer;
	std::list<Particle> particle_list;

	Particle *headParticle;

	Matrix cameraMatrix;
	Matrix transParticle;
	Matrix rotParticle;
	Matrix scaleMatrix;
	Matrix pivotParticle;
	Matrix transCamera;
	Matrix tmp;

	Vect4D camPosVect;
	Vect4D pivotVect;
	Vect4D vTrans;

	Vect4D	start_position;
	Vect4D	start_velocity;
	Vect4D	vel_variance;
	Vect4D	pos_variance;

	Mem memory;

	int     bufferCount;
	int		max_particles;
	int		last_active_particle;

	float	max_life;
	float	last_loop;
	float	last_spawn;
	float	scale_variance;
	float	spawn_frequency;

};

#endif

// End of File
