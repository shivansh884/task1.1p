const int ledPin = LED_BUILTIN;
const int buttonPin = 2;
const int dotDuration = 200;
const int dashDuration = 1000;
const int letterPause = 1000;
const int wordPause = 1200;

bool isBlinking = false;
volatile bool resetFlag = false; // Volatile because it is used in the ISR

void blinkDot() {
  digitalWrite(ledPin, HIGH);
  delay(dotDuration);
  digitalWrite(ledPin, LOW);
  delay(dotDuration);
}

void blinkDash() {
  digitalWrite(ledPin, HIGH);
  delay(dashDuration);
  digitalWrite(ledPin, LOW);
  delay(dotDuration);
}

void blinkCharacter(char c) {
 if (c == 'S') {
    blinkDot(); blinkDot(); blinkDot();
} else if (c == 'H') {
    blinkDot(); blinkDot(); blinkDot(); blinkDot();
} else if (c == 'I') {
    blinkDot(); blinkDot();
} else if (c == 'V') {
    blinkDot(); blinkDot(); blinkDash(); blinkDash();
} else if (c == 'A') {
    blinkDot(); blinkDash();
} else if (c == 'N') {
    blinkDash(); blinkDot();
} else if (c == 'S') { // Repeated for the second 'S'
    blinkDot(); blinkDot(); blinkDot();
} else if (c == 'H') { // Repeated for the second 'H'
    blinkDot(); blinkDot(); blinkDot(); blinkDot();
}

  delay(letterPause);
}

void blinkName(char name[], int length) {
  for (int i = 0; i < length; i++) {
    blinkCharacter(name[i]);
    if (resetFlag) break; // Check for reset flag after each character
  }
  delay(wordPause);
}

// Interrupt service routine (ISR) to set the reset flag
void handleButtonPress() {
  resetFlag = true;
}

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);

  // Attach interrupt to the button pin
  attachInterrupt(digitalPinToInterrupt(buttonPin), handleButtonPress, FALLING);
}

void loop() {
  if (resetFlag) {
    resetFlag = false; // Clear the reset flag
    isBlinking = true;
  }

  if (isBlinking) {
    char name[] = {'S', 'H', 'I', 'V', 'A', 'N', 'S', 'H'};
    int length = sizeof(name) / sizeof(name[0]);
    blinkName(name, length);
    isBlinking = false;
  }
}
