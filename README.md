# MC-AStar
MC-AStar makes efficient pathfinding easily achievable for any Minecraft modder. Due to a Java library, that directly connects native C++ code to any Minecraft mod, this library is easy to use while still preserving the speed of the highly optimized C++ code. The connection of the Java library to Minecraft Forge and Fabric will also be simplified in the future to allow a comfortable installation of MC-AStar. Note that this is an early version of this library, so the library is very sparsely documented at this time and most of the use-cases that this library will feature are not implemented yet.

# How does it work?
The library uses the <a href="https://en.wikipedia.org/wiki/A*_search_algorithm">A* search algorithm</a> to efficiently find the fastest path from any position A to any other position B in a Minecraft world. I am currently working on an implementation of the <a href="https://citeseerx.ist.psu.edu/document?repid=rep1&type=pdf&doi=36d1b4ec6a4a2823e9c875318f1952df4abf4876">Path Refinement A*</a> algorithm, which will allow extremely fast checks for pathability between two positions and much faster path calculations while still generating a near-optimal path.

# Performance
Until this point, no exact performance data has been collected yet. However, the efficiency of C++ makes the API extremely fast even in the early versions of the library.

# Compatibility
This library is currently only tested on a 64-bit Intel processor running Windows 10. It should run on AMD processors and other operating systems as well, but you could in theory experience slight performance issues. Please open an issue if you encounter any problems running the library on your system to support the development of this project.

# Where will this project go?
The main goal of this project is to maximize the efficiency of the pathfinding algorithm. To use even more capabilities of most machines, some machine specific optimizations will be made, such as
- (GPU acceleration for faster chunk pre-computing) [this might be unuseful as soon as Path Refinement A* is implemented, we'll see that in the future]
- ASM optimizations for specific processors

Another goal of this project is user comfort. This library will be heavily documented, designed for intuitive use and still provide a wide range of use-cases.
