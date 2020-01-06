#include <lcom/lcf.h>
#include "kbd.h"
#include "maze.h"
#include "video.h"
#include "i8042.h"
#include "i8254.h"
#include "pixelmaps.h"
#include "char.h"
#include "game.h"

static char *maze;
extern uint32_t counter;
extern uint8_t kbd_out_buffer_byte;
extern uint8_t irq_timer0, irq_kbd;

void insert_leaderboard(int32_t time);

int disable_interrupts(){
  if(kbd_unsubscribe_int())
    return 1;

  //if(timer_unsubscribe_int())
    //return 1;
  return 0;
}

int enable_interrupts(){
  if(kbd_subscribe_int(&irq_kbd))
    return 1;

  //if(timer_subscribe_int(&irq_timer0))
    //return 1;
  return 0;
}

/** 
 * \callgraph
 */
void draw_timer(int32_t time){
  switch(time/10){
    case 1:
      vg_draw_pixmap(char_1, XPM_8_8_8, 900, 15);
      break;
    case 2:
      vg_draw_pixmap(char_2, XPM_8_8_8, 900, 15);
      break;
    case 3:
      vg_draw_pixmap(char_3, XPM_8_8_8, 900, 15);
      break;
    case 4:
      vg_draw_pixmap(char_4, XPM_8_8_8, 900, 15);
      break;
    case 5:
      vg_draw_pixmap(char_5, XPM_8_8_8, 900, 15);
      break;
    case 6:
      vg_draw_pixmap(char_6, XPM_8_8_8, 900, 15);
      break;
    case 7:
      vg_draw_pixmap(char_7, XPM_8_8_8, 900, 15);
      break;
    case 8:
      vg_draw_pixmap(char_8, XPM_8_8_8, 900, 15);
      break;
    case 9:
      vg_draw_pixmap(char_9, XPM_8_8_8, 900, 15);
      break;
    default:
      vg_draw_pixmap(char_0, XPM_8_8_8, 900, 15);
      break;
  }
  switch(time%10){
    case 1:
      vg_draw_pixmap(char_1, XPM_8_8_8, 910, 15);
      break;
    case 2:
      vg_draw_pixmap(char_2, XPM_8_8_8, 910, 15);
      break;
    case 3:
      vg_draw_pixmap(char_3, XPM_8_8_8, 910, 15);
      break;
    case 4:
      vg_draw_pixmap(char_4, XPM_8_8_8, 910, 15);
      break;
    case 5:
      vg_draw_pixmap(char_5, XPM_8_8_8, 910, 15);
      break;
    case 6:
      vg_draw_pixmap(char_6, XPM_8_8_8, 910, 15);
      break;
    case 7:
      vg_draw_pixmap(char_7, XPM_8_8_8, 910, 15);
      break;
    case 8:
      vg_draw_pixmap(char_8, XPM_8_8_8, 910, 15);
      break;
    case 9:
      vg_draw_pixmap(char_9, XPM_8_8_8, 910, 15);
      break;
    default:
      vg_draw_pixmap(char_0, XPM_8_8_8, 910, 15);
      break;
  }

}

/** 
 * \callgraph
 */
void draw_key_counter(uint8_t keys){
  vg_draw_pixmap(key_counter, XPM_8_8_8, 945, 0);
  switch(keys){
    case 1:
      vg_draw_pixmap(char_1, XPM_8_8_8, 990, 15);
      break;
    case 2:
      vg_draw_pixmap(char_2, XPM_8_8_8, 990, 15);
      break;
    case 3:
      vg_draw_pixmap(char_3, XPM_8_8_8, 990, 15);
      break;
    case 4:
      vg_draw_pixmap(char_4, XPM_8_8_8, 990, 15);
      break;
    case 5:
      vg_draw_pixmap(char_5, XPM_8_8_8, 990, 15);
      break;
    case 6:
      vg_draw_pixmap(char_6, XPM_8_8_8, 990, 15);
      break;
    case 7:
      vg_draw_pixmap(char_7, XPM_8_8_8, 990, 15);
      break;
    default:
      vg_draw_pixmap(char_0, XPM_8_8_8, 990, 15);
      break;
  }
}

