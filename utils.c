#include "utils.h"

void initLog() {

	FILE* logFile = NULL;
	logFile = fopen(PATH_TO_LOG, "a+");
	fprintf(logFile, "Starting program...\n");
	fclose(logFile);

}

void endLog() {

	FILE* logFile = NULL;
	logFile = fopen(PATH_TO_LOG, "a+");
	fprintf(logFile, "Program Ended.\n");
	fclose(logFile);

}

void initDebugger() {

	FILE* logFile = NULL;
	logFile = fopen(PATH_TO_DEBUG, "w");
	fprintf(logFile, "Starting debug...\n");
	fclose(logFile);

}

void log(const char* key, const char* value, char* path) {

	FILE* logFile = NULL;
	time_t timeSeconds = time(NULL);
	struct tm * timeInfo;

	time (&timeSeconds);
	timeInfo = gmtime (&timeSeconds);

	logFile = fopen(path, "a+");
	fprintf(logFile, "[%d/%d/%d - %d:%d:%d] %s: %s\n",
			timeInfo->tm_mday,
			timeInfo->tm_mon+1,
			timeInfo->tm_year+1900,
			timeInfo->tm_hour,
			timeInfo->tm_min,
			timeInfo->tm_sec,
			key,value);

	fclose(logFile);
}

void logVInt(const char* key, int var, char* path) {

	FILE* logFile = NULL;
	time_t timeSeconds = time(NULL);
	struct tm * timeInfo;

	time (&timeSeconds);
	timeInfo = gmtime (&timeSeconds);

	logFile = fopen(path, "a+");
	fprintf(logFile, "[%d/%d/%d - %d:%d:%d] %s: %d\n",
			timeInfo->tm_mday,
			timeInfo->tm_mon+1,
			timeInfo->tm_year+1900,
			timeInfo->tm_hour,
			timeInfo->tm_min,
			timeInfo->tm_sec,
			key,var);

	fclose(logFile);

}


int wait_for_key(unsigned char keyScancode) {

	int ipc_status;
	message msg;
	unsigned short irq_set = BIT(KEYBOARD_HOOK_ID);

	if(kbd_subscribe_int() != -1) {

		int is_E0 = 0;
		unsigned char scancode;

		while(scancode != keyScancode) {

			int r = driver_receive(ANY, &msg, &ipc_status);

			if(r != 0) {

				printf("driver_receive failed with: %d", r);
				continue;
			}

			if(is_ipc_notify(ipc_status)) {

				switch (_ENDPOINT_P(msg.m_source)) {

				case HARDWARE:

					if(msg.NOTIFY_ARG & irq_set) {

						keyboard_scan_handlerC(&scancode);

					}

					break;
				default:
					break;
				}
			}
		}
	}
	else {
		return 1;
	}

	if(!kbd_unsubscribe_int()) {

		printf("The end\n");
		return 0;
	}

	printf("unsubsribe did not return 0");

	return 1;
}


int keyPressed(scode* scancode) {

	int ipc_status;
	message msg;
	unsigned short irq_set = BIT(KEYBOARD_HOOK_ID);

	if(kbd_subscribe_int() != -1) {

		int r = driver_receive(ANY, &msg, &ipc_status);

		if(r != 0) {

			printf("driver_receive failed with: %d", r);

		}

		if(is_ipc_notify(ipc_status)) {

			switch (_ENDPOINT_P(msg.m_source)) {

			case HARDWARE:

				if(msg.NOTIFY_ARG & irq_set) {
					readFromKeyboard(scancode);
				}
				break;
			default:
				break;
			}
		}

	}

	else {

			return 0;

	}

	if(!kbd_unsubscribe_int()) {

		printf("The end\n");
		return 0;

	}

	printf("unsubsribe did not return 0");

	return 1;


}




//-1 = something went wrong, 0 = space key 1 = enter key

int wait_for_player_turn() {

	int ipc_status;
	message msg;
	unsigned short irq_set = BIT(KEYBOARD_HOOK_ID);
	int result = -1;

	if(kbd_subscribe_int() != -1) {

		unsigned char scancode;

		do {

			int r = driver_receive(ANY, &msg, &ipc_status);

			if(r != 0) {

				continue;
			}

			if(is_ipc_notify(ipc_status)) {

				switch (_ENDPOINT_P(msg.m_source)) {

				case HARDWARE:

					if(msg.NOTIFY_ARG & irq_set) {

						readFromKeyboard(&scancode);

					}

					break;
				default:
					break;
				}
			}
		} while(scancode != SPACE_SCANCODE && scancode != ENTER_SCANCODE);

		if(scancode == SPACE_SCANCODE) {

			result = 0;
		}
		else  {

			result = 1;
		}



	}
	else {
		return result;
	}

	if(!kbd_unsubscribe_int()) {

		return result;
	}

	return -1;


}

