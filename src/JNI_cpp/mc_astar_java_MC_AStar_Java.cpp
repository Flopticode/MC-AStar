#include "../pch.h"
#include "mc_astar_java_MC_AStar_Java.h"
#include "../MCAStar.h"
#include <iostream>
#include "../MinecraftTypes.h"

JNIEXPORT jintArray JNICALL Java_mc_1astar_1java_MC_1AStar_1Java_findPath
(JNIEnv* env, jobject obj, jint sX, jint sY, jint sZ, jint eX, jint eY, jint eZ)
{
	Path* path;
	auto err = calculatePath(path, BlockPos(sX, sY, sZ), BlockPos(eX, eY, eZ));

	if (err != MC_ASTAR_NO_ERROR)
	{
		std::cout << "Pathfinding error " << err << std::endl;
		return nullptr;
	}

	if (path == nullptr)
		return nullptr;

	auto pathList = path->get();
	size_t len = pathList.size() * 3;
	jintArray arr = env->NewIntArray((jsize)len);
	
	long* longArr = new long[len];
	auto iter = pathList.begin();
	
	for(uint32 i = 0; iter != pathList.end(); i += 3)
	{
		BlockPos pos = (*iter)->getPos();
		longArr[i] = pos.x;
		longArr[i + 1] = pos.y;
		longArr[i + 2] = pos.z;
	}

	env->SetIntArrayRegion(arr, (jsize)0, (jsize)len, longArr);
	delete[] longArr;
	return arr;
}

jobject getErrorObject(JNIEnv* env, uint32 code)
{
	std::string fieldName;
	switch (code)
	{
	case MCAStarError::MC_ASTAR_NO_ERROR:
	{
		fieldName = "MC_ASTAR_NO_ERROR";
	} break;
	case MCAStarError::MC_ASTAR_WORLD_INST_FAILED:
	{
		fieldName = "MC_ASTAR_WORLD_INST_FAILED";
	} break;
	case MCAStarError::MC_ASTAR_NULLPTR:
	{
		fieldName = "MC_ASTAR_NULLPTR";
	} break;
	case MCAStarError::MC_ASTAR_INVALID_POSITION:
	{
		fieldName = "MC_ASTAR_INVALID_POSITION";
	} break;
	case MCAStarError::MC_ASTAR_ALREADY_INITIALIZED:
	{
		fieldName = "MC_ASTAR_ALREADY_INITIALIZED";
	} break;
	case MCAStarError::MC_ASTAR_NOT_INITIALIZED:
	{
		fieldName = "MC_ASTAR_NOT_INITIALIZED";
	} break;
	case MCAStarError::MC_ASTAR_ALLOCATION_FAILED:
	{
		fieldName = "MC_ASTAR_ALLOCATION_FAILED";
	} break;
	default:
		return nullptr;
	}

	jclass errorEnum = env->FindClass("mc_astar_java/MCAStarError");
	if (errorEnum == nullptr)
		return nullptr;

	jfieldID fieldID = env->GetStaticFieldID(errorEnum, fieldName.c_str(), "Lmc_astar_java/MCAStarError;");
	if (fieldID == nullptr)
		return nullptr;
	
	jobject obj = env->GetStaticObjectField(errorEnum, fieldID);
	return obj;
}

JNIEXPORT jobject JNICALL Java_mc_1astar_1java_MC_1AStar_1Java_init
(JNIEnv* env, jobject obj)
{
	auto ret = init();
	return getErrorObject(env, ret);
}

JNIEXPORT jobject JNICALL Java_mc_1astar_1java_MC_1AStar_1Java_addChunk
(JNIEnv* env, jobject obj, jint x, jint y, jint z, jlongArray jniData)
{
	if (jniData == nullptr)
		return getErrorObject(env, MCAStarError::MC_ASTAR_NULLPTR);

	auto chunkDataLL = new jlong[16 * 16 * 16];
	if (chunkDataLL == nullptr)
		return getErrorObject(env, MCAStarError::MC_ASTAR_ALLOCATION_FAILED);
	env->GetLongArrayRegion(jniData, 0, 16 * 16 * 16, (jlong*)chunkDataLL);

	auto chunkData = new PathFindingBlockState[16 * 16 * 16];
	if (chunkData == nullptr)
		return getErrorObject(env, MCAStarError::MC_ASTAR_ALLOCATION_FAILED);

	for(uint8 cdx = 0; cdx < 16; cdx++)
		for(uint8 cdy = 0; cdy < 16; cdy++)
			for (uint8 cdz = 0; cdz < 16; cdz++)
			{
				auto index = cdz * 16 * 16 + cdy * 16 + cdx;
				chunkData[index] = (PathFindingBlockState)chunkDataLL[index];
			}

	delete[] chunkDataLL;

	return getErrorObject(env, addChunk(ChunkPos(x, y, z), chunkData));
}

JNIEXPORT jobject JNICALL Java_mc_1astar_1java_MC_1AStar_1Java_setBlock
(JNIEnv* env, jobject obj, jint x, jint y, jint z, jlong data)
{
	return getErrorObject(env, setBlock(BlockPos(x, y, z),
		(PathFindingBlockState)data));
}
