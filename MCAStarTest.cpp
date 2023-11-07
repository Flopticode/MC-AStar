#include <iostream>
#include "PathFindingWorld.h"
#include "AStar.h"
#include "PathFindingBlockStateUtils.h"
#include "NodePrioQueue.h"
#include <Windows.h>
#include <chrono>
#include "time.h"
#include "WorldRenderer.h"
#include "DebugDefs.h"

#define NUM_RUNS 10
#define RUN_LENGTH 1

#define CHUNK_HEIGHT 16
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
                data[index] = PFBSUtils::createState(true, true, rand() % 10 + 1, false);
            }
        }
    }
    return data;
}
int main()
{
    srand((uint32)GetTickCount64());

    std::list<uint64> millisPerIteration = std::list<uint64>();
    uint32 iterations = 0;


    std::string a;
    std::cin >> a;
    std::cout << "Starting pathfinding." << std::endl;

    for (uint8 runID = 0; runID < NUM_RUNS; runID++)
    {
        auto world = new PathFindingWorld();

#ifdef DEBUG_RENDERING
        auto renderer = new WorldRenderer(world);
#endif

        auto worldLen = 20;
        for (int i = 0; i < worldLen; i++)
        {
            auto chunk = new PathFindingChunk(
                BlockPos(0, 0, i * CHUNK_DEPTH),
                randomData());
            world->addChunk(ChunkPos(0, 0, i), chunk);
        }

        std::chrono::milliseconds duration;
        uint32 times = 0;
        auto start = BlockPos(0, 0, 0);
        auto end = BlockPos(0, 0, worldLen * CHUNK_DEPTH - 1);

        Path* lastPath = nullptr;
        auto startmillis = GetTickCount64();
        auto ts1 = std::chrono::high_resolution_clock::now();
        auto ts2 = std::chrono::high_resolution_clock::now();
        
        uint32 closedNodes;
        uint32 openNodes;
        do
        {
            auto path = AStar::calculatePath(closedNodes, openNodes,
#ifdef DEBUG_RENDERING
                renderer,
#endif
#ifndef DEBUG_RENDERING
                nullptr,
#endif
                world, start, end);
            delete lastPath;
            lastPath = path;
            times++;
        } while (GetTickCount64() - startmillis < RUN_LENGTH);
        ts2 = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(ts2 - ts1);

        millisPerIteration.push_back(duration.count());

        std::cout << "Evaluated " << closedNodes
            << " nodes. " << openNodes << " were still open." << std::endl;

        std::cout << "Path finding completed in " << (duration.count() / times) << " time units." << std::endl;
        std::cout << "Operations in total: " << times << std::endl;
        if (lastPath == nullptr)
            std::cout << "No path was found." << std::endl;
        else
            std::cout << "A path of size " << lastPath->get().size() << " was found." << std::endl;
        iterations += times;

        delete world;
    }

    std::cout << std::endl << "==========" << std::endl << std::endl;
    std::cout << "Test finished. Performance: " << std::endl;
    time_t total = 0;
    for (auto& millis : millisPerIteration)
    {
        total += millis;
        std::cout << millis << " ";
    }
    std::cout << std::endl << "AVG: " << (total / iterations) << std::endl << std::endl;

    return 0;
}
