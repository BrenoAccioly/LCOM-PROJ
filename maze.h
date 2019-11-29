#ifndef MAZE_H_INCLUDED
#define MAZE_H_INCLUDED

void drawMaze(const char *maze, int width, int height);

void carveMaze(char *maze, int width, int height, int x, int y);

void generateMaze(char *maze, int width, int height);

void addKeys(char *maze, int width, int height);

#endif // MAZE_H_INCLUDED
