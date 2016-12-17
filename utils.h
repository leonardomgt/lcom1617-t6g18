#include "keyboard.h"
#include "video_gr.h"
#include <stdio.h>
#include <time.h>

void initLog();
void log(const char* key, const char* value);
void logVInt(const char* key, int var);

int wait_for_key(unsigned char keyScancode);


/**
 * @brief Waits for first player pressing his key
 *
 * @return 0 player 1 (bar space key), 1 player 2 (enter key)
 */
int wait_for_player_turn();

/**
 * @brief Waits for player answer and animates a decreasing bar
 *
 *
 * @param base mem address of 1st frame buffer byte
 * @param xi x position of the upper left pixel of the bar
 * @param yi y position of the upper left pixel of the bar
 * @param delta bar width
 * @param time time to answer the question
 * @param height bar height
 * @param answer
 * @param time spent
 * @return 0 ok 1 not ok
 */
int wait_for_player_answer(char* base, unsigned short xi, unsigned short yi, short delta, unsigned short time, unsigned short height, unsigned char* answer, unsigned short* timeSpent);






int generate_question();


char** configLetter(char** xpm, unsigned char mainColor);

void resizeLetter(char** xpm, char** xpm_tmp, int size);

char* itoa(int i, char b[]);














