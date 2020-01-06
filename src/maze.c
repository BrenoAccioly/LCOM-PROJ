#include <lcom/lcf.h>
#include "video.h"
#include "pixelmaps.h"
#include "mainMenu.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

xpm_image_t imageDoor;
uint8_t *doorSprite;

xpm_image_t imageWall;
uint8_t *wallSprite;

xpm_image_t imageSideWall;
uint8_t *sideWallSprite;

xpm_image_t imageFloor;
uint8_t *floorSprite;

xpm_image_t imageKey;
uint8_t *keySprite;

xpm_image_t imageSpike1;
uint8_t *spike1Sprite;

xpm_image_t imageSpike2;
uint8_t *spike2Sprite;

xpm_image_t imageSpike3;
uint8_t *spike3Sprite;

/** 
 * \callgraph
 */
void initialize_maze(){
  doorSprite = xpm_load(door, XPM_8_8_8, &imageDoor);
  wallSprite = xpm_load(wall, XPM_8_8_8, &imageWall);
  sideWallSprite = xpm_load(side_wall, XPM_8_8_8, &imageSideWall);
  floorSprite = xpm_load(floor, XPM_8_8_8, &imageFloor);
  keySprite = xpm_load(key, XPM_8_8_8, &imageKey);
  spike1Sprite = xpm_load(spike1, XPM_8_8_8, &imageSpike1);
  spike2Sprite = xpm_load(spike2, XPM_8_8_8, &imageSpike2);
  spike3Sprite = xpm_load(spike3, XPM_8_8_8, &imageSpike3);
}

/** 
 * \callgraph
 */
void drawMaze(const char *maze, int width, int height, int x_move, int y_move, uint8_t spike_pos) {
   int x, y;
   for(y = 0; y < height; y++) {
      for(x = 0; x < width; x++) {
         switch(maze[y * width + x]) {
         case 1:  
            if(maze[y * width + x + width] == 1)
               vg_draw_image(imageWall, wallSprite, XPM_8_8_8, x*80 + x_move, y*80 + y_move);  
            else
            {
               vg_draw_image(imageSideWall, sideWallSprite, XPM_8_8_8, x*80 + x_move, y*80 + y_move);  
            }
            break;
         case 2:
            vg_draw_image(imageDoor, doorSprite, XPM_8_8_8, x*80 + x_move, y*80 + y_move); 
            break;
         case 3: 
            vg_draw_image(imageFloor, floorSprite, XPM_8_8_8, x*80 + x_move, y*80 + y_move);
            vg_draw_image(imageKey, keySprite, XPM_8_8_8, x*80 + x_move, y*80 + y_move); 
            break;
         case 4:
            if(spike_pos == 0)
               vg_draw_image(imageSpike1, spike1Sprite, XPM_8_8_8, x*80 + x_move, y*80 + y_move);
            else if(spike_pos == 1)
               vg_draw_image(imageSpike2, spike2Sprite, XPM_8_8_8, x*80 + x_move, y*80 + y_move);
            else
               vg_draw_image(imageSpike3, spike3Sprite, XPM_8_8_8, x*80 + x_move, y*80 + y_move); 
            break;
         default: 
            vg_draw_image(imageFloor, floorSprite, XPM_8_8_8, x*80 + x_move, y*80 + y_move);
            break;
         }
      }
      
   }

}

/** 
 * \callgraph
 */
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
      } 
      else 
      {
         dir = (dir + 1) % 4;
         count += 1;
      }
   }

}

/** 
 * \callgraph
 */
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

   maze[width/2] = 2; // door
}

/** 
 * \callgraph
 */
void addKeys(uint8_t *maze, uint32_t width, uint32_t height){
    uint32_t blankCont = 0;
    uint32_t blankPos[width*height];
    for(uint32_t i = 0; i < width*height; i++){
        if(maze[i] == 0 && i > width && i != width/2 + width)
            {
                blankPos[blankCont] = i;
                blankCont++;
            }
    }
    for(uint8_t i = 0; i < 7; i++){
        maze[blankPos[rand() % blankCont]] = 3;
    }
}

/** 
 * \callgraph
 */
void addTraps(uint8_t *maze, uint32_t width, uint32_t height){
   uint32_t blankCont = 0;
   uint32_t blankPos[width*height];
    for(uint32_t i = 0; i < width*height; i++){
        if(maze[i] == 0 && i > width &&  i != width/2 + width)
            {
                blankPos[blankCont] = i;
                blankCont++;
            }

    }
    for(uint8_t i = 0; i < 10; i++){
        maze[blankPos[rand() % blankCont]] = 4;
    }
}
