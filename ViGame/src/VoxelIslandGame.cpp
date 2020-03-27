#include "VoxelIslandGame.h"

#include "WorldGenerator.h"

vigame::VoxelIslandGame::VoxelIslandGame() : ViGame(640 * 2, 480 * 2),
	transform(ViTransform::Positioned({ 0.0, 0.0, -1.0 }))
{
	transform.SetScale({ 0.5, 0.5, 0.5 });
	viEnv->GetAssetHandler()->InitialParse("./Assets/assets.vif");
}

void vigame::VoxelIslandGame::Init()
{
	SDL_GL_SetSwapInterval(0);	//turn vsync off
	SDL_ShowCursor(SDL_DISABLE);

	int width = 0, height = 0;
	SDL_GetWindowSize(GetWindow(), &width, &height);

	SDL_WarpMouseInWindow(GetWindow(), width / 2, height / 2);

	testFont = GET_ASSET_FONT("debug");
	textMaterial = GET_ASSET_MATERIAL("font_debug");

	mProgramText = static_cast<ViProgramText*>(GET_ASSET_PROGRAM("text"));
	mProgramGeneric = static_cast<ViProgramGeneric*>(GET_ASSET_PROGRAM("generic"));

	mProgramGeneric->SetAmbientStrength(0.1f);

	int worldSize = 256;
	world = new VoxelWorld({ worldSize, 64, worldSize }, 0.05f, new WorldGenerator);

	ViGame::Init();
}

void vigame::VoxelIslandGame::Update(double aDeltaTime)
{
	int width = 0, height = 0;

	SDL_GetWindowSize(GetWindow(), &width, &height);

	float moveMult = -1 * aDeltaTime;
	float mouseSens = -100;

	if (INPUT_MANAGER->KeyHeld(SDL_SCANCODE_ESCAPE))
		Quit();

	if (INPUT_MANAGER->KeyHeld(SDL_SCANCODE_W))
		transform.Translate(transform.Forward() * moveMult);
	if (INPUT_MANAGER->KeyHeld(SDL_SCANCODE_A))
		transform.Translate(transform.Left() * moveMult);
	if (INPUT_MANAGER->KeyHeld(SDL_SCANCODE_S))
		transform.Translate(transform.Backwards() * moveMult);
	if (INPUT_MANAGER->KeyHeld(SDL_SCANCODE_D))
		transform.Translate(transform.Right() * moveMult);

	if (INPUT_MANAGER->KeyHeld(SDL_SCANCODE_X))
		transform.Translate(transform.Down() * moveMult);
	if (INPUT_MANAGER->KeyHeld(SDL_SCANCODE_Z))
		transform.Translate(transform.Up() * moveMult);

	//TODO correct camera rotation
	//Right now it's going to go a little faster on the y axis since it's not properly normalized
	vec2 mDelta = INPUT_MANAGER->GetDistanceFromCenter(GetWindow());
	vec2 mNormalized = mDelta / (vec2(width, height) / vec2(2));
	vec3 rotation = transform.GetRotation();
	rotation = rotation + vec3(mNormalized.y * mouseSens, mNormalized.x * mouseSens, 0.0);

	if (rotation.x > 85)
		rotation.x = 85;
	if (rotation.x < -85)
		rotation.x = -85;

	rotation.y = std::fmod(rotation.y, 360.0f);

	transform.SetRotation(rotation);

	mProgramGeneric->SetCamera(transform.Matrix());

	ViGame::Update(aDeltaTime);
	
	if (SDL_GetWindowFlags(GetWindow()) & SDL_WINDOW_INPUT_FOCUS)
		SDL_WarpMouseInWindow(GetWindow(), width / 2, height / 2);
}

void vigame::VoxelIslandGame::Draw(double aDeltaTime)
{
	glViewport(0, 0, viEnv->GetScreenWidth(), viEnv->GetScreenHeight());

	ViColorGL color = GetClearColor();
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	VERTEX_BATCH->SetSettings(ViVertexBatchSettings(ViVertexBatchSettings::cCULL_CW, ViVertexBatchSettings::cDEPTH_LESS, ViVertexBatchSettings::cCLAMP_POINT, ViVertexBatchSettings::cBLEND_NONPREMULTIPLIED));
	//VERTEX_BATCH->Draw(ViTransform::Positioned(vec3(0, 0, 0)), mTestChunk->GetOptimizedMesh());
	world->Draw(VERTEX_BATCH);

	VERTEX_BATCH->SetSettings(ViVertexBatchSettings(ViVertexBatchSettings::cCULL_CW, ViVertexBatchSettings::cDEPTH_NONE, ViVertexBatchSettings::cCLAMP_POINT, ViVertexBatchSettings::cBLEND_ALPHABLEND));
	VERTEX_BATCH->DrawString(ViTransform::Positioned(vec3(0, 0, 0)), textMaterial, testFont, "Avg. FPS: " + std::to_string(GetAvgFPS()));
	VERTEX_BATCH->DrawString(ViTransform::Positioned(vec3(0, testFont->GetSize() + 8, 0)), textMaterial, testFont, "FPS: " + std::to_string(GetFPS()));

	ViGame::Draw(aDeltaTime);
}
