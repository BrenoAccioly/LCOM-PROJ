#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

/** @defgroup Game Game
 * @{
 *
 * Functions and structures related to the gameplay.
 */

struct leader{
  uint8_t name[5];
  uint32_t time;
};

/**
 * @brief Start the game loop.
 *
 * @return 0 if no errors were detected
 */
int runGame();

/**
 * @brief Asks the name of the player and inserts it in the leaderboard.txt file.
 * 
 * @param time The time the player took to finish the maze.
 */
void insert_leaderboard(int32_t time);

/**
 * @brief This function displays the name given as argument.
 * 
 * @param name 
 */
void write_name(uint8_t name[]);

/**
 * @brief Displays the player results in the game. The time taken to finish it and the number of keys collected.
 * 
 * @param game_win
 * @param time 
 * @param keys 
 */
void game_result(bool game_win, int32_t time, uint8_t keys);

/**
 * @brief Displays the key counter on the top right corner of the screen
 * 
 * @param keys 
 */
void draw_key_counter(uint8_t keys);

/**
 * @brief Displays the time given on the top right corner of the screen
 * 
 * @param time 
 */
void draw_timer(int32_t time);

#endif // GAME_H_INCLUDED