/** 
 * \callgraph
 */
void game_result(bool game_win, int32_t time, uint8_t keys){

  uint8_t size = 1;
  uint8_t scancode[2];
  bool make = 0, two_byte = 0;

  xpm_image_t imageGameResult;
  uint8_t *gameResultSprite = xpm_load(gameResult, XPM_8_8_8, &imageGameResult); 

  xpm_image_t imageGameWin;
  uint8_t *gameWinSprite = xpm_load(gameWin, XPM_8_8_8, &imageGameWin);

  vg_draw_image(imageGameResult, gameResultSprite, XPM_8_8_8, 0, 0);
  
  switch(keys){
    case 1:
      vg_draw_pixmap(charB_1, XPM_8_8_8, 590, 376);
      break;
    case 2:
      vg_draw_pixmap(charB_2, XPM_8_8_8, 590, 376);
      break;
    case 3:
      vg_draw_pixmap(charB_3, XPM_8_8_8, 590, 376);
      break;
    case 4:
      vg_draw_pixmap(charB_4, XPM_8_8_8, 590, 376);
      break;
    case 5:
      vg_draw_pixmap(charB_5, XPM_8_8_8, 590, 376);
      break;
    case 6:
      vg_draw_pixmap(charB_6, XPM_8_8_8, 590, 376);
      break;
    case 7:
      vg_draw_pixmap(charB_7, XPM_8_8_8, 590, 376);
      break;
    default:
      vg_draw_pixmap(charB_0, XPM_8_8_8, 590, 376);
      break;
  }

  switch(time/10){
    case 1:
      vg_draw_pixmap(charB_1, XPM_8_8_8, 590, 456);
      break;
    case 2:
      vg_draw_pixmap(charB_2, XPM_8_8_8, 590, 456);
      break;
    case 3:
      vg_draw_pixmap(charB_3, XPM_8_8_8, 590, 456);
      break;
    case 4:
      vg_draw_pixmap(charB_4, XPM_8_8_8, 590, 456);
      break;
    case 5:
      vg_draw_pixmap(charB_5, XPM_8_8_8, 590, 456);
      break;
    case 6:
      vg_draw_pixmap(charB_6, XPM_8_8_8, 590, 456);
      break;
    case 7:
      vg_draw_pixmap(charB_7, XPM_8_8_8, 590, 456);
      break;
    case 8:
      vg_draw_pixmap(charB_8, XPM_8_8_8, 590, 456);
      break;
    case 9:
      vg_draw_pixmap(charB_9, XPM_8_8_8, 590, 456);
      break;
    default:
      vg_draw_pixmap(charB_0, XPM_8_8_8, 590, 456);
      break;
  }
  switch(time%10){
    case 1:
      vg_draw_pixmap(charB_1, XPM_8_8_8, 620, 456);
      break;
    case 2:
      vg_draw_pixmap(charB_2, XPM_8_8_8, 620, 456);
      break;
    case 3:
      vg_draw_pixmap(charB_3, XPM_8_8_8, 620, 456);
      break;
    case 4:
      vg_draw_pixmap(charB_4, XPM_8_8_8, 620, 456);
      break;
    case 5:
      vg_draw_pixmap(charB_5, XPM_8_8_8, 620, 456);
      break;
    case 6:
      vg_draw_pixmap(charB_6, XPM_8_8_8, 620, 456);
      break;
    case 7:
      vg_draw_pixmap(charB_7, XPM_8_8_8, 620, 456);
      break;
    case 8:
      vg_draw_pixmap(charB_8, XPM_8_8_8, 620, 456);
      break;
    case 9:
      vg_draw_pixmap(charB_9, XPM_8_8_8, 620, 456);
      break;
    default:
      vg_draw_pixmap(charB_0, XPM_8_8_8, 620, 456);
      break;
  }

  if(game_win)
      vg_draw_image(imageGameWin, gameWinSprite, XPM_8_8_8, 300, 650);
  copy_buffer();

  kbd_out_buffer_byte = 0x0;
  while(kbd_out_buffer_byte != BREAK_ESC && kbd_out_buffer_byte != MAKE_ENTER){
      kbc_ih();
      
      kbd_read_scancode(&two_byte, &make, &size, scancode);

      if(two_byte)
          continue;
      
      tickdelay(micros_to_ticks(DELAY_US));
  } 
  if(game_win)
      insert_leaderboard(99-time);

}

