#include "timer.h"
#include "macros.h"
#include <minix/drivers.h>
#include <minix/com.h>
#include <minix/syslib.h>

static unsigned int G_timer_hook_id;

int timer_subscribe_int() {

	G_timer_hook_id = TIMER_HOOK_ID;
	unsigned int originalHookId = G_timer_hook_id;

	if(!sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &G_timer_hook_id)) {
			printf("Subscription of timer 0 correctly done!\n");
			return originalHookId;
	}

	return -1;
}

int timer_unsubscribe_int() {

	if(!sys_irqrmpolicy(&G_timer_hook_id)) {
			printf("Unsubscription of timer 0 correctly done!\n");
			return 0;
	}

	return 1;
}

void timer_int_handler(int* interruptCounter, int* secondsCounter) {

	(*interruptCounter)++;
	if(!((*interruptCounter) % 60)) {

		++(*secondsCounter);

	}
}
