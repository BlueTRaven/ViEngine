#include "VoxelIslandGame.h"

vigame::VoxelIslandGame::VoxelIslandGame() : ViGame(640 * 2, 480 * 2),
	transform(ViTransform::Positioned({ 0.0, 0.0, -1.0 }))
{
	transform.SetScale({ 0.5, 0.5, 0.5 });
	viEnv->GetAssetHandler()->InitialParse("./Assets/assets.vif");
}

void vigame::VoxelIslandGame::Init()
{
	SDL_ShowCursor(SDL_DISABLE);

	testFont = GET_ASSET_FONT("debug");
	textMaterial = GET_ASSET_MATERIAL("font_debug");

	mProgramText = static_cast<ViProgramText*>(GET_ASSET_PROGRAM("text"));
	mProgramGeneric = static_cast<ViProgramGeneric*>(GET_ASSET_PROGRAM("generic"));

	world = new VoxelWorld({ 128, 128, 1 }, 0.05f);

	for (int x = 0; x < 16; x++)
	{
		for (int z = 0; z < 16; z++)
		{
			world->SetCubeInstance({ x, 0, z }, world->GetCube(1));
		}
	}

	ViGame::Init();
}

void vigame::VoxelIslandGame::Update()
{
	int width = 0, height = 0;

	SDL_GetWindowSize(GetWindow(), &width, &height);

	float moveMult = -0.01f;
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

	rotation.y = (float)((int)rotation.y % 360);

	transform.SetRotation(rotation);

	mProgramGeneric->SetCamera(transform.Matrix());

	ViGame::Update();
	
	if (SDL_GetWindowFlags(GetWindow()) & SDL_WINDOW_INPUT_FOCUS)
		SDL_WarpMouseInWindow(GetWindow(), width / 2, height / 2);
}

void vigame::VoxelIslandGame::Draw()
{
	glViewport(0, 0, viEnv->GetScreenWidth(), viEnv->GetScreenHeight());

	ViColorGL color = GetClearColor();
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	VERTEX_BATCH->SetSettings(ViVertexBatchSettings(ViVertexBatchSettings::cCULL_CW, ViVertexBatchSettings::cDEPTH_LESS, ViVertexBatchSettings::cCLAMP_POINT, ViVertexBatchSettings::cBLEND_ALPHABLEND));
	world->Draw(VERTEX_BATCH);

	VERTEX_BATCH->SetSettings(ViVertexBatchSettings(ViVertexBatchSettings::cCULL_CW, ViVertexBatchSettings::cDEPTH_NONE, ViVertexBatchSettings::cCLAMP_POINT, ViVertexBatchSettings::cBLEND_ALPHABLEND));
	float fps = GetFPS();
	VERTEX_BATCH->DrawString(textMaterial, ViTransform::None, testFont, std::to_string(fps));

	ViGame::Draw();
}
