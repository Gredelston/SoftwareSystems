#include <stdio.h>
#include <stdlib.h>

/* Updates the count!
 * If the input value is between 3 and 6, inclusive,
 * then the count is incremented.
 * If the input value is a 10 (or a non-ace face card),
 * then the count is decremented.
 */
int updateCount(int val, int count)
{
	if ((val > 2) && (val < 7)) {
		count++;
	} else if (val == 10) {
		count--;
	}
	return count;
}

/* From the card name, returns the card value.
 * Non-ace face cards return 10; aces return 11;
 * an X or otherwise invalid card returns 0.
 * 0 indicates that the main code should continue.
 * (If this is for an X, it will equate to breaking.)
 */
int getVal(char card_name[])
{
	int val;
	switch(card_name[0]) {
		case 'K':
		case 'Q':
		case 'J':
			val = 10;
			break;
		case 'A':
			val = 11;
			break;
		case 'X':
			val = 0;
			break;
		default:
			val = atoi(card_name);
			if ((val < 1) || (val > 10)) {
				puts("I don't understand that value!");
				val = 0;
			}
	}
	return val;
}

/* Prints the count. Duh.
 * count: int for the number of low cards thus far,
 * 		  minus the number of high cards thus far.
 */
void printCount(int count) {
	printf("Current count: %i\n", count);
}

/* The main code for the program.
 * User inputs a card value, either 1-10, J, Q, K, or A.
 * We then determine the Blackjack value of the card.
 * If the value is sufficiently low, we increase the count;
 * if it is sufficiently high, we decrease the count.
 * We then tell the user what the current count is, and repeat.
 * If the user ever inputs an X, we break.
 */
int main()
{
	char card_name[3];
	int count = 0;
	int val; /* uggh pedantic */
	/* ps, why does this stupid thing not recognize // */
	
	while ( card_name[0] != 'X' ) {
		puts("Enter the card_name: ");
		scanf("%2s", card_name);
		
		val = getVal(card_name);
		if (!val)
			continue;

		count = updateCount(val, count);
		printCount(count);
	}
	return 0;
}