/** 
 * \callgraph
 */
void write_name(uint8_t name[]){
  uint32_t previousCharWidth = 0;
  for(int i=0; i < 5; i++){
    switch(name[i])
    {
      case 'a':
        vg_draw_pixmap(charb_a, XPM_8_8_8, 430 + previousCharWidth, 400); previousCharWidth += 30; break;
      case 'b':
        vg_draw_pixmap(charb_b, XPM_8_8_8, 430 + previousCharWidth, 400); previousCharWidth += 23; break;
      case 'c':
        vg_draw_pixmap(charb_c, XPM_8_8_8, 430 + previousCharWidth, 400);previousCharWidth += 27; break;
      case 'd':
        vg_draw_pixmap(charb_d, XPM_8_8_8, 430 + previousCharWidth, 400);previousCharWidth += 29; break;
      case 'e':
        vg_draw_pixmap(charb_e, XPM_8_8_8, 430 + previousCharWidth, 400); previousCharWidth += 23;break;
      case 'f':
        vg_draw_pixmap(charb_f, XPM_8_8_8, 430 + previousCharWidth, 400); previousCharWidth += 21; break;
      case 'g':
        vg_draw_pixmap(charb_g, XPM_8_8_8, 430 + previousCharWidth, 400); previousCharWidth += 29; break;
      case 'h':
        vg_draw_pixmap(charb_h, XPM_8_8_8, 430 + previousCharWidth, 400); previousCharWidth += 32;break;
      case 'i':
        vg_draw_pixmap(charb_i, XPM_8_8_8, 430 + previousCharWidth, 400);previousCharWidth += 13; break;
      case 'j':
        vg_draw_pixmap(charb_j, XPM_8_8_8, 430 + previousCharWidth, 400);previousCharWidth += 13; break;
      case 'k':
        vg_draw_pixmap(charb_k, XPM_8_8_8, 430 + previousCharWidth, 400);previousCharWidth += 29; break;
      case 'l':
        vg_draw_pixmap(charb_l, XPM_8_8_8, 430 + previousCharWidth, 400);previousCharWidth += 22; break;
      case 'm':
        vg_draw_pixmap(charb_m, XPM_8_8_8, 430 + previousCharWidth, 400);previousCharWidth += 37; break;
      case 'n':
        vg_draw_pixmap(charb_n, XPM_8_8_8, 430 + previousCharWidth, 400);previousCharWidth += 31; break;
      case 'o':
        vg_draw_pixmap(charb_o, XPM_8_8_8, 430 + previousCharWidth, 400); previousCharWidth += 30; break;
      case 'p':
        vg_draw_pixmap(charb_p, XPM_8_8_8, 430 + previousCharWidth, 400);previousCharWidth += 23; break;
      case 'q':
        vg_draw_pixmap(charb_q, XPM_8_8_8, 430 + previousCharWidth, 400);previousCharWidth += 30; break;
      case 'r':
        vg_draw_pixmap(charb_r, XPM_8_8_8, 430 + previousCharWidth, 400);previousCharWidth += 27; break;
      case 's':
        vg_draw_pixmap(charb_s, XPM_8_8_8, 430 + previousCharWidth, 400); previousCharWidth += 19;break;
      case 't':
        vg_draw_pixmap(charb_t, XPM_8_8_8, 430 + previousCharWidth, 400);previousCharWidth += 23; break;
      case 'u':
        vg_draw_pixmap(charb_u, XPM_8_8_8, 430 + previousCharWidth, 400);previousCharWidth += 28; break;
      case 'v':
        vg_draw_pixmap(charb_v, XPM_8_8_8, 430 + previousCharWidth, 400); previousCharWidth += 28;break;
      case 'w':
        vg_draw_pixmap(charb_w, XPM_8_8_8, 430 + previousCharWidth, 400);previousCharWidth += 38; break;
      case 'x':
        vg_draw_pixmap(charb_x, XPM_8_8_8, 430 + previousCharWidth, 400);previousCharWidth += 25; break;
      case 'y':
        vg_draw_pixmap(charb_y, XPM_8_8_8, 430 + previousCharWidth, 400);previousCharWidth += 25; break;
      case 'z':
        vg_draw_pixmap(charb_z, XPM_8_8_8, 430 + previousCharWidth, 400);previousCharWidth += 25; break;
      default:
        break;
    }
        
  }
} 

