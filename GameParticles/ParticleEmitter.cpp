#include "DO_NOT_MODIFY\OpenGLInterface.h"

#include "ParticleEmitter.h"
#include "Settings.h"
#include "rand_sse.h"


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

ParticleEmitter::ParticleEmitter()
	: start_position(0.0f, 0.0f, 0.0f),
	  start_velocity(0.0f, 1.0f, 0.0f), 
	  start_scale(START_SCALE, START_SCALE, START_SCALE),
	  max_life(MAX_LIFE),
	  max_particles(NUM_PARTICLES),
	  spawn_frequency(SPAWN_FREQUENCY),
	  last_active_particle(0),
	  vel_variance(1.0f, 2.0f, 0.4f),
	  pos_variance(1.0f, 1.0f, 1.0f)
{
	GlobalTimer.Toc();
	particlePool = this->InitializeParticles();
	this->headParticle = this->privRemoveParticleFromPool();

	last_spawn = GlobalTimer.TimeInSeconds();
	last_loop = GlobalTimer.TimeInSeconds();

}

ParticleEmitter::~ParticleEmitter()
{
}

Particle* ParticleEmitter::InitializeParticles() const
{
	Particle* chunk = (Particle*)_aligned_malloc(NUM_PARTICLES * sizeof(Particle), 16);

#undef new
	Particle* particle = new (chunk) Particle();

	Particle* head = particle;
	Particle* prev = head;

	for (int x=0; x<NUM_PARTICLES; x++)
	{
		head->Initialize(start_position, start_velocity, start_scale);
		head++;
		prev->next = head;
		head->prev = prev;
		prev = prev->next;
	}
	head->next = nullptr;
	particle->prev = nullptr;
	return particle;
}

inline const void ParticleEmitter::privAddParticleToFrontPool(Particle* particle)
{
	particle->prev = nullptr;
	particle->next = this->particlePool;
	this->particlePool->prev = particle;
	this->particlePool = particle;
	particle->isFree = true;
}

inline Particle* ParticleEmitter::privRemoveParticleFromPool()
{
	Particle* head = this->particlePool;
	if (this->particlePool->next)
	{
		this->particlePool = this->particlePool->next;
		this->particlePool->prev = nullptr;
	}
	head->next = nullptr;
	head->prev = nullptr;
	return head;
}


inline const void ParticleEmitter::SpawnParticle()
{
	Particle* particle = this->privRemoveParticleFromPool();

	// initialize the particle
	this->ApplyVariance(particle->position, particle->velocity, particle->scale);

	// increment count
	last_active_particle++;

	// add to list
	this->privAddParticleToFrontList(particle);
}

void ParticleEmitter::update()
{
	// get current time
	GlobalTimer.Toc();
	float current_time = GlobalTimer.TimeInSeconds();

	// spawn particles
	float time_elapsed = current_time - last_spawn;

	// update
	while (spawn_frequency < time_elapsed)
	{
		// create another particle if there are ones free
		if (last_active_particle < max_particles)
		{
			// spawn a particle
			this->SpawnParticle();
		}
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
			this->privRemoveParticleFromList(s);
			this->privAddParticleToFrontPool(s);

			// update the number of particles
			last_active_particle--;
		}
		else
		{
			// increment to next point
			p = p->next;
		}
	}
	last_loop = current_time;
}

inline const void ParticleEmitter::privAddParticleToFrontList(Particle* particle)
{
	particle->prev = nullptr;
	particle->next = this->headParticle;
	this->headParticle->prev = particle;
	this->headParticle = particle;
	particle->isFree = false;
}

inline const void ParticleEmitter::privRemoveParticleFromList(Particle* p)
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

	p->Initialize(start_position, start_velocity, start_scale);
}

