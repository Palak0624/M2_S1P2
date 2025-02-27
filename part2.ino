const byte LED_PIN = 13;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  
  double userFrequency = 1.0;  // Example: Set frequency to 1 Hz (1 sec blink)
  Serial.println("Starting Timer...");
  startTimer(userFrequency);
}

void loop() {
  // Nothing needed here, as the timer handles LED toggling
}

void startTimer(double timerFrequency) {
  noInterrupts(); // Disable interrupts while configuring
  
  Serial.print("Setting Timer Frequency to: ");
  Serial.print(timerFrequency);
  Serial.println(" Hz");

  double period = 1.0 / timerFrequency; // Period in seconds
  Serial.print("Calculated Timer Interval: ");
  Serial.print(period, 3);
  Serial.println(" seconds");

  OCR1A = (16000000 / 1024) * period / 2 - 1; // Compute OCR1A value
  TCCR1A = 0;   // Normal mode
  TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10); // CTC mode, prescaler 1024
  TIMSK1 = (1 << OCIE1A); // Enable Timer1 interrupt

  Serial.println("Timer Setup Complete!");
  interrupts(); // Enable interrupts
}

ISR(TIMER1_COMPA_vect) {
  digitalWrite(LED_PIN, digitalRead(LED_PIN) ^ 1); // Toggle LED
  
  Serial.print("LED Toggled! Current State: ");
  Serial.println(digitalRead(LED_PIN) ? "ON" : "OFF");
}
