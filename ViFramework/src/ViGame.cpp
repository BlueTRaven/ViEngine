#include <stdio.h>

#include <vector>
#include <string>

#include "ViGame.h"

ViGame::ViGame(int aScreenWidth, int aScreenHeight) :
	mWindow(NULL),
	mCounter(0),
	mTimer(new ViTimer()),
	mFpsTimer(new ViTimer()),
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

	mTimer->Start();
	mFpsTimer->Start();

	while (mRunning)
	{
		if ((SDL_GetWindowFlags(GetWindow()) & SDL_WINDOW_INPUT_FOCUS) == SDL_WINDOW_INPUT_FOCUS)
		{
			if (mTimer->GetState() == ViTimer::cTIMER_PAUSE)
				mTimer->Start();	//restart

			ENVIRONMENT->SetFocused(true);
		}
		else 
		{
			if (mTimer->GetState() == ViTimer::cTIMER_START)
				mTimer->Pause();

			ENVIRONMENT->SetFocused(false);
		}

		double time = mTimer->GetMs();
		mTimer->Update();
		double deltaTime = (mTimer->GetMs() - time) / 1000.0f;
		//if unpausing deltaTime can be negative... don't want that.
		deltaTime = glm::max<double>(0, deltaTime);

		mFpsTimer->Update();

		PrivateUpdate(deltaTime);
		PrivateDraw(deltaTime);
		Events();

		mCounter++;
		mFpsCounter++;

		if (mFpsTimer->GetSec() > 1)
		{
			mFps = (float)(mFpsCounter / mFpsTimer->GetSec());
			mFpsTimer->Start();
			mFpsCounter = 0;
		}
	}
}

void ViGame::Quit()
{
	mRunning = false;
}

float ViGame::GetAvgFPS()
{
	return (float)(mCounter / mTimer->GetSec());
}

float ViGame::GetFPS()
{
	return mFps;
}

void ViGame::PrivateUpdate(double aDeltaTime)
{
	ENVIRONMENT->GetInputManager()->Update();

	Update(aDeltaTime);

	ENVIRONMENT->GetInputManager()->LateUpdate();
}

void ViGame::Update(double aDeltaTime)
{
}

void ViGame::Draw(double aDeltaTime)
{
}

void ViGame::PrivateDraw(double aDeltaTime)
{
	Draw(aDeltaTime);

	ENVIRONMENT->GetVertexBatch()->Flush();	//manual flush call - must be done at end of frame
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

		mWindow = SDL_CreateWindow("Title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ENVIRONMENT->GetScreenWidth(), ENVIRONMENT->GetScreenHeight(), SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		if (mWindow == NULL)
		{
			printf("Window could not be created! %s\n", SDL_GetError());
			Quit();
			return;
		}

		ENVIRONMENT->SetWindow(mWindow);

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

		ENVIRONMENT->GetVertexBatch()->Init(ViVertexBatchSettings::Default);

		Init();
	}
}

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
		return;

	string sourceText;

	switch (source)
	{
	case (GL_DEBUG_SOURCE_API):				//Calls to the OpenGL API
		sourceText = "API";
		break;
	case (GL_DEBUG_SOURCE_WINDOW_SYSTEM):	//Calls to a window-system API
		sourceText = "Window System";
		break;
	case (GL_DEBUG_SOURCE_SHADER_COMPILER):	//A compiler for a shading language
		sourceText = "Shader Compiler";
		break;
	case (GL_DEBUG_SOURCE_THIRD_PARTY):		//An application associated with OpenGL
		sourceText = "Third Party";
		break;
	case (GL_DEBUG_SOURCE_APPLICATION):		//Generated by the user of this application
		sourceText = "Application";
		break;
	case (GL_DEBUG_SOURCE_OTHER):
	default:
		sourceText = "Other";
		break;
	}

	string typeText;

	switch (type)
	{
	case (GL_DEBUG_TYPE_ERROR):					//An error, typically from the API
		typeText = "Error";
		break;
	case (GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR):	//Some behavior marked deprecated has been used
		typeText = "Deprecated Behavior";
		break;
	case (GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR):	//Something has invoked undefined behavior
		typeText = "Undefined Beahavior";
		break;
	case (GL_DEBUG_TYPE_PORTABILITY):			//Some functionality the user relies upon is not portable
		typeText = "Portability";
		break;
	case (GL_DEBUG_TYPE_PERFORMANCE):			//Code has triggered possible performance issues
		typeText = "Performance";
		break;
	case (GL_DEBUG_TYPE_MARKER):				//Command stream annotation
		typeText = "Marker";
		break;
	case (GL_DEBUG_TYPE_PUSH_GROUP):			//Group pushing
		typeText = "Push Group";
		break;
	case (GL_DEBUG_TYPE_POP_GROUP):				//Group Popping
		typeText = "Pop Group";
		break;
	case (GL_DEBUG_TYPE_OTHER):
	default:
		typeText = "Other";
		break;
	}

	string severityText;

	switch (severity)
	{
	case (GL_DEBUG_SEVERITY_HIGH):			//All OpenGL Errors, shader compilation/linking errors, or highly-dangerous undefined behavior
		severityText = "High";
		break;
	case (GL_DEBUG_SEVERITY_MEDIUM):		//Major performance warnings, shader compilation/linking warnings, or the use of deprecated functionality
		severityText = "Medium";
		break;
	case (GL_DEBUG_SEVERITY_LOW):			//Redundant state change performance warning, or unimportant undefined behavior
		severityText = "Low";
		break;
	case (GL_DEBUG_SEVERITY_NOTIFICATION):	//Anything that isn't an error or performance issue.
	default:
		severityText = "Notification";
		break;
	}

	auto output = stdout;

	if (severity == GL_DEBUG_SEVERITY_HIGH || type == GL_DEBUG_TYPE_ERROR)
		output = stderr;

	fprintf(output, "GL CALLBACK: source = %s, type = %s, severity = %s, message = %s\n",
		sourceText.c_str(), typeText.c_str(), severityText.c_str(), message);
}
