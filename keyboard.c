
#include "keyboard.h"

static unsigned int G_kbd_hook_id;

int keyboard_scan_handlerC(unsigned char *scancode) {

	unsigned long tmp;

	//read scancode to st

	if(!sys_inb(OUT_REG, &tmp)) {

		*scancode = tmp;
		return 0;

	}
	return -1;

}

int kbd_subscribe_int() {

	G_kbd_hook_id = KEYBOARD_HOOK_ID;
	unsigned int originalHookId = G_kbd_hook_id;

	if(!sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &G_kbd_hook_id)) {
			printf("Subscription of Keyboard correctly done!\n");
			return originalHookId;
	}

	return -1;
}

int kbd_unsubscribe_int() {

	if(!sys_irqrmpolicy(&G_kbd_hook_id)) {
			printf("\nUnsubscription of Keyboard correctly done!\n");
			return 0;
	}

	printf("sys_irqrmpolicy did not return 0");


	return 1;

}

int writeToKeyboard(unsigned char cmd) {

	unsigned long stat;
	int exitLoop=0;
	while( exitLoop < 20 ) {
		exitLoop++;
		sys_inb(STATUS_REG, &stat); /* assuming it returns OK */
		/* loop while 8042 input buffer is not empty */
		if( (stat & IBF) == 0) {

			if(!sys_outb(KBD_CMD_REG, cmd) /* no args command */) {

				return 0;

			}

		}
		tickdelay(micros_to_ticks(WAIT_KBC));

	}
	return 1;
}

unsigned char readFromKeyboard(unsigned char *data) {


	unsigned long stat;
	unsigned long tmp;
	int exitLoop=0;
	while( exitLoop < 1000 ) {
		exitLoop++;

		sys_inb(STATUS_REG, &stat); /* assuming it returns OK */
		/* loop while 8042 output buffer is empty */

		if( stat & OBF ) {
			sys_inb(OUT_REG, &tmp); /* assuming it returns OK*/
			if ((stat & (PAR_ERR | TO_ERR)) == 0 ) {
				*data = tmp;
				return *data;
			}
				else
			return -1;
		}
	tickdelay(micros_to_ticks(WAIT_KBC));
	}
}

