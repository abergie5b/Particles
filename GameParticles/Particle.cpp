#include "Particle.h"
#include "Matrix.h"

Particle::Particle()
	 :position(Vect4D(0.0f, 0.0f, 0.0f)),
	  velocity(Vect4D(0.0f, 0.0f, 0.0f)),
	  scale(Vect4D(START_SCALE, START_SCALE, START_SCALE)),
	  life(START_LIFE),
	  rotation(START_ROTATION),
	  rotation_velocity(START_ROTATION_VELOCITY),
	  isFree(true)
{
}

Particle::Particle(const Vect4D start_position, const Vect4D start_velocity, const Vect4D start_scale)
	 :position(start_position),
	  velocity(start_velocity),
	  scale(start_scale),
	  life(START_LIFE),
	  rotation(START_ROTATION),
	  rotation_velocity(START_ROTATION_VELOCITY),
	  isFree(true)
{
}

Particle::~Particle()
{
	// nothing to do
}

void Particle::Update(const float& time_elapsed)
{
	tmp.v0 = this->curr_Row0 - this->prev_Row0;
	tmp.v1 = this->curr_Row1 - this->prev_Row1;
	tmp.v2 = this->curr_Row2 - this->prev_Row2;
	tmp.v3 = this->curr_Row3 - this->prev_Row3;

	float MatrixScale = tmp.Determinant();

	// serious math below - magic secret sauce
	life += time_elapsed;
	position = position + (velocity * (time_elapsed));

	Vect4D v;
	position.Cross(v);
	v.norm(v);

	position = position + v * POSITION_FACTOR * life;

	if (MatrixScale > 1.0f)
	{
		MatrixScale = 1.0f / MatrixScale;
	};
	
	rotation = rotation + (MatrixScale + rotation_velocity) * time_elapsed * ROTATION_FACTOR;

	// squirrel away matrix for next update
	this->curr_Row0 = tmp.v0;
	this->curr_Row1 = tmp.v1;
	this->curr_Row2 = tmp.v2;
	this->curr_Row3 = tmp.v3;

	// Rotate the matrices
	prev_Row0 = tmp_Row0;
	tmp_Row0 = curr_Row0;

	prev_Row1 = tmp_Row1;
	tmp_Row1 = curr_Row1;

	prev_Row2 = tmp_Row2;
	tmp_Row2 = curr_Row2;

	prev_Row3 = tmp_Row3;
	tmp_Row3 = curr_Row3;

}


// End of file
