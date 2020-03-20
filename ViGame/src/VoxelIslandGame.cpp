#include "VoxelIslandGame.h"

vigame::VoxelIslandGame::VoxelIslandGame() : ViGame(640, 480),
	transform(ViTransform::Positioned({ 0.0, 0.0, -1.0 }))
{
	transform.SetScale({ 0.5, 0.5, 0.5 });
	viEnv->GetAssetHandler()->InitialParse("./Assets/assets.vif");
}

void vigame::VoxelIslandGame::Init()
{
	mProgramGeneric = new ViProgramGeneric();
	mProgramText = new ViProgramText();

	testFont = ViFont::Load("./Assets/Fonts/arial.ttf");
	textMaterial = new ViMaterial(mProgramText, testFont->GetTexture());

	genericMaterial = new ViMaterial(mProgramGeneric, viEnv->GetAssetHandler()->LoadTexture("dirt_01"));

	ViGame::Init();
}

void vigame::VoxelIslandGame::Update()
{
	float moveMult = -0.01f;

	if (viEnv->GetInputManager()->KeyHeld(SDL_SCANCODE_ESCAPE))
		Quit();

	if (viEnv->GetInputManager()->KeyHeld(SDL_SCANCODE_Q))
		transform.Rotate(glm::vec3(0, 1.0, 0));
	if (viEnv->GetInputManager()->KeyHeld(SDL_SCANCODE_E))
		transform.Rotate(glm::vec3(0, -1.0, 0));

	if (viEnv->GetInputManager()->KeyHeld(SDL_SCANCODE_W))
		transform.Translate(transform.Forward() * moveMult);
	if (viEnv->GetInputManager()->KeyHeld(SDL_SCANCODE_A))
		transform.Translate(transform.Left()* moveMult);
	if (viEnv->GetInputManager()->KeyHeld(SDL_SCANCODE_S))
		transform.Translate(transform.Backwards()* moveMult);
	if (viEnv->GetInputManager()->KeyHeld(SDL_SCANCODE_D))
		transform.Translate(transform.Right()* moveMult);

	mProgramGeneric->SetCamera(transform.Matrix());

	ViGame::Update();
}

void vigame::VoxelIslandGame::Draw()
{
	glViewport(0, 0, viEnv->GetScreenWidth(), viEnv->GetScreenHeight());

	ViColorGL color = GetClearColor();
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viEnv->GetVertexBatch()->SetSettings(ViVertexBatchSettings(ViVertexBatchSettings::cCULL_CW, ViVertexBatchSettings::cDEPTH_LESS, ViVertexBatchSettings::cCLAMP_POINT));
	viEnv->GetVertexBatch()->DrawQuad(genericMaterial, ViTransform::None, { -0.5, 0.5, 0 }, { 0.5, 0.5, 0 }, { 0.5, -0.5, 0 }, { -0.5, -0.5, 0 });

	viEnv->GetVertexBatch()->DrawQuad(genericMaterial, ViTransform::Positioned({ -0.25, 0.0, -1.0 }), { -0.5, 0.5, 0 }, { 0.5, 0.5, 0 }, { 0.5, -0.5, 0 }, { -0.5, -0.5, 0 });

	viEnv->GetVertexBatch()->DrawQuad(genericMaterial, ViTransform::Positioned({ 0.25, 0.0, -2.0 }), { -0.5, 0.5, 0 }, { 0.5, 0.5, 0 }, { 0.5, -0.5, 0 }, { -0.5, -0.5, 0 });

	viEnv->GetVertexBatch()->DrawQuad(genericMaterial, ViTransform::Positioned({ 0.0, 0.0, -3.0 }), { -0.5, 0.5, 0 }, { 0.5, 0.5, 0 }, { 0.5, -0.5, 0 }, { -0.5, -0.5, 0 });

	viEnv->GetVertexBatch()->SetSettings(ViVertexBatchSettings(ViVertexBatchSettings::cCULL_CW, ViVertexBatchSettings::cDEPTH_NONE, ViVertexBatchSettings::cCLAMP_POINT));
	viEnv->GetVertexBatch()->DrawString(textMaterial, ViTransform::None, testFont, "This is a test.");

	ViGame::Draw();
}