/** 
 * \callgraph
 */
int compare_leaders(const void *p1, const void *p2)
{
  const struct leader *t1 = (struct leader *)p1;
  const struct leader *t2 = (struct leader *)p2;
  return t1->time > t2->time;
}

/** 
 * \callgraph
 */
void insert_leaderboard(int32_t time)
{
  uint8_t size = 1;
  uint8_t scancode[2];
  bool make = 0, two_byte = 0;
  uint8_t pos = 0, c;
  struct leader player;
  struct leader leaderboard[6];
  //uint8_t name[5];
  xpm_image_t leaderImage;
  uint8_t *leaderSprite = xpm_load(leaderboard_name, XPM_8_8_8, &leaderImage);


  FILE *fptr;
  fptr = fopen("/home/lcom/labs/proj/src/leaderboard.txt","r+");


  for(int i = 0; i < 5; i++){
    for(int j = 0; j < 8; j++){
        c = fgetc(fptr);
        if(j < 5)
          leaderboard[i].name[j] = c;    
        else if(j == 6)
        {
          leaderboard[i].time = (c - '0') * 10; 
        }
        else if(j == 7)
        {
          leaderboard[i].time += c - '0'; 
        }
    }
    c = fgetc(fptr); c = fgetc(fptr);
  }
  fclose(fptr);

  kbd_out_buffer_byte = 0x0;

  while(kbd_out_buffer_byte != BREAK_ESC){
      kbc_ih();
      kbd_read_scancode(&two_byte, &make, &size, scancode);

      if(two_byte)
          continue; 
      vg_clean_screen();
      vg_draw_image(leaderImage, leaderSprite, XPM_8_8_8, 0, 0); 
      
      if(pos == 5 && kbd_out_buffer_byte == MAKE_ENTER)
      {
        //escrever no leaderboard
        FILE *fptr;
        fptr = fopen("/home/lcom/labs/proj/src/leaderboard.txt","w");
        player.time = time;
        leaderboard[5] = player;
        qsort(leaderboard, 6, sizeof(struct leader),compare_leaders);
        for(int i = 0; i < 5; i++){
          fprintf(fptr, "%c%c%c%c%c %02d ", leaderboard[i].name[0], leaderboard[i].name[1],leaderboard[i].name[2],leaderboard[i].name[3],leaderboard[i].name[4], leaderboard[i].time);
          if(i < 4)
            fprintf(fptr, "\r");  
        }
        fclose(fptr);
        break;

      }
      else if(pos == 5 && kbd_out_buffer_byte != MAKE_BACKSPACE)
        continue;

      switch(kbd_out_buffer_byte)
        {
          case MAKE_A:
            player.name[pos] = 'a'; pos++; break;
          case MAKE_B:
            player.name[pos] = 'b'; pos++; break;
          case MAKE_C:
            player.name[pos] = 'c'; pos++; break;   
          case MAKE_D:
            player.name[pos] = 'd'; pos++; break;
          case MAKE_E:
            player.name[pos] = 'e'; pos++; break;                      
          case MAKE_F:
            player.name[pos] = 'f'; pos++; break;
          case MAKE_G:
            player.name[pos] = 'g'; pos++; break;            
          case MAKE_H:
            player.name[pos] = 'h'; pos++; break;  
          case MAKE_I:
            player.name[pos] = 'i'; pos++; break;
          case MAKE_J:
            player.name[pos] = 'j'; pos++; break;
          case MAKE_K:
            player.name[pos] = 'k'; pos++; break;
          case MAKE_L:
            player.name[pos] = 'l'; pos++; break;  
          case MAKE_M:
            player.name[pos] = 'm'; pos++; break;  
          case MAKE_N:
            player.name[pos] = 'n'; pos++; break;  
          case MAKE_O:
            player.name[pos] = 'o'; pos++; break;  
          case MAKE_P:
            player.name[pos] = 'p'; pos++; break;  
          case MAKE_Q:
            player.name[pos] = 'q'; pos++; break;  
          case MAKE_R:
            player.name[pos] = 'r'; pos++; break;                                                    
          case MAKE_S:
            player.name[pos] = 's'; pos++; break;
          case MAKE_T:
            player.name[pos] = 't'; pos++; break;
          case MAKE_U:
            player.name[pos] = 'u'; pos++; break;
          case MAKE_V:
            player.name[pos] = 'v'; pos++; break;
          case MAKE_W:
            player.name[pos] = 'w'; pos++; break;
          case MAKE_X:
            player.name[pos] = 'x'; pos++; break;
          case MAKE_Y:
            player.name[pos] = 'y'; pos++; break;
          case MAKE_Z:
            player.name[pos] = 'z'; pos++; break;
          case MAKE_BACKSPACE:
            if(pos > 0) pos--; 
            player.name[pos] = '-'; 
            break;
          default:
            break;
        }
      write_name(player.name);
      copy_buffer();
      tickdelay(micros_to_ticks(DELAY_US));
  }
}

