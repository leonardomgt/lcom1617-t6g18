/**
 * @brief Subscribes and enables Timer 0 interrupts
 *
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int timer_subscribe_int();

/**
 * @brief Unsubscribes Timer 0 interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int timer_unsubscribe_int();

/**
 * @brief Timer 0 interrupt handler
 *
 * Increments counter
 */
void timer_int_handler(int* interruptCounter, int* secondsCounter);
