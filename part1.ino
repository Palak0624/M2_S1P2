const byte LED_PIN = 13;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600); 
  Serial.println("Starting LED Blink (2 sec interval)");
  startTimer();
}

void loop() {
}

void startTimer() {
  noInterrupts();

  TCCR1A = 0;
  TCCR1B = 0;

  OCR1A = 15624; // (16MHz / 1024 prescaler) * 2s - 1
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12) | (1 << CS10);
  TIMSK1 |= (1 << OCIE1A);

  interrupts();
}

ISR(TIMER1_COMPA_vect) {
  digitalWrite(LED_PIN, digitalRead(LED_PIN) ^ 1); // Toggle LED
  Serial.println("LED Toggled!");
}
