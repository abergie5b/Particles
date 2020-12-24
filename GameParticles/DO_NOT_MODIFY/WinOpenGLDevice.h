#ifdef WIN32

#include "WindowsWrapper.h"						// Header File For Windows
#include <gl\gl.h>								// Header File For The OpenGL32 Library
#include <gl\glu.h>								// Header File For The GLu32 Library

struct sWH
{
	int width;
	int height;
};

class OpenGLDevice
{
	
public:
	~OpenGLDevice();
	
	static void StartDraw();
	static void EndDraw();
	static bool InitWindow();
	static bool KillWindow();
	static bool IsRunning();
	static void Quit();
	static void SetHInstance(HINSTANCE hInst);
	static void GetApplicationDir(char* dest, int size);
	
private:
	OpenGLDevice();
	static OpenGLDevice& Instance();
	
	bool initWindow();
	bool killWindow();
	void endDraw();
	bool isRunning();
	void quit();
	void setHInstance(HINSTANCE hInst);
	void getApplicationDir(char* dest, int size);
	
	HDC			hDC;
	HGLRC		hRC;
	HWND		hWnd;
	HINSTANCE	hInstance;
	WNDCLASS	_wc;
	DWORD		_dwExStyle;
	DWORD		_dwStyle;
	sWH			screenSize;
	int			depth;
	int			pixelsize;
	bool		fullscreen;
	bool		running;
	char		pad0;
	char		pad1;
};

#endif // WIN32

// End of File
