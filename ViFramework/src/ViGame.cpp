#include <stdio.h>

#include <vector>
#include <string>

#include "ViGame.h"

ViGame::ViGame(int aScreenWidth, int aScreenHeight) :
	mWindow(NULL),
	mClearColor(vicolors::BLACK)
{
	new ViEnvironment(aScreenWidth, aScreenHeight);
}

ViGame::~ViGame()
{
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void ViGame::Start()
{
	PrivateInit();

	while (mRunning)
	{
		PrivateUpdate();
		PrivateDraw();
		Events();
	}
}

void ViGame::Quit()
{
	mRunning = false;
}

void ViGame::PrivateUpdate()
{
	viEnv->GetInputManager()->Update();

	Update();

	viEnv->GetInputManager()->LateUpdate();
}

void ViGame::Update()
{
}

void ViGame::Draw()
{
}

void ViGame::PrivateDraw()
{
	Draw();

	viEnv->GetVertexBatch()->Flush();	//manual flush call - must be done at end of frame
	SDL_GL_SwapWindow(mWindow);
}

void ViGame::Events()
{
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			Quit();
		}
	}
}

void ViGame::Init()
{
}

void ViGame::PrivateInit()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! Error: %s\n", SDL_GetError());
	}
	else
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		mWindow = SDL_CreateWindow("Title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, viEnv->GetScreenWidth(), viEnv->GetScreenHeight(), SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		if (mWindow == NULL)
		{
			printf("Window could not be created! %s\n", SDL_GetError());
			Quit();
			return;
		}

		mContext = SDL_GL_CreateContext(mWindow);
		if (mWindow == NULL)
		{
			printf("Gl Context could not be created! %s\n", SDL_GetError());
			Quit();
			return;
		}

		GLenum err = glewInit();
		if (err != GLEW_OK)
		{
			printf("Error initializing glew: %s\n", glewGetErrorString(err));
			Quit();
			return;
		}

		// During init, enable debug output
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(&MessageCallback, 0);

		glClearColor(mClearColor.r, mClearColor.g, mClearColor.b, mClearColor.a);
		glClear(GL_COLOR_BUFFER_BIT);

		mInitialized = true;

		viEnv->GetVertexBatch()->Init(ViVertexBatchSettings(ViVertexBatchSettings::cCULL_NONE, ViVertexBatchSettings::cDEPTH_NONE, ViVertexBatchSettings::cWRAP_LINEAR));

		Init();
	}
}

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
		return;

	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}
