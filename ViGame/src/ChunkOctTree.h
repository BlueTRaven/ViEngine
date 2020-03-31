#pragma once

#include "ViUtil.h"

#include "ViGameUtil.h"

namespace vigame
{
	struct ChunkNode;

	//An oct tree specifically for querying cubes in chunks.
	class ChunkOctTree
	{
		ChunkNode* mRootNode;

		//The minimum unit size
		//Queries are in cube space; therefore we can't go lower than 1
		const int cMIN_SIZE = 1;
	};

	struct ChunkNode
	{
		void Set(vec3i aPosition, cubeid aId);
		ChunkNode* Query(vec3i aPosition);

		cubeid id;

		int depth;

		ChunkNode* parent;
		ChunkNode* nodes[8];
		uint8_t activeNodes;
	};
}