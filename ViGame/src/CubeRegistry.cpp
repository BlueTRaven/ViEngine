#include "CubeRegistry.h"

vigame::CubeRegistry::CubeRegistry() :
	mCachedCube(nullptr),
	mCachedCubeid(-1)
{
}

void vigame::CubeRegistry::AddCubeType(Cube* aCube)
{
	cubeid id = (cubeid)mCubeTypes.size();
	aCube->SetId(id);
	mCubeTypes.push_back(aCube);
	mIdCubeMappings.emplace(aCube, id);
}

vigame::cubeid vigame::CubeRegistry::GetCubeId(Cube* aCube)
{
	if (mCachedCubeid == -1 || mCachedCube == nullptr || aCube != mCachedCube)
	{
		mCachedCubeid = mIdCubeMappings[aCube];
		mCachedCube = aCube;
	}

	return mCachedCubeid;
}

vigame::Cube* vigame::CubeRegistry::GetCubeType(cubeid aId)
{
	if (mCachedCubeid == -1 || mCachedCube == nullptr || aId != mCachedCubeid)
	{
		mCachedCube = mCubeTypes[aId];
		mCachedCubeid = aId;
	}

	return mCachedCube;
}
