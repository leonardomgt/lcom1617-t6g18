#include "utils.h"

void initLog() {
	FILE* logFile = NULL;
	logFile = fopen(PATH_TO_LOG, "w");
	fprintf(logFile, "Starting program...\n");
	fclose(logFile);
}

void log(const char* key, const char* value) {

	FILE* logFile = NULL;
	time_t timeSeconds = time(NULL);
	struct tm * timeInfo;

	time (&timeSeconds);
	timeInfo = gmtime (&timeSeconds);

	logFile = fopen(PATH_TO_LOG, "a+");
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

void logVInt(const char* key, int var) {


	FILE* logFile = NULL;
	time_t timeSeconds = time(NULL);
	struct tm * timeInfo;

	time (&timeSeconds);
	timeInfo = gmtime (&timeSeconds);

	logFile = fopen(PATH_TO_LOG, "a+");
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

char** configLetter(char** xpm, unsigned char mainColor){
	char** xpm_tmp = xpm;

	//Change Main Color
	char* str1;
	char str2[2];
	str1 = "  ";
	itoa(mainColor, str2);
	char * newMainColor = (char *) malloc(1 + strlen(str1)+ strlen(str2) );
	strcpy(newMainColor, str1);
	strcat(newMainColor, str2);

	xpm_tmp[1] = newMainColor;

//	char str3[2];
//	str1 = "F ";
//	itoa(background, str2);
//	char * newBackground = (char *) malloc(1 + strlen(str1)+ strlen(str2) );
//	strcpy(newBackground, str1);
//	strcat(newBackground, str2);
//
//	xpm_tmp[2] = newBackground;

//	free(newBackground);
//	free(newMainColor);

	return xpm_tmp;
}

void resizeLetter(char** xpm, char** xpm_tmp, int size){


	log("estado", "inicializou o resize");
	int width=16, high =17, nColors =2;
	char spaceChar;

	log("dimensions", xpm[0]);

	sscanf( xpm[0], "%d %d %d", &width, &high, &nColors);

	logVInt("width", width);
	logVInt("high", high);
	logVInt("nColors", nColors);

	xpm_tmp = (char**)malloc((3 + high*size)*sizeof(char*));

	int n;
	for(n = 0; n < (3 + high*size); n++){
		xpm_tmp[n] = (char*) malloc(width*size);
	}

	//CHANGE STRING

	int counter = 3;

	while(counter <= (high + 3)){


		if(counter == (high + 3)){
			xpm_tmp[high + 3] = 0;
			break;
		}
//		logVInt("counter", counter);

		char newLine[width*size+1];

		memset(newLine, 0, width*size+1);
		int letters = 0;
		for(; letters < width; letters++){

			int i = 0;
			for(; i < size; i++){

				newLine[i+letters*size] = xpm[counter][letters];
//				logVInt("",i+letters*size);
			}

		}

		int i = 0;
		for(; i < size; i++){
			xpm_tmp[size*counter - 3 + i] = newLine;

			log("newLine", newLine);
		}


		counter++;
	}
	//CHANGE DIMENSIONS STRING


	log("estado", "acabou a primeira parte");

	width *= size;
	high *= size;

	logVInt("width", width);
	logVInt("high", high);

	char* space = " ";
	char str1[3];
	char str2[3];
	char str3[1];

	itoa(width, str1);
	itoa(high, str2);
	itoa(nColors, str3);


//	char dimensions[10];
	char* dimensions = (char *) malloc(3 + strlen(str1) + strlen(str2));
	strcpy(dimensions, str1);
	strcat(dimensions, space);
	strcat(dimensions, str2);
	strcat(dimensions, space);
	strcat(dimensions, str3);

	xpm_tmp[0] = dimensions;
	xpm_tmp[1] = xpm[1];
	xpm_tmp[2] = xpm[2];


	int i = 0;
	for(; i < 37; i++){
		log("", xpm_tmp[i]);
	}
}

char* itoa(int i, char b[]){
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}
