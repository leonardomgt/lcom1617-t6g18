#include "statemachine.h"

int main(int argc, char **argv) {
	sef_startup();
//	printLetterText(0);

	initLog();
	initDebugger();
	log("teste","teste", PATH_TO_DEBUG);
	logVInt("teste", 29, PATH_TO_DEBUG);
	logVInt("teste", 29, PATH_TO_LOG);
	log("teste","teste", PATH_TO_LOG);

	char* base = (char*)vg_init(GRAPHICS_MODE_105);
	//char* base = NULL;
	paintAllScreen(base, BACKGROUND_COLOR);

	initDebugger();
	Game thisGame;
	printLetterXPM('j', 50, 50, base, BLUE, BACKGROUND_COLOR);

	/*
	initiateGame(&thisGame, base);

	while(!thisGame.finish) {
		updateGame(&thisGame, base);
	}

	/*



	vg_exit();

//	generate_question(base);

	/*

	showPalette(base, 100, 100);
	int player;

	unsigned char answer;
	unsigned short time;

	player = wait_for_player_turn();

	wait_for_player_answer(base, 10, 20, 800, 10, 15, &answer, &time);
	vg_exit();

	switch (player) {
	case -1:
		printf("something went wrong with the keyboard");
		break;
	case 0:
		printf("Player 1 (space was pressed first)\n");
		printf("answer: %d\n", answer);
		printf("time spent: %d\n", time);
		break;
	case 1:
		printf("Player 2 (enter was pressed first)\n");
		printf("answer: %d\n", answer);
		printf("time spent: %d\n", time);
		break;

	}
	*/

	if(!wait_for_key(ESC_SCANCODE)) {
		vg_exit();

	}



	endLog();
}
