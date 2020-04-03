#include "VoxelIslandGame.h"

#include <sstream>

#include "WorldGenerator.h"
#include "ViGameAssetHolderProgramFactory.h"

vigame::VoxelIslandGame::VoxelIslandGame() : ViGame(640 * 2, 480 * 2),
	transform(ViTransform::Positioned({ 0.0, 0.0, -1.0 })),
	highlightedChunk(nullptr)
{
	//transform.SetScale({ 0.5, 0.5, 0.5 });
	viEnv->GetAssetHandler()->InitialParse("./Assets/assets.vif");

	ViAssetHolderProgram::SetFactory(new ViGameAssetHolderProgramFactory());
}

void vigame::VoxelIslandGame::Init()
{
	SDL_GL_SetSwapInterval(0);	//turn vsync off
	SDL_ShowCursor(SDL_DISABLE);

	int width = 0, height = 0;
	SDL_GetWindowSize(GetWindow(), &width, &height);

	SDL_WarpMouseInWindow(GetWindow(), width / 2, height / 2);

	mDebugFont = new ViMaterialFont(GET_ASSET_FONT("debug"), GET_ASSET_MATERIAL("font_debug"));

	mProgramText = static_cast<ViProgramText*>(GET_ASSET_PROGRAM("text"));

	mProgramCubesInstanced = static_cast<ProgramCubesInstanced*>(GET_ASSET_PROGRAM("cube_instanced"));

	//grid size 0.05f
	int worldSize = 64;
	int worldHeight = 512;
	world = new VoxelWorld({ worldSize, worldHeight, worldSize }, 1, new WorldGenerator);

	mCubeMesh = ViMesh::MakeUCube(ASSET_HANDLER->LoadMaterial("white_pixel_fullbright"), vec3(-0.5), vec3(0.5), ViMesh::cFACE_ALL, vicolors::WHITE);
	mCubeMesh->UploadData();

	ViGame::Init();

	mCamera = new Camera(ViTransform::None);
	player = new Player(ViTransform::Positioned(-vec3(worldSize / 2, worldHeight, worldSize / 2)), world, mCamera);
}

void vigame::VoxelIslandGame::Update(double aDeltaTime)
{
	int width = 0, height = 0;

	SDL_GetWindowSize(GetWindow(), &width, &height);
	
	if (INPUT_MANAGER->KeyHeld(SDL_SCANCODE_ESCAPE))
		Quit();

	if (INPUT_MANAGER->KeyDown(SDL_SCANCODE_T))
		world->SetDrawDebug(!world->GetDrawDebug());

	world->Update(aDeltaTime);

	player->Update(aDeltaTime);
	mCamera->Update(aDeltaTime);

	ViGame::Update(aDeltaTime);

	mCamera->LateUpdate(aDeltaTime);
	
	if (SDL_GetWindowFlags(GetWindow()) & SDL_WINDOW_INPUT_FOCUS)
		SDL_WarpMouseInWindow(GetWindow(), width / 2, height / 2);
}

void vigame::VoxelIslandGame::Draw(double aDeltaTime)
{
	glViewport(0, 0, viEnv->GetScreenWidth(), viEnv->GetScreenHeight());

	ViColorGL color = GetClearColor();
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	VERTEX_BATCH->SetSettings(ViVertexBatchSettings(ViVertexBatchSettings::cCULL_CW, ViVertexBatchSettings::cDEPTH_LESS,
		ViVertexBatchSettings::cCLAMP_POINT, ViVertexBatchSettings::cBLEND_NONPREMULTIPLIED, ViVertexBatchSettings::cDRAW_FILLED));
	world->Draw(VERTEX_BATCH);
	player->Draw(VERTEX_BATCH);

	VERTEX_BATCH->SetSettings(ViVertexBatchSettings(ViVertexBatchSettings::cCULL_CW, ViVertexBatchSettings::cDEPTH_NONE, 
		ViVertexBatchSettings::cCLAMP_POINT, ViVertexBatchSettings::cBLEND_ALPHABLEND, ViVertexBatchSettings::cDRAW_FILLED));
	VERTEX_BATCH->DrawString(ViTransform::Positioned(vec3(0, 0, 0)), mDebugFont, "Avg. FPS: " + std::to_string(GetAvgFPS()));
	VERTEX_BATCH->DrawString(ViTransform::Positioned(vec3(0, mDebugFont->GetFont()->GetSize() + 8, 0)), mDebugFont, "FPS: " + std::to_string(GetFPS()));
	
	vec3 pos = mCamera->GetTransform().GetPosition();
	vec3 rot = mCamera->GetTransform().GetRotation();
	std::stringstream str;
	str << std::fixed << std::setprecision(2) << "Camera Position: x " << pos.x << ", y " << pos.y << ", z " << pos.z <<
		" Rotation: x " << rot.x << ", y " << rot.y << ", z " << rot.z;

	VERTEX_BATCH->DrawString(ViTransform::Positioned(vec3(0, mDebugFont->GetFont()->GetSize() * 2 + 8, 0)), mDebugFont, str.str());

	pos = player->GetTransform().GetPosition();
	rot = player->GetTransform().GetRotation();
	str = std::stringstream();
	str << std::fixed << std::setprecision(2) << "Player Position: x " << pos.x << ", y " << pos.y << ", z " << pos.z <<
		" Rotation: x " << rot.x << ", y " << rot.y << ", z " << rot.z;

	VERTEX_BATCH->DrawString(ViTransform::Positioned(vec3(0, mDebugFont->GetFont()->GetSize() * 3 + 8, 0)), mDebugFont, str.str());

	ViGame::Draw(aDeltaTime);
}
