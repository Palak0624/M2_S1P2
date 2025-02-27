const byte LED_PIN = 13;
const byte METER_PIN = A4;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(METER_PIN, INPUT);
  
  Serial.begin(9600);
  Serial.println("Potentiometer-controlled LED blink");

  double timerFrequency = readPotentiometer();
  startTimer(timerFrequency);
}

void loop() {
  double timerFrequency = readPotentiometer();
  Serial.print("Updated Timer Frequency: ");
  Serial.print(timerFrequency);
  Serial.println(" Hz");

  startTimer(timerFrequency);
  delay(500); // Allow time before updating frequency
}

double readPotentiometer() {
  int sensorValue = analogRead(METER_PIN);
  double frequency = map(sensorValue, 0, 1023, 0.5 * 100, 2.0 * 100) / 100.0; // Map to 0.5Hz - 2Hz
  Serial.print("Potentiometer Value: ");
  Serial.print(sensorValue);
  Serial.print(" | Mapped Frequency: ");
  Serial.print(frequency);
  Serial.println(" Hz");
  
  return frequency;
}

void startTimer(double timerFrequency) {
  noInterrupts();

  TCCR1A = 0;
  TCCR1B = 0;

  double period = 1.0 / timerFrequency;
  unsigned long timerCount = (16000000 / 1024) * period / 2 - 1;

  OCR1A = (unsigned int)timerCount;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12) | (1 << CS10);
  TIMSK1 |= (1 << OCIE1A);

  Serial.print("Timer set for ");
  Serial.print(1.0 / timerFrequency);
  Serial.println(" second intervals");

  interrupts();
}

ISR(TIMER1_COMPA_vect) {
  digitalWrite(LED_PIN, digitalRead(LED_PIN) ^ 1);
  Serial.println("LED Toggled!");
}
