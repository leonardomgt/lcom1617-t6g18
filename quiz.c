#include "timer.h"
#include "keyboard.h"
#include "utils.h"


int main(int argc, char **argv) {

//	initLog();

	sef_startup();
//	printLetterText(0);

	char* base = (char*)vg_init(GRAPHICS_MODE_105);
	//char* base = NULL;

	log("test", "test");

	paintAllScreen(base, BACKGROUND_COLOR);
	/*
	int i = 10;
	while(i-- > 0) {
		tickdelay(micros_to_ticks(WAIT_KBC*1000));

	}
	*/
	//generate_question(base);

//	printLetterText(base);
	printSentence("quiz", 20,50,base,BLUE);

//	showPalette(base, 100, 100);
//	int player;
//
//	unsigned char answer;
//	unsigned short time;
//
//	player = wait_for_player_turn();
//
//	wait_for_player_answer(base, 10, 20, 800, 10, 15, &answer, &time);
//	vg_exit();
//
//	switch (player) {
//	case -1:
//		printf("something went wrong with the keyboard");
//		break;
//	case 0:
//		printf("Player 1 (space was pressed first)\n");
//		printf("answer: %d\n", answer);
//		printf("time spent: %d\n", time);
//		break;
//	case 1:
//		printf("Player 2 (enter was pressed first)\n");
//		printf("answer: %d\n", answer);
//		printf("time spent: %d\n", time);
//		break;
//
//	}




//	printf("Antes do resize\n");
//
//	char** tmp;
//	resizeLetter(A,tmp,2);
//	free(tmp);

	if(!wait_for_key(ENTER_SCANCODE)) {
		vg_exit();

	}


}
