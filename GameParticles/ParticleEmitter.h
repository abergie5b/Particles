#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H

#include "Math\Matrix.h"
#include "Math\Vect4D.h"
#include "Particle.h"

class ParticleEmitter
{
private:
	Particle *headParticle;
	Particle* particlePool;

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
	Vect4D  start_scale;

	Vect4D	vel_variance;
	Vect4D	pos_variance;

	unsigned int randTmp[4];

	int		max_particles;

	float	max_life;
	float	last_loop;
	float	last_spawn;
	float	spawn_frequency;

public:
	int		last_active_particle;

public:
	ParticleEmitter();
	ParticleEmitter(const ParticleEmitter& pe) = default;
	ParticleEmitter& operator=(const ParticleEmitter& pe) = default;
	~ParticleEmitter();

	Particle* InitializeParticles() const;
	inline const void SpawnParticle();

	void update();
	void draw();

	void ApplyVariance(Vect4D& pos, Vect4D& vel, Vect4D& sc);
	void GoodBye();

private:
	inline const void privRemoveParticleFromList(Particle *p);
	inline const void privAddParticleToFrontList(Particle* particle);

	inline const void privAddParticleToFrontPool(Particle* particle);
	inline Particle* privRemoveParticleFromPool();

};

#endif

// End of File
