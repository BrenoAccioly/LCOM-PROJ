#include <lcom/lcf.h>
#include "video.h"
#include "pixelmaps.h"
#include "mainMenu.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void drawMaze(const char *maze, int width, int height) {

   int x, y;
   for(y = 0; y < height; y++) {
      for(x = 0; x < width; x++) {
         switch(maze[y * width + x]) {
         case 1:  
            if(maze[y * width + x + width] == 1)
               vg_draw_pixmap(wall, XPM_8_8_8, x*80, y*80);  
            else
            {
               vg_draw_pixmap(side_wall, XPM_8_8_8, x*80, y*80);
            }
            break;
         //case 1:  vg_draw_rectangle(x*10, y*10, 10, 10, 0xFFFF);  break;
         case 3:  vg_draw_rectangle(x*80, y*80, 20, 20, 0xAFF);  break;
         default: vg_draw_rectangle(x*80, y*80, 80, 80, 0);  break;
         }
      }
      
   }

   for(x = 0; x < width; x++){
      //vg_draw_rectangle(x*10, height*10, 10, 10, 0xFFFF);
   }

   //if(vg_draw_pixmap(wall, XPM_8_8_8, 0, 400) != 0)
      //return; 

}

void carveMaze(char *maze, int width, int height, int x, int y) {

   int x1, y1;
   int x2, y2;
   int dx, dy;
   int dir, count;

   dir = rand() % 4;
   count = 0;
   while(count < 4) {
      dx = 0; dy = 0;
      switch(dir) {
      case 0:  dx = 1;  break;
      case 1:  dy = 1;  break;
      case 2:  dx = -1; break;
      default: dy = -1; break;
      }
      x1 = x + dx;
      y1 = y + dy;
      x2 = x1 + dx;
      y2 = y1 + dy;
      if( x2 > 0 && x2 < width && y2 > 0 && y2 < height
         && maze[y1 * width + x1] == 1 && maze[y2 * width + x2] == 1) {
         maze[y1 * width + x1] = 0;
         maze[y2 * width + x2] = 0;
         x = x2; y = y2;
         dir = rand() % 4;
         count = 0;
      } else {
         dir = (dir + 1) % 4;
         count += 1;
      }
   }

}

void generateMaze(char *maze, int width, int height) {

   int x, y;

   for(x = 0; x < width * height; x++) {
      maze[x] = 1;
   }
   maze[1 * width + width/2 ] = 0;

   srand(time(0));

   for(y = 1; y < height; y += 2) {
      for(x = 1; x < width; x += 2) {
         carveMaze(maze, width, height, x, y);
      }
   }

   maze[width/2] = 0;
}

void addKeys(uint8_t *maze, uint32_t width, uint32_t height){
    uint32_t blankCont = 0;
    uint32_t blankPos[width*height];
    for(uint32_t i = 0; i < width*height; i++){
        if(maze[i] == 0 && i > width)
            {
                blankPos[blankCont] = i;
                blankCont++;
            }

    }
    for(uint8_t i = 0; i < 7; i++){
        maze[blankPos[rand() % blankCont]] = 3;
    }
}
