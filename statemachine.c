#include "statemachine.h"

void initiateGame(Game* game, char* base) {

	game->currentState = INITIAL_STATE;
	game->player1 = NULL;
	game->player2 = NULL;
	game->finish = 0;

	//desenhar
	printSentence("quizzzz", 50, 50, base, GREEN, BACKGROUND_COLOR);

}

void updateGame(Game* game, char* base) {

	log("updateGame", "updateGameChamado", PATH_TO_DEBUG);
	logVInt("finish", game->finish, PATH_TO_DEBUG);

	switch (game->currentState) {
	case INITIAL_STATE: {

		scode scanCode;
		do {
			keyPressed(&scanCode);
			logVInt("scancode", scanCode, PATH_TO_DEBUG);
		}
		while((scanCode != ESC_SCANCODE) && (scanCode != ENTER_SCANCODE));

		if(scanCode == ESC_SCANCODE) {
			game->finish = 1;
		}
		else {
			game->currentState = WAIT_FOR_PLAYER1_NAME;
			printRectangle(base, 262, 200, 500, 40, WHITE, BLACK);
			showPalette(base, 437, 250);

			printRectangle(base, 262, 450, 500, 40, WHITE, BLACK);
			showPalette(base, 437, 500);
		}
		break;

	}
	case WAIT_FOR_PLAYER1_NAME: {

		scode scanCode;
		do {
			keyPressed(&scanCode);
		}
		while(scanCode != ENTER_SCANCODE);

		game->currentState = WAIT_FOR_PLAYER2_NAME;
		printRectangle(base, 262, 450, 500, 40, WHITE, BLACK);
		showPalette(base, 437, 500);

		break;

	}

	case WAIT_FOR_PLAYER2_NAME: {

		scode scanCode;
		do {
			keyPressed(&scanCode);
		}
		while(scanCode != ENTER_SCANCODE);

		game->currentState = START_GAME;

		break;

	}

	case START_GAME: {

		scode scanCode;
		do {
			keyPressed(&scanCode);
		}
		while(scanCode != ESC_SCANCODE);

		game->finish = 1;

		break;

	}

	}

}
