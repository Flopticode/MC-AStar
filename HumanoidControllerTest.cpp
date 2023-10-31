#include <iostream>
#include "PathFindingWorld.h"
#include "AStar.h"
#include "PathFindingBlockStateUtils.h"
#include "PrioQueue.h"
#include <Windows.h>
#include <chrono>
#include "time.h"

PathFindingBlockState* randomData()
{
    auto data = new PathFindingBlockState[16 * 16 * 16];
    for (uint8 x = 0; x < 16; x++)
        for (uint8 y = 0; y < 16; y++)
            for (uint8 z = 0; z < 16; z++)
                data[z * 16 * 16 + y * 16 + x] = PFBSUtils::createState(true, true, rand()%10 + 1, false);
    return data;
}
int main()
{
    srand(0);

    auto world = new PathFindingWorld();
    auto worldLen = 10;
    for (int i = 0; i < worldLen; i++)
    {
        auto chunk = new PathFindingChunk(
            BlockPos(0, 0, i*16),
            BlockPos(15, 15, (i + 1) * 16 - 1),
            randomData());
        world->addChunk(ChunkPos(0, 0, i), chunk);
    }
    std::chrono::milliseconds duration;
    uint32 times = 0;
    auto start = BlockPos(0, 0, 0);
    auto end = BlockPos(0, 0, worldLen * 16 - 1);

    
    int a;
    std::cin >> a;
    std::cout << "Starting pathfinding." << std::endl;
    Path* lastPath = nullptr;
    auto startmillis = GetTickCount64();
    auto ts1 = std::chrono::high_resolution_clock::now();
    auto ts2 = std::chrono::high_resolution_clock::now();
    do
    {
        auto path = AStar::calculatePath(world, start, end);
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
