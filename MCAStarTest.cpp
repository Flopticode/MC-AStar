#include <iostream>
#include "PathFindingWorld.h"
#include "AStar.h"
#include "PathFindingBlockStateUtils.h"
#include "NodePrioQueue.h"
#include <Windows.h>
#include <chrono>
#include "time.h"
#include "WorldRenderer.h"

#define CHUNK_HEIGHT 1
#define CHUNK_WIDTH 16
#define CHUNK_DEPTH 16

PathFindingBlockState* randomData()
{
    auto data = new PathFindingBlockState[CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH];
    for (uint8 x = 0; x < CHUNK_WIDTH; x++)
    {
        for (uint8 y = 0; y < CHUNK_HEIGHT; y++)
        {
            for (uint8 z = 0; z < CHUNK_DEPTH; z++)
            {
                uint32 index =
                    z * CHUNK_WIDTH * CHUNK_HEIGHT
                    + y * CHUNK_WIDTH
                    + x;
                data[index] = PFBSUtils::createState(true, true, rand()%10 + 1, false);
            }
        }
    }
    return data;
}
int main()
{
    srand(0);

    auto world = new PathFindingWorld();
    auto worldLen = 3;
    for (int i = 0; i < worldLen; i++)
    {
        auto chunk = new PathFindingChunk(
            BlockPos(0, 0, i*CHUNK_DEPTH),
            BlockPos(
                CHUNK_WIDTH - 1,
                CHUNK_HEIGHT - 1,
                ((i + 1) * CHUNK_DEPTH) - 1),
            randomData());
        world->addChunk(ChunkPos(0, 0, i), chunk);
    }
    WorldRenderer* renderer = new WorldRenderer(world);

    std::chrono::milliseconds duration;
    uint32 times = 0;
    auto start = BlockPos(0, 0, 0);
    auto end = BlockPos(0, 0, worldLen * CHUNK_DEPTH - 1);

    
    std::string a;
    std::cin >> a;
    std::cout << "Starting pathfinding." << std::endl;
    Path* lastPath = nullptr;
    auto startmillis = GetTickCount64();
    auto ts1 = std::chrono::high_resolution_clock::now();
    auto ts2 = std::chrono::high_resolution_clock::now();
    do
    {
        auto path = AStar::calculatePath(renderer, world, start, end);
        delete lastPath;
        lastPath = path;
        times++;
    } while (GetTickCount64() - startmillis < 10000);
    ts2 = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(ts2 - ts1);

    std::cout << "Path finding completed in " << (duration.count() / times) << " time units." << std::endl;
    std::cout << "Operations in total: " << times << std::endl;
    if (lastPath == nullptr)
        std::cout << "No path was found." << std::endl;
    else
        std::cout << "A path of size " << lastPath->get().size() << " was found." << std::endl;
}
