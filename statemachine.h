#include "keyboard.h"
#include "timer.h"
#include "utils.h"

typedef enum { INITIAL_STATE, WAIT_FOR_PLAYER1_NAME, WAIT_FOR_PLAYER2_NAME, START_GAME , LAUNCH_QUESTION, WAIT_FOR_PLAYER_TURN, WAIT_FOR_PLAYER_ANSWER, FINAL_STATE } GameState;

typedef struct {

	char* name;
	unsigned int score;
	unsigned int color;


} Player;


typedef struct {

	Player* player1;
	Player* player2;
	GameState currentState;
	int finish;


} Game;

void initiateGame(Game* game, char* base);
void updateGame(Game* game, char* base);
