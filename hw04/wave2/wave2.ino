/*
  

 Author: Allen Downey 
 
 Based on http://arduino.cc/en/Tutorial/AnalogInput
 Created by David Cuartielles
 modified 30 Aug 2011
 By Tom Igoe

 License: Public Domain
 
 */
 
 
int ledPin = 5;       // select the pin for the LED
int buttonPin1 = 2;
int buttonPin2 = 3;
int lock;

void setup() {
  Serial.begin(9600);
  
  // a little fugly, but for some reason my compiler doesn't recognize
  // INPUT_PULLUP. I think for some reason it's compiling my ardy files
  // as .cpp instead of .c?
  pinMode(buttonPin1, INPUT);
  digitalWrite(buttonPin1, HIGH);
  pinMode(buttonPin2, INPUT);
  digitalWrite(buttonPin2, HIGH);
  //pinMode(buttonPin1, INPUT_PULLUP);  
  //pinMode(buttonPin2, INPUT_PULLUP);  

  pinMode(ledPin, OUTPUT);
  
  pinMode(13, OUTPUT);  
  pinMode(12, OUTPUT);  
  pinMode(11, OUTPUT);  
  pinMode(10, OUTPUT);  
  pinMode(9, OUTPUT);  
  pinMode(8, OUTPUT);  
  pinMode(7, OUTPUT);  
  pinMode(6, OUTPUT);

  lock = 0;

  //set timer interrupt at (freq) Hz
  TCCR0A = 0;// set entire TCCR2A register to 0
  TCCR0B = 0;// same for TCCR2B
  TCNT0  = 0;//initialize counter value to 0
  // set compare match register for 1760Hz increments
  // (1760Hz is an A)
  OCR0A = 141;// = (16*10^6) / (1760*64) - 1 (must be <256)
  // turn on CTC mode
  TCCR0A |= (1 << WGM01);
  // Set CS01 and CS00 bits for 64 prescaler
  TCCR0B |= (1 << CS01) | (1 << CS00);   
  // enable timer compare interrupt
  TIMSK0 |= (1 << OCIE0A);
}

ISR(TIMER0_COMPA_vect){//timer0 interrupt 2kHz toggles pin 8
//generates pulse wave of frequency 2kHz/2 = 1kHz (takes two cycles for full wave- toggle high then toggle low)
  if (lock){
    lock = 0;
  }
  else{
    lock = 1;
  }
}

void writeByte(int x) {
  int pin;

  // The six more significant bits go to the right end of PORTB;
  // the two less significant bits go to the left end of PORTD.
  // Also, each one is wrapped in ~(expr), because not.
  PORTB &= ~(x>>2); // pins 13..8 are &'d by the 6 most significant bits
  PORTD &= ~(x<<6); // pins 7..6 are &'d by the 2 least significant bits

/*
  for (pin=13; pin>=6; pin--) {
    digitalWrite(pin, x&1);
    x >>= 1;
  }
*/
}

int low = 36;
int high = 255;
int stride = 5;
int counter = low;

void loop() {
  lock = 1; // we're locked until the timer toggles again!
  
  int button1 = digitalRead(buttonPin1);
  if (button1) return;
  
  counter += stride;
  if (counter > high) {
    counter = low;
    //Serial.println(counter);
  }

  // write to the digital pins  
  writeByte(counter);

  while(lock) {
    // just... just wait for the interrupt.
  }
}
