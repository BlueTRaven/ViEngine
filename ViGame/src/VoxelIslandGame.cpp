#include "VoxelIslandGame.h"

#include <sstream>

#include "WorldGenerator.h"
#include "ViGameAssetHolderProgramFactory.h"

vigame::VoxelIslandGame::VoxelIslandGame() : ViGame(640 * 2, 480 * 2),
	transform(ViTransform::Positioned({ 0.0, 0.0, -1.0 }))
{
	ENVIRONMENT->GetAssetHandler()->InitialParse("./Assets/assets.vif");

	ViAssetHolderProgram::SetFactory(new ViGameAssetHolderProgramFactory());
}

void vigame::VoxelIslandGame::Init()
{
	SDL_GL_SetSwapInterval(0);	//turn vsync off
	SDL_ShowCursor(SDL_DISABLE);

	int width = 0, height = 0;
	SDL_GetWindowSize(GetWindow(), &width, &height);

	SDL_WarpMouseInWindow(GetWindow(), width / 2, height / 2);

	mDebugFont = new ViMaterialFont(GET_ASSET_FONT("debug"), GET_ASSET_PROGRAM("text"));

	mProgramText = static_cast<ViProgramText*>(GET_ASSET_PROGRAM("text"));

	//grid size 0.05f
	//int worldSize = 64;
	//int worldHeight = 128;

	vec3i worldSize = vec3i(32, 64, 32);
	vec3i viewDistanceChunks = vec3i(3, 3, 3);
	float gridSize = 1;
	float fogStart = 64;
	float fogEnd = 128;

	ViVifParser parser("./Assets/variables.vif");

	if (parser.GetValid())
	{
		parser.FindVec3i("world_size", worldSize);
		
		parser.FindFloat("world_cube_scale", gridSize);

		vec3i chunkSize = vec3i(32);
		parser.FindVec3i("world_chunk_size", chunkSize);
		Chunk::SetSize(chunkSize);

		string meshingMethodName = "naive";
		parser.FindString("world_chunk_meshing_method", meshingMethodName);

		Chunk::MeshingMethod method = Chunk::MeshingMethod::cNAIVE;
		if (meshingMethodName == "stupid")
			method = Chunk::MeshingMethod::cSTUPID;
		else if (meshingMethodName == "naive")
			method = Chunk::MeshingMethod::cNAIVE;
		else if (meshingMethodName == "greedy")
			method = Chunk::MeshingMethod::cGREEDY;

		Chunk::SetMeshingMethod(method);

		parser.FindVec3i("view_distance_chunks", viewDistanceChunks);

		parser.FindFloat("fog_start", fogStart);
		parser.FindFloat("fog_end", fogEnd);
	}

	vec3 startPos = vec3(worldSize / 2);

	world = new VoxelWorld(worldSize, gridSize, new WorldGenerator, fogStart, fogEnd);
	world->SetViewDistanceChunks(viewDistanceChunks);
	world->SetLoadPosition(startPos);
	world->Init();

	ViGame::Init();
}

void vigame::VoxelIslandGame::Update(double aDeltaTime)
{
	int width = 0, height = 0;

	SDL_GetWindowSize(GetWindow(), &width, &height);
	
	if (INPUT_MANAGER->KeyHeld(SDL_SCANCODE_ESCAPE))
		Quit();

	if (INPUT_MANAGER->KeyDown(SDL_SCANCODE_T))
		world->SetDrawDebug(!world->GetDrawDebug());

	if (INPUT_MANAGER->KeyDown(SDL_SCANCODE_P))
		mPaused = !mPaused;

	if (ENVIRONMENT->GetFocused() && (!mPaused || (mPaused && INPUT_MANAGER->KeyDown(SDL_SCANCODE_O))))
	{
		world->Update(aDeltaTime);
	}

	ViGame::Update(aDeltaTime);
	
	if (SDL_GetWindowFlags(GetWindow()) & SDL_WINDOW_INPUT_FOCUS)
		SDL_WarpMouseInWindow(GetWindow(), width / 2, height / 2);
}

void vigame::VoxelIslandGame::Draw(double aDeltaTime)
{
	glViewport(0, 0, ENVIRONMENT->GetScreenWidth(), ENVIRONMENT->GetScreenHeight());

	ViColorGL color = GetClearColor();
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	VERTEX_BATCH->SetSettings(ViVertexBatchSettings(ViVertexBatchSettings::cCULL_CW, ViVertexBatchSettings::cDEPTH_LESS,
		ViVertexBatchSettings::cCLAMP_POINT, ViVertexBatchSettings::cBLEND_NONPREMULTIPLIED, ViVertexBatchSettings::cDRAW_FILLED));
	world->Draw(aDeltaTime, VERTEX_BATCH);

	VERTEX_BATCH->SetSettings(ViVertexBatchSettings(ViVertexBatchSettings::cCULL_CW, ViVertexBatchSettings::cDEPTH_NONE, 
		ViVertexBatchSettings::cCLAMP_POINT, ViVertexBatchSettings::cBLEND_ALPHABLEND, ViVertexBatchSettings::cDRAW_FILLED));
	VERTEX_BATCH->DrawString(ViTransform::Positioned(vec3(0, 0, 0)), mDebugFont, "Avg. FPS: " + std::to_string(GetAvgFPS()));
	VERTEX_BATCH->DrawString(ViTransform::Positioned(vec3(0, mDebugFont->GetFont()->GetSize() + 8, 0)), mDebugFont, "FPS: " + std::to_string(GetFPS()));
	
	std::stringstream str;
	str << "Loading Chunks Queue: " << world->GetChunkManager()->GetChunksToLoadCount() << std::endl;

	VERTEX_BATCH->DrawString(ViTransform::Positioned(vec3(0, mDebugFont->GetFont()->GetSize() * 2 + 8, 0)), mDebugFont, str.str());

	ViGame::Draw(aDeltaTime);
}
