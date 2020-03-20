#pragma once

#include <GL/glew.h>
#include <GL/GLU.h>
#include <GL/GL.h>

#include <SDL.h>
#include <SDL_opengl.h>

#include "ViEnvironment.h"

#include "ProgramGeneric.h"
#include "ProgramText.h"
#include "ViAssetHandler.h"
#include "ViVertexBatch.h"
#include "ViColor.h"
#include "ViColorsGL.h"
#include "ViTransform.h"
#include "ViFont.h"

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

private:
	void PrivateUpdate();
	void PrivateDraw();
	void PrivateInit();

	SDL_Window* mWindow;
	SDL_GLContext mContext;

	ViColorGL mClearColor;

	bool mRunning = true;
	bool mInitialized = false;
};

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);