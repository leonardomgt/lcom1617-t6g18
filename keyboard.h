#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include "macros.h"
#include "bitmap.h"
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <minix/sysutil.h>

int keyboard_scan_handlerC(unsigned char *scancode);

int kbd_subscribe_int();

int kbd_unsubscribe_int();

int writeToKeyboard(unsigned char cmd);

unsigned char readFromKeyboard(unsigned char *data);

int readKeyboardUntil(unsigned char* array, unsigned int stringSize, unsigned char endScancode);

char scancodeToChar(unsigned char scancode);

unsigned short lengthOfLetter(char letter);

void printLetterXPM(char letter, unsigned short xi, unsigned short yi, char* base, unsigned char mainColor, unsigned char background);

void printSentence(char* frase, unsigned short xi, unsigned short yi, char* base, unsigned char mainColor, unsigned char background);

int printLetterText(char* base);

char** letterToPrint(char letter);

char** configLetter(char** xpm_tmp, unsigned char mainColor, unsigned char background);

#endif
