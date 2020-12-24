#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vect4D.h"
#include "Matrix.h"
#include "Settings.h"

class Particle : public Align16
{
public:
	friend class ParticleEmitter;

	Particle();
	Particle(const Particle& p) = default;
	Particle& operator=(const Particle& p) = default;
	~Particle();

	Particle(const Vect4D, const Vect4D, const Vect4D);

	void Update(const float& time_elapsed);
	inline void Initialize(const Vect4D start_position, const Vect4D start_velocity, const Vect4D start_scale)
	{
		this->position = start_position;
		this->velocity = start_velocity;
		this->scale = start_scale;
		this->life = START_LIFE;
		this->rotation = START_ROTATION;
		this->rotation_velocity = START_ROTATION_VELOCITY;
		this->isFree = true;
	}


private:
	Particle *next;
	Particle *prev;

	Matrix tmp;

	Vect4D	prev_Row0;
	Vect4D	prev_Row1;
	Vect4D  prev_Row2;
	Vect4D  prev_Row3;

	Vect4D	position;
	Vect4D	velocity;
	Vect4D	scale;

	Vect4D	curr_Row0;
	Vect4D	curr_Row1;
	Vect4D  curr_Row2;
	Vect4D  curr_Row3;

	Vect4D tmp_Row0;
	Vect4D tmp_Row1;
	Vect4D tmp_Row2;
	Vect4D tmp_Row3;

	float	life;
	float	rotation;
	float	rotation_velocity;

	bool isFree;

};


#endif 

// End of File
