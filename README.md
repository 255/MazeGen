MazeGen
=======
This is a silly little program that randomly produces rectangular mazes of arbitrary size. What could be more fun than solving a 1000 x 1000 maze?

I wrote program because I needed to take a break from writing a boring paper for school. The code is not necessarily any good. It produces a minimum spanning tree from a randomly weighted grid graph using Prim's algorithm.

To compile this with g++, use something like the following:
```
g++ -O2 -std=c++11 *.cpp -omazegen
```
