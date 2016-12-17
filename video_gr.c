#include "video_gr.h"

//#include "vbe.h"

/* Constants for VBE 0x105 mode */

/* The physical address may vary from VM to VM.
 * At one time it was 0xD0000000
 *  #define VRAM_PHYS_ADDR    0xD0000000 
 * Currently on lab B107 is 0xF0000000
 * Better run my version of lab5 as follows:
 *     service run `pwd`/lab5 -args "mode 0x105"
 */
#define VRAM_PHYS_ADDR	0xE0000000
#define H_RES           1024
#define V_RES		  	768
#define BITS_PER_PIXEL	  8

/* Private global variables */

static char *video_mem;		/* Process address to which VRAM is mapped */

static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */


int vg_exit() {
  struct reg86u reg86;

  reg86.u.b.intno = 0x10; /* BIOS video services */

  reg86.u.b.ah = 0x00;    /* Set Video Mode function */
  reg86.u.b.al = 0x03;    /* 80x25 text mode*/

  if( sys_int86(&reg86) != OK ) {
      printf("\tvg_exit(): sys_int86() failed \n");
      return 1;
  } else
	  printf("Back to Text Mode\n");
      return 0;
}

void* vg_init(unsigned short mode) {

	struct reg86u reg86;

	reg86.u.b.intno = 0x10;
	reg86.u.w.ax = VBE_MODE_SET;
	reg86.u.w.bx = LINEAR_MODE|mode;

	if( sys_int86(&reg86) != OK ) {
		printf("vg_init(): sys_int86() failed \n");
	}


	int r;
	struct mem_range mr;
	unsigned int vram_base = VRAM_PHYS_ADDR;  /* VRAM’s physical addresss */
	unsigned int vram_size = H_RES*V_RES;  /* VRAM’s size, but you can use
	                                the frame-buffer size, instead */
	void *video_mem;         /* frame-buffer VM address */
	    /* Allow memory mapping */
	mr.mr_base = (phys_bytes) vram_base;
	mr.mr_limit = mr.mr_base + vram_size;
	if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
	       panic("sys_privctl (ADD_MEM) failed: %d\n", r);
	    /* Map memory */
	video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
	if(video_mem == MAP_FAILED)
	    panic("couldn’t map video memory");
	return video_mem;
}

char *read_xpm(char *map[], int *wd, int *ht)
{
  int width, height, colors;
  char sym[256];
  int  col;
  int i, j;
  char *pix, *pixtmp, *tmp, *line;
  char symbol;

  /* read width, height, colors */
  if (sscanf(map[0],"%d %d %d", &width, &height, &colors) != 3) {
    printf("read_xpm: incorrect width, height, colors\n");
    return NULL;
  }
#ifdef DEBUG
  printf("%d %d %d\n", width, height, colors);
#endif
  if (width > HRES || height > VRES || colors > 256) {
    printf("read_xpm: incorrect width, height, colors\n");
    return NULL;
  }

  *wd = width;
  *ht = height;

  for (i=0; i<256; i++)
    sym[i] = 0;

  /* read symbols <-> colors */
  for (i=0; i<colors; i++) {
    if (sscanf(map[i+1], "%c %d", &symbol, &col) != 2) {
      printf("read_xpm: incorrect symbol, color at line %d\n", i+1);
      return NULL;
    }
#ifdef DEBUG
    printf("%c %d\n", symbol, col);
#endif
    if (col > 256) {
      printf("read_xpm: incorrect color at line %d\n", i+1);
      return NULL;
    }
    sym[col] = symbol;
  }

  /* allocate pixmap memory */
  pix = pixtmp = malloc(width*height);

  /* parse each pixmap symbol line */
  for (i=0; i<height; i++) {
    line = map[colors+1+i];
#ifdef DEBUG
    printf("\nparsing %s\n", line);
#endif
    for (j=0; j<width; j++) {
      tmp = memchr(sym, line[j], 256);  /* find color of each symbol */
      if (tmp == NULL) {
    	  printf("read_xpm: incorrect symbol at line %d, col %d\n", colors+i+1, j);
    	  return NULL;
      }
      *pixtmp++ = tmp - sym; /* pointer arithmetic! back to books :-) */
#ifdef DEBUG
      printf("%c:%d ", line[j], tmp-sym);
#endif
    }
  }

  return pix;
}

