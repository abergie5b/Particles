//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "DO_NOT_MODIFY\OpenGLInterface.h"

#include "ParticleEmitterOld.h"
#include "Settings.h"

#include "Memory/Mem.h"


extern PerformanceTimer GlobalTimer;

static const unsigned char squareColors[] =
{
	// ----------------------------
	//  point is actually a quad   
	//  set color on each vertex   
	// ----------------------------
	//    Vert A = Yellow 
	//    Vert B = Yellow
	//    Vert C = Yellow
	//    Vert D = Yellow
	// ----------------------------

	200,  200,  0,  255,
	200,  200,  0,  255,
	200,  200,  0,  255,
	200,  200,  0,  255,
};

static const float squareVertices[] =
{
	// --------------------
	//   Vert A = (x,y,z)
	//   Vert B = (x,y,z)
	//   Vert C = (x,y,z)
	//   Vert D = (x,y,z)
	// --------------------

	-0.015f,  -0.015f, 0.0f, // Size of Triangle
	 0.015f,  -0.015f, 0.0f, // Size of Triangle
	-0.015f,   0.015f, 0.0f, // Size of Triangle
	 0.015f,   0.015f, 0.0f, // Size of Triangle
};

ParticleEmitterOld::ParticleEmitterOld()
	: start_position(0.0f, 0.0f, 0.0f),
	start_velocity(0.0f, 1.0f, 0.0f),
	max_life(MAX_LIFE),
	max_particles(NUM_PARTICLES),
	spawn_frequency(0.00005f),
	last_active_particle(0),
	particle_list(NUM_PARTICLES),
	vel_variance(1.0f, 2.0f, 0.4f),
	pos_variance(1.0f, 1.0f, 1.0f),
	scale_variance(0.5f),
	headParticle(0)
{
	GlobalTimer.Toc();
	memory.initialize();

	last_spawn = GlobalTimer.TimeInSeconds();
	last_loop = GlobalTimer.TimeInSeconds();
}

ParticleEmitterOld::~ParticleEmitterOld()
{
	// do nothing
}


void ParticleEmitterOld::SpawnParticle()
{
	// create another particle if there are ones free
	if (last_active_particle < max_particles)
	{

		// create new particle
		Particle* newParticle = (Particle*)memory.malloc(sizeof(Particle));
		//Particle* newParticle = new Particle();

		// initialize the particle
		newParticle->Initialize(start_position, start_velocity, last_active_particle);
		this->ApplyVariance(newParticle->position, newParticle->velocity, newParticle->scale);

		// increment count
		last_active_particle++;

		// add to list
		this->addParticleToList(newParticle);

	}
}

void ParticleEmitterOld::update()
{
	// get current time
	GlobalTimer.Toc();
	float current_time = GlobalTimer.TimeInSeconds();

	// spawn particles
	float time_elapsed = current_time - last_spawn;

	// update
	while (spawn_frequency < time_elapsed)
	{
		// spawn a particle
		this->SpawnParticle();
		// adjust time
		time_elapsed -= spawn_frequency;
		// last time
		last_spawn = current_time;
	}

	// total elapsed
	time_elapsed = current_time - last_loop;

	Particle* p = this->headParticle;
	while (p != nullptr)
	{
		// call every particle and update its position 
		p->Update(time_elapsed);

		// if it's live is greater that the max_life 
		// and there is some on the list
		// remove node
		if ((p->life > max_life) && (last_active_particle > 0))
		{
			// particle to remove
			Particle *s = p;

			// need to squirrel it away.
			p = p->next;

			// remove last node
			this->removeParticleFromList(s);

			// update the number of particles
			last_active_particle--;
		}
		else
		{
			// increment to next point
			p = p->next;
		}
	}

	//move a copy to vector for faster iterations in draw
	p = this->headParticle;

	// walk the pointers, add to list
	while (p != nullptr)
	{
		// add to buffer
		drawBuffer.push_back(*p);

		// advance ptr
		p = p->next;

	}
	last_loop = current_time;
}

void ParticleEmitterOld::addParticleToList(Particle *p)
{
	assert(p);
	if (this->headParticle == nullptr)
	{ // first on list
		this->headParticle = p;
		p->next = nullptr;
		p->prev = nullptr;
	}
	else
	{ // add to front of list
		headParticle->prev = p;
		p->next = headParticle;
		p->prev = nullptr;
		headParticle = p;
	}

}

void ParticleEmitterOld::removeParticleFromList(Particle *p)
{
	// make sure we are not screwed with a null pointer
	assert(p);

	if (p->prev == nullptr && p->next == nullptr)
	{ // only one on the list
		this->headParticle = nullptr;
	}
	else if (p->prev == nullptr && p->next != nullptr)
	{ // first on the list
		p->next->prev = nullptr;
		this->headParticle = p->next;
	}
	else if (p->prev != nullptr && p->next == nullptr)
	{ // last on the list 
		p->prev->next = nullptr;
	}
	else//( p->next != nullptr  && p->prev !=nullpttr )
	{ // middle of the list
		p->prev->next = p->next;
		p->next->prev = p->prev;
	}

	// bye bye
	memory.free(p);
	//delete p;
}

