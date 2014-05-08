#include "Time.h"

#define EPOCH 1970

typedef struct Data {
	char index;

	// the actual data
	char more;
	char less;
	struct Data *next;
} Data;

Data *first;
Data *last;
int incomingByte;

time_t START_TIME;
#define TICK_TIME (60*5)
int i;

Data *dataConstructor(word w) {
	Data *d = (Data *) check_malloc(sizeof(Data));
	if (d == NULL)
		return NULL;

	(d->next) = (Data *) check_malloc(sizeof(Data));
	if ((d->next) == NULL)
		return NULL;

	d->index = i;

	d->more = (char) w >> 16;
	d->less = (char) w % 255;
	d->next = (Data *) NULL;
}

Data *generateData() {
	word w = word(random(65535));
	return dataConstructor(w);
}

void *check_malloc(int size)
{
	void *p = malloc(size);
	if (p == NULL)
		noMemory();
	return p;
}

void noMemory() {
	analogWrite(13, HIGH);
	Serial.println("Ain't no memory left :/");
}

void pop(Data *first) {
	*first = *(first->next);
}

void send (Data *d) {
	Serial.print("T");
	time_t time_now = START_TIME + ((d->index)*TICK_TIME);
	Serial.print(time_now);

	Serial.print("M");
	Serial.print(d->more);
	Serial.print("L");
	Serial.print(d->less);

	Serial.print("0");
}

void setup() {
	Serial.begin(9600);
	first = NULL;
	last = NULL;
	incomingByte = 0;
	analogWrite(13, LOW);

	START_TIME = now();
	i = 0;
}

void loop() {
	char t = (char) minute();
	Serial.print("Index: ");
	Serial.println(i);

	// Listen to see if we're sending data!
	if ((Serial.available() > 0) && (first != NULL)) {
		incomingByte = Serial.read();
		if (incomingByte) {
			//send(first);
			pop(first);
		}
		return;
	}

	// Get and store data!
	Data *d = generateData();

	// If there's no data, complain!
	if (d == NULL) {
		noMemory();
		return;
	}

	// If we made it this far, we collected data!
	// So store it.
	i++;

	if (first == NULL) {
		first = d;
		last = d;
		return;
	} else {
		last->next = d;
		last = d;
	}

	return;
}