void paintPixel(char* pixelMem, int x, int y, int color) {

	if(color != TRANSPARENT){
		if(x<1024 && y<768){
			*(pixelMem + (y * HRES) + x) = color;
		}
	}



}

void paintAllScreen(char* baseMem, int color) {

	printSquare(baseMem, 0,0,1024, color);

}

void printSquare(char* base, unsigned short x, unsigned short y, unsigned short size, unsigned long color) {

	int xMax = (x + size) > HRES ? HRES : (x + size);
	int yMax = (y + size) > VRES ? VRES : (y + size);

	while(y < yMax)  {
		while(x < xMax) {
			paintPixel(base, x, y, color);
			//printf("currentX: %d, currentY: %d\n", x, y);
			x++;
		}
		x -= size;
		y++;

	}
}

void printXPM(char* base, char* xpm[], unsigned short x, unsigned short y) {

	int width;
	int height;
	char* bitmap;

	bitmap = read_xpm(xpm, &width, &height);
	int i,j;
	for(i = 0; i < height; i++) {
		for(j = 0; j < width; j++) {
			paintPixel(base, x + j, y + i, bitmap[(i * width) + j]);
			//printf("x: %d, y: %d cor: %d\n", x+j, y+i, bitmap[(i * width) + j]);

		}
	}
}

void eraseXPM(char* base, char* xpm[], unsigned short x, unsigned short y) {
	int width;
	int height;
	char* bitmap;

	bitmap = read_xpm(xpm, &width, &height);
	int i,j;
	for(i = 0; i < height; i++) {
		for(j = 0; j < width; j++) {
			paintPixel(base, x + j, y + i, BACKGROUND_COLOR);
			//printf("x: %d, y: %d cor: %d\n", x+j, y+i, bitmap[(i * width) + j]);

		}
	}

}

void printRectangle(char* base, unsigned short x, unsigned short y, unsigned short sizeX, unsigned short sizeY, unsigned long color, unsigned long borderColor) {

	int xMax = (x + sizeX) > HRES ? HRES : (x + sizeX);
	int yMax = (y + sizeY) > VRES ? VRES : (y + sizeY);
	int xi = x;
	int yi = y;

	while(y < yMax)  {
	  while(x < xMax) {
		if(y == yi || y == (yMax-1) || x == xi || x == (xMax-1)) {
			paintPixel(base, x, y, borderColor);
		}
		else {
			paintPixel(base, x, y, color);
		}
		x++;
	  }
	  x -= sizeX;
	  y++;

	}

}

void showPalette(char* base, unsigned short x, unsigned short y) {


	printRectangle(base, 0 + x, 0 + y, 30, 30, BLUE, BLACK);
	printRectangle(base, 0 + x, 40 + y, 30, 30, GREEN, BLACK);
	printRectangle(base, 0 + x, 80 + y, 30, 30, PINK, BLACK);
	printRectangle(base, 0 + x, 120 + y, 30, 30, RED, BLACK);


	printRectangle(base, 40 + x, 0 + y, 30, 30, GREY, BLACK);
	printRectangle(base, 40 + x, 40 + y, 30, 30, EGG, BLACK);
	printRectangle(base, 40 + x, 80 + y, 30, 30, BLACK, BLACK);
	printRectangle(base, 40 + x, 120 + y, 30, 30, PURPLE, BLACK);

	printRectangle(base, 80 + x, 0 + y, 30, 30, LIME, BLACK);
	printRectangle(base, 80 + x, 40 + y, 30, 30, CORAL, BLACK);
	printRectangle(base, 80 + x, 80 + y, 30, 30, CYAN, BLACK);
	printRectangle(base, 80 + x, 120 + y, 30, 30, ORANGE, BLACK);

	printRectangle(base, 120 + x, 0 + y, 30, 30, BROWN, BLACK);
	printRectangle(base, 120 + x, 40 + y, 30, 30, YELLOW, BLACK);
	printRectangle(base, 120 + x, 80 + y, 30, 30, WHITE, BLACK);
	printRectangle(base, 120 + x, 120 + y, 30, 30, MILITAR, BLACK);



}
