#ifndef MAZE_H_INCLUDED
#define MAZE_H_INCLUDED

void initialize_maze();

void drawMaze(const char *maze, int width, int height, int x_move, int y_move, uint8_t spike_pos);

void carveMaze(char *maze, int width, int height, int x, int y);

void generateMaze(char *maze, int width, int height);

void addKeys(char *maze, int width, int height);

void addTraps(char *maze, int width, int height);

#endif // MAZE_H_INCLUDED
