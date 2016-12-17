#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <minix/com.h>
#include <minix/sysutil.h>
#include "macros.h"

/** @defgroup video_gr video_gr
 * @{
 *
 * Functions for outputing data to screen in graphics mode
 */

/**
 * @brief Initializes the video module in graphics mode
 * 
 * Uses the VBE INT 0x10 interface to set the desired
 *  graphics mode, maps VRAM to the process' address space and
 *  initializes static global variables with the resolution of the screen, 
 *  and the number of colors
 * 
 * @param mode 16-bit VBE mode to set
 * @return Virtual address VRAM was mapped to. NULL, upon failure.
 */
void *vg_init(unsigned short mode);

 /**
 * @brief Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
 * 
 * @return 0 upon success, non-zero upon failure
 */
int vg_exit(void);


char *read_xpm(char *map[], int *wd, int *ht);
void paintPixel(char* pixelMem, int x, int y, int color);
void printSquare(char* base, unsigned short x, unsigned short y, unsigned short size, unsigned long color);
void printRectangle(char* base, unsigned short x, unsigned short y, unsigned short sizeX, unsigned short sizeY, unsigned long color, unsigned long borderColor);
void paintAllScreen(char* baseMem, int color);
void printXPM(char* base, char* xpm[], unsigned short x, unsigned short y);
void eraseXPM(char* base, char* xpm[], unsigned short x, unsigned short y);
void showPalette(char* base, unsigned short x, unsigned short y);
#endif /* __VIDEO_GR_H */