int readKeyboardUntil(unsigned char* array, unsigned int stringSize, unsigned char endScancode){

	unsigned int charCounter = stringSize;
	int ipc_status;
	message msg;
	unsigned short irq_set = BIT(G_kbd_hook_id);

	if(kbd_subscribe_int(&G_kbd_hook_id) != -1) {

		int is_E0 = 0;
		unsigned char scancode;

		while(scancode != endScancode && stringSize > 0) {

			int r = driver_receive(ANY, &msg, &ipc_status);

			if(r != 0) {

				printf("driver_receive failed with: %d", r);
				continue;
			}

			if(is_ipc_notify(ipc_status)) {


				switch (_ENDPOINT_P(msg.m_source)) {

				case HARDWARE:

					if(msg.NOTIFY_ARG & irq_set) {

						readFromKeyboard(&scancode);


						if(scancode == TWO_BYTES_SC){
							is_E0 = 1;
						}
						else if(is_E0){

							is_E0 = 0;
						}
						else{
							if(!(scancode & BIT(7))){ //Make Code

								array[stringSize - charCounter--] = scancode;

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

	if(!kbd_unsubscribe_int(&G_kbd_hook_id)) {

		printf("The end\n");
		return 0;

	}
}

char scancodeToChar(unsigned char scancode){

	char* characters = (char*)malloc(256);

	//Numbers
	characters[0x2] = '1';
	characters[0x3] = '2';
	characters[0x4] = '3';
	characters[0x5] = '4';
	characters[0x6] = '5';
	characters[0x7] = '6';
	characters[0x8] = '7';
	characters[0x9] = '8';
	characters[0xa] = '9';
	characters[0xb] = '0';

	//Down case letters
	characters[0x10] = 'q';
	characters[0x11] = 'w';
	characters[0x12] = 'e';
	characters[0x13] = 'r';
	characters[0x14] = 't';
	characters[0x15] = 'y';
	characters[0x16] = 'u';
	characters[0x17] = 'i';
	characters[0x18] = 'o';
	characters[0x19] = 'p';
	characters[0x1e] = 'a';
	characters[0x1f] = 's';
	characters[0x20] = 'd';
	characters[0x21] = 'f';
	characters[0x22] = 'g';
	characters[0x23] = 'h';
	characters[0x24] = 'j';
	characters[0x25] = 'k';
	characters[0x26] = 'l';
	characters[0x2c] = 'z';
	characters[0x2d] = 'x';
	characters[0x2e] = 'c';
	characters[0x2f] = 'v';
	characters[0x30] = 'b';
	characters[0x31] = 'n';
	characters[0x32] = 'm';

	characters[0x39] = ' ';

	//Upper case letters -  DOWN CASE + 0x30
	characters[0x40] = 'Q';
	characters[0x41] = 'W';
	characters[0x42] = 'E';
	characters[0x43] = 'R';
	characters[0x44] = 'T';
	characters[0x45] = 'Y';
	characters[0x46] = 'U';
	characters[0x47] = 'I';
	characters[0x48] = 'O';
	characters[0x49] = 'P';
	characters[0x4e] = 'A';
	characters[0x4f] = 'S';
	characters[0x50] = 'D';
	characters[0x51] = 'F';
	characters[0x52] = 'G';
	characters[0x53] = 'H';
	characters[0x54] = 'J';
	characters[0x55] = 'K';
	characters[0x56] = 'L';
	characters[0x5c] = 'Z';
	characters[0x5d] = 'X';
	characters[0x5e] = 'C';
	characters[0x5f] = 'V';
	characters[0x60] = 'B';
	characters[0x61] = 'N';
	characters[0x62] = 'M';

	unsigned char charResult = characters[scancode];

	free(characters);

	return charResult;
}

unsigned short lengthOfLetter(char letter){
	switch(letter){
	case 'i':
		return 9;
		break;
	case 'w':
		return 17;
		break;
	case 'm':
		return 22;
		break;
	case 'n':
		return 19;
		break;
	case '1':
		return 11;
		break;
	default:
		return 16;
		break;
	}
}

void printLetterXPM(char letter, unsigned short xi, unsigned short yi, char* base, unsigned char mainColor, unsigned char background){
	char** xpm_tmp;

	if(letter == ' ')
		printXPM(base, configLetter(letters[26], mainColor, background), xi, yi);

	else if(letter < 123 && letter > 96)
		printXPM(base, configLetter(letters[letter-97], mainColor, background), xi, yi);

	else if(letter < 58 && letter > 47)
		printXPM(base, configLetter(letters[letter-21], mainColor, background), xi, yi);
}


void printSentence(char* frase, unsigned short xi, unsigned short yi, char* base, unsigned char mainColor, unsigned char background){

	printLetterXPM(frase[0], xi, yi, base, mainColor, background);

	char previous = frase[0];
	xi += lengthOfLetter(frase[0]);

	int i;
	for(i = 1; i < strlen(frase); i++){
		printLetterXPM(frase[i], xi, yi, base, mainColor, background);

		char previous = frase[i];
		xi += lengthOfLetter(frase[i]);

	}
}

int printLetterText(char* base){
	char frase[12] = "";
	unsigned counter = 0;
	int ipc_status;
	message msg;
	unsigned short irq_set = BIT(KEYBOARD_HOOK_ID);

	if(kbd_subscribe_int() != -1) {

		int is_E0 = 0;
		unsigned char scancode;

		while(scancode != ENTER_SCANCODE) {

			int r = driver_receive(ANY, &msg, &ipc_status);

			if(r != 0) {

				printf("driver_receive failed with: %d", r);
				continue;
			}

			if(is_ipc_notify(ipc_status)) {


				switch (_ENDPOINT_P(msg.m_source)) {

				case HARDWARE:

					if(msg.NOTIFY_ARG & irq_set) {

						//keyboard_scan_handlerC(&scancode);
						readFromKeyboard(&scancode);


						if(scancode != ENTER_SCANCODE){


							if(scancode == BACKSPACE_SCANCODE && counter>0){
								frase[--counter] = 0;


							}
							else if(counter < 12){

								if(!(scancode & BIT(7)) && scancodeToChar(scancode) != ' '){
//									printf("%c : %d\n", scancodeToChar(scancode), counter++);

									frase[counter++] = scancodeToChar(scancode);
								}
								printSentence("            ", 20,20,base,45,BACKGROUND_COLOR);
								printSentence(frase, 20, 20, base, 45, BACKGROUND_COLOR);
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

	if(!kbd_unsubscribe_int(&G_kbd_hook_id)) {

		printf("The end\n");
		return 0;

	}

	printf("unsubsribe did not return 0");

	return 1;
}

char** configLetter(char** xpm, unsigned char mainColor, unsigned char background){
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

	char str3[2];
	str1 = "F ";
	itoa(background, str2);
	char * newBackground = (char *) malloc(1 + strlen(str1)+ strlen(str2) );
	strcpy(newBackground, str1);
	strcat(newBackground, str2);

	xpm_tmp[2] = newBackground;

//	free(newBackground);
//	free(newMainColor);

	return xpm_tmp;
}