void ParticleEmitterOld::renderParticle(std::list<Particle>::iterator it)
{
	// OpenGL goo... don't worrry about this
	glVertexPointer(3, GL_FLOAT, 0, squareVertices);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, squareColors);
	glEnableClientState(GL_COLOR_ARRAY);

	// camera position
	transCamera.setTransMatrix(&camPosVect);

	// particle position
	vTrans = it->position * 0.25f;
	transParticle.setTransMatrix(&(vTrans));

	// rotation matrix
	rotParticle.setRotZMatrix(-it->rotation);

	// pivot Point
	pivotVect.set(1.0f, 0.0f, -550.0f);
	pivotVect = pivotVect * 20.0f * it->life;

	pivotParticle.setTransMatrix(&pivotVect);

	// scale Matrix
	scaleMatrix.setScaleMatrix(&it->scale);

	// total transformation of particle
	tmp = scaleMatrix * transCamera * transParticle * rotParticle * scaleMatrix;

	// set the transformation matrix
	glLoadMatrixf(reinterpret_cast<float*>(&(tmp)));

	// squirrel away matrix for next update
	it->curr_Row0 = tmp.v0;
	it->curr_Row1 = tmp.v1;
	it->curr_Row2 = tmp.v2;
	it->curr_Row3 = tmp.v3;

	// draw the trangle strip
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	// difference vector     
	it->diff_Row0 = it->curr_Row0 - it->prev_Row0;
	it->diff_Row1 = it->curr_Row1 - it->prev_Row1;
	it->diff_Row2 = it->curr_Row2 - it->prev_Row2;
	it->diff_Row3 = it->curr_Row3 - it->prev_Row3;

	// clears or flushes some internal setting, used in debug, but is need for performance reasons
	// magic...  really it's magic.
	// do not delete these two lines
	GLenum glError = glGetError();
	AZUL_UNUSED_VAR(glError);

}

void ParticleEmitterOld::draw()
{
	// get the camera matrix from OpenGL
	glGetFloatv(GL_MODELVIEW_MATRIX, reinterpret_cast<float*>(&cameraMatrix));
	camPosVect = cameraMatrix.v3;

	// iterate throught the list of particles
	std::list<Particle>::iterator it;
	for (it = drawBuffer.begin(); it != drawBuffer.end(); ++it)
	{
		renderParticle(it);
	}

	// delete the buffer
	for (size_t i = 0; i < drawBuffer.size(); i++)
	{
		drawBuffer.pop_back();
	}

	// done with buffer, clear it.
	drawBuffer.clear();

}

void ParticleEmitterOld::ApplyVariance(Vect4D& pos, Vect4D& vel, Vect4D& sc)
{
	// x - variance
	float var = static_cast<float>(rand() % 1000) * 0.005f; // Var
	float sign = static_cast<float>(rand() % 2);  // Sign 
	float *t_pos = reinterpret_cast<float*>(&pos);
	float* t_var = &pos_variance.x;
	if (sign == 0)
	{
		var *= -1.0f;
	}
	*t_pos += *t_var * var;

	// y - variance
	var = static_cast<float>(rand() % 1000) * 0.001f;
	sign = static_cast<float>(rand() % 2);
	t_pos++;
	t_var++;
	if (sign == 0)
	{
		var *= -1.0f;
	}
	*t_pos += *t_var * var;

	// z - variance
	var = static_cast<float>(rand() % 1000) * 0.001f;
	sign = static_cast<float>(rand() % 2);
	t_pos++;
	t_var++;
	if (sign == 0)
	{
		var *= -1.0f;
	}
	*t_pos += *t_var * var;

	var = static_cast<float>(rand() % 1000) * 0.001f;
	sign = static_cast<float>(rand() % 2);

	// x  - add velocity
	t_pos = &vel.x;
	t_var = &vel_variance.x;
	if (sign == 0)
	{
		var *= -1.0f;
	}
	*t_pos += *t_var * var;

	// y - add velocity
	var = static_cast<float>(rand() % 1000) * 0.001f;
	sign = static_cast<float>(rand() % 2);
	t_pos++;
	t_var++;
	if (sign == 0)
	{
		var *= -1.0f;
	}
	*t_pos += *t_var * var;

	// z - add velocity
	var = static_cast<float>(rand() % 1000) * 0.001f;
	sign = static_cast<float>(rand() % 2);
	t_pos++;
	t_var++;
	if (sign == 0)
	{
		var *= -1.0f;
	}
	*t_pos += *t_var * var;

	// correct the sign
	var = 1.5f * static_cast<float>(rand() % 1000) * 0.001f;
	sign = static_cast<float>(rand() % 2);  

	if (sign == 0)
	{
		var *= -1.0f;
	}
	sc *= var;
}


void ParticleEmitterOld::GoodBye()
{
	Particle *pTmp = this->headParticle;
	Particle *pDeadMan = nullptr;
	while (pTmp)
	{
		pDeadMan = pTmp;
		pTmp = pTmp->next;
		delete pDeadMan;
	}
}

// End of file