int wait_for_player_answer(char* base, unsigned short xi, unsigned short yi, short delta, unsigned short time, unsigned short height,unsigned char* answer, unsigned short* timeSpent) {

	delta = (delta + xi) >= HRES ? HRES - xi : delta;

	printRectangle(base, xi, yi, delta, height, GREEN, BLACK);
	int ipc_status;
	short initialDelta = delta;
	unsigned short initialTime = time;
	int interruptionsPixel = time*60 / delta;
	int pixelsInterreption = delta /(time*60);
	int speed = delta/time;
	time++;
	int interruptsCounter = 0;
	message msg;
	unsigned short irq_set_timer = BIT(TIMER_HOOK_ID);
	unsigned short irq_set_keyboard = BIT(KEYBOARD_HOOK_ID);

	unsigned int keyPressed = 0;

	if((timer_subscribe_int() != -1) && (kbd_subscribe_int() != -1 )) {

		while(delta>0 && !keyPressed) {

			int r = driver_receive(ANY, &msg, &ipc_status);

			if(r != 0) {

				printf("driver_receive failed with: %d", r);
				continue;
			}

			if(is_ipc_notify(ipc_status)) {

				switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE:

					if(msg.NOTIFY_ARG & irq_set_keyboard) {

						unsigned char key;
						readFromKeyboard(&key);

						if(key == ONE_SCANCODE) {
							keyPressed = *answer = 1;
							*timeSpent = (initialTime - (time -1));
							break;
						}
						else if(key == TWO_SCANCODE) {
							keyPressed = 1;
							*answer = 2;
							*timeSpent = (initialTime - (time - 1));
							break;
						}
						else if(key == THREE_SCANCODE) {
							keyPressed = 1;
							*answer = 3;
							*timeSpent = (initialTime - (time - 1));
							break;
						}
						else if(key == FOUR_SCANCODE) {
							keyPressed = 1;
							*answer = 4;
							*timeSpent = (initialTime - (time - 1));
							break;
						}

					}

					else if(msg.NOTIFY_ARG & irq_set_timer) {

						interruptsCounter++;

						if(!(interruptsCounter % 60)) {
							--time;
						}


						if(speed <= 60) {

							if(!(interruptsCounter % interruptionsPixel)) {

								printRectangle(base, xi, yi, delta, height, BACKGROUND_COLOR, BACKGROUND_COLOR);
								delta--;
								delta = delta < 0 ? 0 : delta;

								if(delta > (initialDelta * 2)/3) {
									printRectangle(base, xi, yi, delta, height, GREEN, BLACK);
								}
								else if(delta > initialDelta / 2) {
									printRectangle(base, xi, yi, delta, height, LIME, BLACK);
								}
								else if(delta > initialDelta / 3) {
									printRectangle(base, xi, yi, delta, height, YELLOW, BLACK);
								}
								else if(delta > initialDelta / 6) {
									printRectangle(base, xi, yi, delta, height, ORANGE, BLACK);
								}
								else {
									printRectangle(base, xi, yi, delta, height, RED, BLACK);
								}


							}

						}
						else {

							printRectangle(base, xi, yi, delta, height, BACKGROUND_COLOR, BACKGROUND_COLOR);
							delta-=pixelsInterreption;
							delta = delta < 0 ? 0 : delta;

							if(delta > (initialDelta * 2)/3) {
								printRectangle(base, xi, yi, delta, height, GREEN, BLACK);
							}
							else if(delta > initialDelta / 2) {
								printRectangle(base, xi, yi, delta, height, LIME, BLACK);
							}
							else if(delta > initialDelta / 3) {
								printRectangle(base, xi, yi, delta, height, YELLOW, BLACK);
							}
							else if(delta > initialDelta / 6) {
								printRectangle(base, xi, yi, delta, height, ORANGE, BLACK);
							}
							else {
								printRectangle(base, xi, yi, delta, height, RED, BLACK);
							}


						}

					}
					break;
				default:
					break;

				}

			}

		}

	}
	else {
		return 1;
	}

	if(!timer_unsubscribe_int() && !kbd_unsubscribe_int()) {

		if(!keyPressed) {
			*timeSpent = initialTime;
		}

		return 0;

	}
	return 1;

}

int generate_question(char* base) {

	srand((unsigned) time(NULL));
	unsigned* answer = (unsigned*) malloc(sizeof(unsigned) * 4);
	answer[0] = (rand() % 4) + 1;

	short i;
	for(i = 1; i < 4 ; i++) {

		unsigned element;
		do{
			element = (rand() % 4) + 1;
		}
		while (element == answer[i-1]);
		answer[i] = element;
	}

	for(i = 0; i < 4 ; i++) {

		switch(answer[i]){
		case 1:
			printRectangle(base, i*20 + 10, 20, 100, 30, GREEN, BLACK);
			//printf("i: %d = %d\n", i, answer[i]);
			break;
		case 2:
			printRectangle(base, i*20 + 10, 20, 100, 30, BLUE, BLACK);
			//printf("i: %d = %d\n", i, answer[i]);
			break;
		case 3:
			printRectangle(base, i*20 + 10, 20, 100, 30, RED, BLACK);
			//printf("i: %d = %d\n", i, answer[i]);
			break;
		case 4:
			printRectangle(base, i*20 + 10, 20, 100, 30, YELLOW, BLACK);
			//printf("i: %d = %d\n", i, answer[i]);
			break;
		}
	}
	free(answer);

}
