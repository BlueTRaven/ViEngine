#pragma once

#include <GL/glew.h>
#include <GL/GLU.h>
#include <GL/GL.h>

#include <SDL.h>
#include <SDL_opengl.h>

#include "ViEnvironment.h"
#include "ViColor.h"
#include "ViTimer.h"

class VIENGINE_EXPORT ViGame
{
public:
	ViGame(int aScreenWidth, int aScreenHeight);
	~ViGame();

	//Starts the game loop.
	void Start();

	//Quits the game loop.
	virtual void Quit();

protected:
	virtual void Update();
	virtual void Draw();
	virtual void Events();

	virtual void Init();

	vi_property_get_named(ViColorGL, mClearColor, ClearColor);
	vi_property_set_named(ViColorGL, mClearColor, ClearColor);

	vi_property_get_named(SDL_Window*, mWindow, Window);

protected:
	float GetAvgFPS();
	float GetFPS();

private:
	void PrivateUpdate();
	void PrivateDraw();
	void PrivateInit();

	uint32_t mCounter;	//number of frames since the start
	ViTimer* mTimer;	//timer since the start of the game

	uint32_t mFpsCounter;
	ViTimer* mFpsTimer;

	float mFps;

	SDL_Window* mWindow;
	SDL_GLContext mContext;

	ViColorGL mClearColor;

	bool mRunning = true;
	bool mInitialized = false;
};

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);