void ParticleEmitter::draw()
{
	// get the camera matrix from OpenGL
	glGetFloatv(GL_MODELVIEW_MATRIX, reinterpret_cast<float*>(&cameraMatrix));
	camPosVect = cameraMatrix.v3;

	// iterate throught the list of particles
	Particle* it = this->headParticle;
	while (it != nullptr)
	{
		glVertexPointer(3, GL_FLOAT, 0, squareVertices);
		glEnableClientState(GL_VERTEX_ARRAY);
		glColorPointer(4, GL_UNSIGNED_BYTE, 0, squareColors);
		glEnableClientState(GL_COLOR_ARRAY);

		// camera position
		transCamera.setTransMatrix(&camPosVect);

		// particle position
		vTrans = it->position * POSITION_TRANS;
		transParticle.setTransMatrix(&(vTrans));

		// rotation matrix
		rotParticle.setRotZMatrix(-it->rotation);

		// pivot Point
		pivotVect.set(1.0f, 0.0f, -550.0f);
		pivotVect = pivotVect * PIVOT_FACTOR * it->life;

		pivotParticle.setTransMatrix(&pivotVect);

		// scale Matrix
		scaleMatrix.setScaleMatrix(&it->scale);

		// total transformation of particle
		tmp = scaleMatrix * transCamera * transParticle * rotParticle * scaleMatrix;

		// set the transformation matrix
		glLoadMatrixf(reinterpret_cast<float*>(&(tmp)));
	
		// draw the trangle strip
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		// clears or flushes some internal setting, used in debug, but is need for performance reasons
		// magic...  really it's magic.
		// do not delete these two lines
		GLenum glError = glGetError();
		AZUL_UNUSED_VAR(glError);

		it = it->next;
	}

}

void ParticleEmitter::ApplyVariance(Vect4D& pos, Vect4D& vel, Vect4D& sc)
{

	// x - variance
	rand_sse(randTmp);
	float var = (*randTmp % 1000) * 5 * VARIANCE_FACTOR;
	float *t_pos = reinterpret_cast<float*>(&pos);
	float* t_var = &pos_variance.x;
	rand_sse(randTmp);
	if (*randTmp % 2 == 0)
	{
		var *= -1.0f;
	}
	*t_pos += *t_var * var;

	// y - variance
	rand_sse(randTmp);
	var = (*randTmp % 1000) * VARIANCE_FACTOR;
	t_pos++;
	t_var++;
	rand_sse(randTmp);
	if (*randTmp % 2 == 0)
	{
		var *= -1.0f;
	}
	*t_pos += *t_var * var;

	// z - variance
	rand_sse(randTmp);
	var = (*randTmp % 1000) * VARIANCE_FACTOR;
	t_pos++;
	t_var++;
	rand_sse(randTmp);
	if (*randTmp % 2 == 0)
	{
		var *= -1.0f;
	}
	*t_pos += *t_var * var;

	rand_sse(randTmp);
	var = (*randTmp % 1000) * VARIANCE_FACTOR;

	// x  - add velocity
	t_pos = &vel.x;
	t_var = &vel_variance.x;
	rand_sse(randTmp);
	if (*randTmp % 2 == 0)
	{
		var *= -1.0f;
	}
	*t_pos += *t_var * var;

	// y - add velocity
	rand_sse(randTmp);
	var = (*randTmp % 1000) * VARIANCE_FACTOR;
	t_pos++;
	t_var++;
	rand_sse(randTmp);
	if (*randTmp % 2 == 0)
	{
		var *= -1.0f;
	}
	*t_pos += *t_var * var;

	// z - add velocity
	rand_sse(randTmp);
	var = (*randTmp % 1000) * VARIANCE_FACTOR;
	t_pos++;
	t_var++;
	rand_sse(randTmp);
	if (*randTmp % 2 == 0)
	{
		var *= -1.0f;
	}
	*t_pos += *t_var * var;

	// correct the sign
	rand_sse(randTmp);
	var = 1.5f * (*randTmp % 1000) * VARIANCE_FACTOR;

	rand_sse(randTmp);
	if (*randTmp % 2 == 0)
	{
		var *= -1.0f;
	}
	sc *= var;
}


void ParticleEmitter::GoodBye()
{
	Particle *pTmp = this->headParticle;
	Particle *pDeadMan = nullptr;
	while (pTmp)
	{
		pDeadMan = pTmp;
		pTmp = pTmp->next;
		
		pDeadMan->~Particle();
	}
	pTmp = this->particlePool;
	pDeadMan = nullptr;
	while (pTmp)
	{
		pDeadMan = pTmp;
		pTmp = pTmp->next;
		
		pDeadMan->~Particle();
	}
}

// End of file