/** 
 * \callgraph
 *
 */
int runGame(){
  int32_t ipc_status;
  uint32_t r;
  message msg;

  uint8_t size = 1;
  uint8_t scancode[2];
  bool make = 0, two_byte = 0;

  int32_t aux_counter=0, max_time = 99;

  uint8_t keys = 0;
  uint8_t spike_pos = 0;
  bool game_win = false;

  uint32_t fr_rate = 30;
  int speed = 20;
  int x_move = -4*80, y_move = 5*80 - 45;
  bool collision = false;

  xpm_image_t imagePlayerFront;
  uint8_t *playerFrontSprite = xpm_load(playerFront, XPM_8_8_8, &imagePlayerFront); 

  xpm_image_t imagePlayerFront1;
  uint8_t *playerFront1Sprite = xpm_load(playerFront1, XPM_8_8_8, &imagePlayerFront1); 

  xpm_image_t imagePlayerFront2;
  uint8_t *playerFront2Sprite = xpm_load(playerFront2, XPM_8_8_8, &imagePlayerFront2); 

  xpm_image_t imagePlayerBack;
  uint8_t *playerBackSprite = xpm_load(playerBack, XPM_8_8_8, &imagePlayerBack);

  xpm_image_t imagePlayerBack1;
  uint8_t *playerBack1Sprite = xpm_load(playerBack1, XPM_8_8_8, &imagePlayerBack1);

  xpm_image_t imagePlayerBack2;
  uint8_t *playerBack2Sprite = xpm_load(playerBack2, XPM_8_8_8, &imagePlayerBack2);

  xpm_image_t imagePlayerLeft;
  uint8_t *playerLeftSprite = xpm_load(playerLeft, XPM_8_8_8, &imagePlayerLeft); 

  xpm_image_t imagePlayerLeft1;
  uint8_t *playerLeft1Sprite = xpm_load(playerLeft1, XPM_8_8_8, &imagePlayerLeft1); 

  xpm_image_t imagePlayerLeft2;
  uint8_t *playerLeft2Sprite = xpm_load(playerLeft2, XPM_8_8_8, &imagePlayerLeft2); 

  xpm_image_t imagePlayerRight;
  uint8_t *playerRightSprite = xpm_load(playerRight, XPM_8_8_8, &imagePlayerRight); 

  xpm_image_t imagePlayerRight1;
  uint8_t *playerRight1Sprite = xpm_load(playerRight1, XPM_8_8_8, &imagePlayerRight1);

  xpm_image_t imagePlayerRight2;
  uint8_t *playerRight2Sprite = xpm_load(playerRight2, XPM_8_8_8, &imagePlayerRight2); 

  uint8_t playerOrientation = MAKE_S;
  uint8_t playerStep = 0;

  y_move -= 80;
  initialize_maze();
  maze = (char*)malloc(21 * 19 * sizeof(char));
  generateMaze(maze, 21, 19);
  addKeys(maze, 21, 19);
  addTraps(maze, 21, 19);
  drawMaze(maze, 21, 19, x_move, y_move, spike_pos); 
  vg_draw_image(imagePlayerFront, playerFrontSprite, XPM_8_8_8, 1024/2 - 15, 768/2 - 23);  
  draw_key_counter(keys);
  copy_buffer();

  uint8_t move = 0;
  while(kbd_out_buffer_byte != BREAK_ESC && max_time >= 0){
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
          printf("driver_receive failed with: %d", r);
          continue;
    }
    if (is_ipc_notify(ipc_status)) { 
          switch (_ENDPOINT_P(msg.m_source)) {
              case HARDWARE: 	
                  if (msg.m_notify.interrupts & irq_timer0){
                      timer_int_handler();
                      if(aux_counter % 30 == 0)
                      {
                          max_time--;
                          if(spike_pos == 2)
                            spike_pos = 0;
                          else
                            spike_pos++;    
                      }
                      if( 60 / fr_rate == counter){
                          counter = 0;
                          
                          vg_clean_screen();
                          drawMaze(maze, 21, 19, x_move, y_move, spike_pos);
                          switch(playerOrientation){
                            case MAKE_W:   
                              if(playerStep == 0)
                                vg_draw_image(imagePlayerBack, playerBackSprite, XPM_8_8_8,1024/2 - 15, 768/2 - 23);
                              
                              else  if(move == 1){
                                  vg_draw_image(imagePlayerBack1, playerBack1Sprite, XPM_8_8_8,1024/2 - 15, 768/2 - 23);
                                    playerStep = 1;
                                    move = 0;
                              }
                              else if(playerStep == 1)
                              {
                                  vg_draw_image(imagePlayerBack2, playerBack2Sprite, XPM_8_8_8,1024/2 - 15, 768/2 - 23);
                                  playerStep = 1;
                                  move = 1;
                              }  
                              break;
                            case MAKE_A:
                              if(playerStep == 0)
                                vg_draw_image(imagePlayerLeft, playerLeftSprite, XPM_8_8_8,1024/2 - 15, 768/2 - 23);
                              
                              else  if(move == 1){
                                  vg_draw_image(imagePlayerLeft1, playerLeft1Sprite, XPM_8_8_8,1024/2 - 15, 768/2 - 23);
                                    playerStep = 1;
                                    move = 0;
                              }
                              else if(playerStep == 1)
                              {
                                  vg_draw_image(imagePlayerLeft2, playerLeft2Sprite, XPM_8_8_8,1024/2 - 15, 768/2 - 23);
                                  playerStep = 1;
                                  move = 1;
                              }     
                              break;
                            case MAKE_S:
                              if(playerStep == 0)
                                vg_draw_image(imagePlayerFront, playerFrontSprite, XPM_8_8_8,1024/2 - 15, 768/2 - 23);
                              
                              else  if(move == 1){
                                  vg_draw_image(imagePlayerFront1, playerFront1Sprite, XPM_8_8_8,1024/2 - 15, 768/2 - 23);
                                    playerStep = 1;
                                    move = 0;
                              }
                              else if(playerStep == 1)
                              {
                                  vg_draw_image(imagePlayerFront2, playerFront2Sprite, XPM_8_8_8,1024/2 - 15, 768/2 - 23);
                                  playerStep = 1;
                                  move = 1;
                              }
                                 
                              break;
                            case MAKE_D:
                              if(playerStep == 0)
                                vg_draw_image(imagePlayerRight, playerRightSprite, XPM_8_8_8,1024/2 - 15, 768/2 - 23);
                              
                              else  if(move == 1){
                                  vg_draw_image(imagePlayerRight1, playerRight1Sprite, XPM_8_8_8,1024/2 - 15, 768/2 - 23);
                                    playerStep = 1;
                                    move = 0;
                              }
                              else if(playerStep == 1)
                              {
                                  vg_draw_image(imagePlayerRight2, playerRight2Sprite, XPM_8_8_8,1024/2 - 15, 768/2 - 23);
                                  playerStep = 1;
                                  move = 1;
                              }
                               
                              break;
                            default:
                              break;
                          }
                         // red_mask(0x88);
                          draw_timer(max_time);
                          draw_key_counter(keys);

                          
                          //disable_interrupts();
                          copy_buffer(); 
                          //enable_interrupts();  
                      }
                    aux_counter++;
                  } 
                  if (msg.m_notify.interrupts & KBD_IRQ) { 
                      kbc_ih();
                      kbd_read_scancode(&two_byte, &make, &size, scancode);
                      if(two_byte)
                          continue; 
                      else{
                        uint32_t aux = 0;
                        /*
                        if(playerStep != 0)
                        {
                          if((playerOrientation | 0x80) != kbd_out_buffer_byte)
                            kbd_out_buffer_byte = playerOrientation;
                        }         
                        */
                        switch(kbd_out_buffer_byte){
                          case MAKE_W:
                            vg_verify_collision(imagePlayerFront, playerFrontSprite, XPM_8_8_8,1024/2 - 15, 768/2 - 23 - speed, &collision);
                            while(collision){
                              aux++;
                              collision = false;
                              vg_verify_collision(imagePlayerFront, playerFrontSprite, XPM_8_8_8,1024/2 - 15, 768/2 - 23  - (speed-aux), &collision);

                            }
                            playerOrientation = MAKE_W;
                            playerStep = 1;
                            y_move += (speed-aux);
                            break;
                          case BREAK_W:
                            playerStep = 0;
                            break;
                          case MAKE_A:
                            vg_verify_collision(imagePlayerFront, playerFrontSprite, XPM_8_8_8,1024/2 - 15 - speed, 768/2 - 23, &collision);
                            while(collision){
                              aux++;
                              collision = false;
                              vg_verify_collision(imagePlayerFront, playerFrontSprite, XPM_8_8_8,1024/2 - 15 - (speed-aux), 768/2 - 23, &collision);
                            }
                            playerOrientation = MAKE_A;
                            playerStep = 1;
                            x_move += (speed-aux);
                            break;
                          case BREAK_A:
                            playerStep = 0;
                            break;
                          case MAKE_S:
                            vg_verify_collision(imagePlayerFront, playerFrontSprite, XPM_8_8_8,1024/2 - 15, 768/2 - 23 + speed, &collision);
                            while(collision){
                              aux++;
                              collision = false;
                              vg_verify_collision(imagePlayerFront, playerFrontSprite, XPM_8_8_8,1024/2 - 15, 768/2 - 23  + (speed-aux), &collision);
                            }
                            playerOrientation = MAKE_S;
                            playerStep = 1;
                            y_move -= (speed-aux);
                            break;
                          case BREAK_S:
                            playerStep = 0;
                            break;
                          case MAKE_D:
                            vg_verify_collision(imagePlayerFront, playerFrontSprite, XPM_8_8_8,1024/2 - 15 + speed, 768/2 - 23, &collision);
                        
                            while(collision){
                              aux++;
                              collision = false;
                              vg_verify_collision(imagePlayerFront, playerFrontSprite, XPM_8_8_8,1024/2 - 15 + (speed-aux), 768/2 - 23, &collision);
                            }
                            playerOrientation = MAKE_D;
                            playerStep = 1;
                            x_move -= (speed-aux);
                            break;
                          case BREAK_D:
                            playerStep = 0;
                            break;
                          default:
                            break;
                        }
                    
                      }
					        }  
                  break;
              default:
                  break;
          }
      } 
      else {
    } 
    if(maze[-(x_move-1024/2 - 15) / 80 + (- (y_move-768/2 - 23) / 80) * 21] == 3){
      maze[-(x_move-1024/2 - 15) / 80 + (- (y_move-768/2 - 23) / 80) * 21] = 0;
      keys++;
    }

    if(keys == 7)
    {
      game_win = true;
      break;
    }

    if(vg_verify_death(imagePlayerFront, playerFrontSprite, XPM_8_8_8,1024/2 - 15, 768/2 - 23) == 0)
      break;

  }

  game_result(game_win, max_time, keys);

  return 0;
}
