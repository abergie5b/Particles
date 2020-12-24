#include <conio.h>
#include "EventHandler.h"
#include "OpenGLInterface.h"

#include "ParticleEmitter.h"


// Timer needed in Particle Emission
PerformanceTimer GlobalTimer;

// -----------------------------------------------------------
// main:                
// -----------------------------------------------------------
int main()
{
	Trace::out("System Start()\n");

	bool success = false;
	srand(1);

	// initialize timers:------------------------------

		// Create a timer objects
		PerformanceTimer UpdateTimer;
		PerformanceTimer DrawTimer;

	// create a window:---------------------------------

		success = OpenGLDevice::InitWindow();
		assert(success);

	// create an emitter:-------------------------------

		ParticleEmitter emitter;

	// Initialize Camera Matrices :-------------------

		Matrix tmp;

		Matrix cameraMatrix;
		cameraMatrix.setIdentMatrix();

		Vect4D trans(0.0f, 10.0f, 15.0f);

		Matrix transMatrix;
		transMatrix.setTransMatrix(&trans);

		Matrix inverseCameraMatrix;

		tmp = cameraMatrix * transMatrix;
		tmp.Inverse(inverseCameraMatrix);

		// counter for printing
		int i = 0;

	// main update loop... do this forever or until some breaks 
	bool exit = false;
	while (OpenGLDevice::IsRunning() && !exit )
	{
		// break if escape key is hit
		if (GetAsyncKeyState(VK_ESCAPE))
		{
			exit = true;
			break;
		}

		// start update timer ---------------------------------------
		UpdateTimer.Tic();

			// start draw... end draw (the draw updates)
			OpenGLDevice::StartDraw();

			// set matrix to Model View
			// push the inverseCameraMarix to stack
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(reinterpret_cast<float*>(&inverseCameraMatrix));
			glPushMatrix(); // push the camera matrix

			// update the emitter
			emitter.update();

		// stop update timer: -----------------------------------------
		UpdateTimer.Toc();

		// start draw timer: ----------------------------------------
		DrawTimer.Tic();

			// draw particles
			emitter.draw();

			// pop matrix - needs to correspond to previous push
			glPopMatrix();

		// stop draw timer: -----------------------------------------
		DrawTimer.Toc();

		// finish draw update
		OpenGLDevice::EndDraw();

		// Love for Windows - allows the windows to behave to external events
		EventHandler::ProcessEvents();

		// update ouput every 50 times
		i++;
		if (i > 15)
		{
			i = 0;
			float updateTime = UpdateTimer.TimeInSeconds();
			float drawTime = DrawTimer.TimeInSeconds();
			printf("NumParticles: %d LoopTime: update:%f ms  draw:%f ms  tot:%f\n", emitter.last_active_particle, updateTime * 1000.0f, drawTime * 1000.0f, (updateTime + drawTime) *1000.0f);
		}
	}

	// Cleanup
	emitter.GoodBye();

	Trace::out("System End()\n");

	return 0;
}

// ---------------------------------------------------------------------
// WinMain required by windows for all win32 applications.
// This is our Windows entry point.
// ---------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	OpenGLDevice::SetHInstance(hInstance);
	//main(__argc, __argv);
	main();
}

// End of file
