#include <DabbleESP32.h>

// Motor control pins
const int IN1 = 18;
const int IN2 = 19;
const int IN3 = 21;
const int IN4 = 22;
const int ENA = 25;  // Left motor speed (PWM)
const int ENB = 26;  // Right motor speed (PWM)

// LED and Buzzer Pins
const int frontWhite1 = 33;     // Front left white LED
const int frontWhite2 = 17;    // Front right white LED
const int backRed1 = 16;       // Back left red LED
const int backRed2 = 4;        // Back right red LED
const int rgbDance1 = 27;      // 4-in-1 RGB LED #1
const int rgbDance2 = 14;      // 4-in-1 RGB LED #2
const int buzzerPin = 13;      // Active Buzzer

int speedVal = 140; // Default speed (0-255)

void setup() {
  // Motor setup
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);

  // LED & Buzzer setup
  pinMode(frontWhite1, OUTPUT); pinMode(frontWhite2, OUTPUT);
  pinMode(backRed1, OUTPUT); pinMode(backRed2, OUTPUT);
  pinMode(rgbDance1, OUTPUT); pinMode(rgbDance2, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  Serial.begin(9600);
  Dabble.begin("V1_ESP_CAR");
}

void loop() {
  Dabble.processInput();

  if (Terminal.available()) {
    String command = Terminal.readString();
    command.trim();
    command.toLowerCase();
    Serial.println("Voice Command: " + command);

    if (command == "forward") moveForward();
    else if (command == "backward") moveBackward();
    else if (command == "left") turnLeft();
    else if (command == "right") turnRight();
    else if (command == "stop") stopCar();
    else if (command == "dance") danceMode();
    else if (command == "drift") Tokyodrift();
    else if (command.startsWith("speed")) {
      int s = command.substring(5).toInt();
      if (s >= 0 && s <= 255) {
        speedVal = s;
        Serial.println("Speed changed to: " + String(speedVal));
      }
    }
  }

  // Gamepad joystick control
  if (GamePad.isUpPressed()) moveForward();
  else if (GamePad.isDownPressed()) moveBackward();
  else if (GamePad.isLeftPressed()) turnLeft();
  else if (GamePad.isRightPressed()) turnRight();
  else if (GamePad.isStartPressed()) stopCar();
}

// Movement functions
void moveForward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, speedVal); analogWrite(ENB, speedVal);
  digitalWrite(frontWhite1, HIGH); digitalWrite(frontWhite2, HIGH);
  digitalWrite(backRed1, LOW); digitalWrite(backRed2, LOW);
  beepTwice();
}

void moveBackward() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, speedVal); analogWrite(ENB, speedVal);
  digitalWrite(frontWhite1, LOW); digitalWrite(frontWhite2, LOW);
  digitalWrite(backRed1, HIGH); digitalWrite(backRed2, HIGH);
  beepTwice();
}

void turnLeft() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, speedVal); analogWrite(ENB, speedVal);
  beepTwice();

  // Indicator blink
  for (int i = 0; i < 3; i++) {
    digitalWrite(backRed1, HIGH);
    delay(80);
    digitalWrite(backRed1, LOW);
    delay(80);
  }
}

void turnRight() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, speedVal); analogWrite(ENB, speedVal);
  beepTwice();

  // Indicator blink
  for (int i = 0; i < 3; i++) {
    digitalWrite(backRed2, HIGH);
    delay(80);
    digitalWrite(backRed2, LOW);
    delay(80);
  }
}

void stopCar() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  analogWrite(ENA, 0); analogWrite(ENB, 0);
  digitalWrite(frontWhite1, LOW); digitalWrite(frontWhite2, LOW);
  digitalWrite(backRed1, LOW); digitalWrite(backRed2, LOW);
  beepTwice();
}

// Beep twice
void beepTwice() {
  for (int i = 0; i < 2; i++) {
    tone(buzzerPin, 2000, 100);  // Play 1000 Hz for 100ms
    for (int j = 0; j < 5; j++) {
      Dabble.processInput();
      delay(30);
    }                  // Small delay between beeps
    noTone(buzzerPin);
  }
}

void victoryBeep() {
  for (int i = 0; i < 2; i++) {
    tone(buzzerPin, 1500, 100);  // Slightly higher tone
    delay(150);
  }
}

// Dance Mode
void danceMode() {
  Serial.println("Dance Mode!");

  // Wiggle 5 times at speed 230
  for (int i = 0; i < 5; i++) {

    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
    analogWrite(ENA, 230); analogWrite(ENB, 230);
    digitalWrite(frontWhite1, HIGH); digitalWrite(frontWhite2, HIGH);
    digitalWrite(backRed1, HIGH); digitalWrite(backRed2, HIGH);
    delay(150);

    digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
    analogWrite(ENA, 230); analogWrite(ENB, 230);
    digitalWrite(frontWhite1, HIGH); digitalWrite(frontWhite2, HIGH);
    digitalWrite(backRed1, HIGH); digitalWrite(backRed2, HIGH);
    delay(150);

    digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
    analogWrite(ENA, 0); analogWrite(ENB, 0);
    digitalWrite(frontWhite1, LOW); digitalWrite(frontWhite2, LOW);
    digitalWrite(backRed1, LOW); digitalWrite(backRed2, LOW);   
    delay(50);

    // Optional short beep & RGB blink
    tone(buzzerPin, 600, 80);
    digitalWrite(rgbDance1, HIGH);
    digitalWrite(rgbDance2, HIGH);
    delay(80);
    digitalWrite(rgbDance1, LOW);
    digitalWrite(rgbDance2, LOW);
    delay(80);

    Dabble.processInput();
  }

  stopCar();  // Just in case

  // Melody + light show (no movement)
int melody[] = {
  659, 659, 0, 659, 0, 523, 659, 0,
  784, 0, 0, 392, 0, 0,
  523, 0, 392, 0, 330,
  440, 0, 494, 466, 440
};

int duration[] = {
  150, 150, 150, 150, 150, 150, 150, 150,
  300, 150, 150, 300, 150, 150,
  300, 150, 300, 150, 300,
  150, 150, 150, 150, 300
};

int size = sizeof(melody) / sizeof(int);
for (int i = 0; i < size; i++) {
  if (melody[i] != 0) {
    tone(buzzerPin, melody[i], duration[i]);
  }

  // LED light show
  if (i % 2 == 0) {
    digitalWrite(frontWhite1, HIGH);
    digitalWrite(frontWhite2, LOW);
    digitalWrite(backRed1, LOW);
    digitalWrite(backRed2, HIGH);
    digitalWrite(rgbDance1, HIGH);
    digitalWrite(rgbDance2, LOW);
  } else {
    digitalWrite(frontWhite1, LOW);
    digitalWrite(frontWhite2, HIGH);
    digitalWrite(backRed1, HIGH);
    digitalWrite(backRed2, LOW);
    digitalWrite(rgbDance1, LOW);
    digitalWrite(rgbDance2, HIGH);
  }

  delay(duration[i] * 1.2);  // Add 30% pause between notes
  noTone(buzzerPin);
  digitalWrite(rgbDance1, LOW);
  digitalWrite(rgbDance2, LOW);

  Dabble.processInput(); // Keep Dabble responsive
}
  noTone(buzzerPin);

  // Final pose: All lights ON for 3 seconds
  digitalWrite(frontWhite1, HIGH);
  digitalWrite(frontWhite2, HIGH);
  digitalWrite(backRed1, HIGH);
  digitalWrite(backRed2, HIGH);
  digitalWrite(rgbDance1, HIGH);
  digitalWrite(rgbDance2, HIGH);
  delay(3000);

  // Turn off RGB
  digitalWrite(rgbDance1, LOW);
  digitalWrite(rgbDance2, LOW);

  stopCar();
}


void Tokyodrift() {
  Serial.println("GO CRAZY!");

  for (int i = 0; i < 2; i++) {

    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
    analogWrite(ENA, 130); analogWrite(ENB, 240);
    digitalWrite(frontWhite1, HIGH); digitalWrite(frontWhite2, HIGH);
    digitalWrite(backRed1, HIGH); digitalWrite(backRed2, HIGH);
    delay(1000);

    digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
    analogWrite(ENA, 130); analogWrite(ENB, 240);
    digitalWrite(frontWhite1, HIGH); digitalWrite(frontWhite2, HIGH);
    digitalWrite(backRed1, HIGH); digitalWrite(backRed2, HIGH);
    delay(1000);

    digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
    analogWrite(ENA, 0); analogWrite(ENB, 0);
    digitalWrite(frontWhite1, LOW); digitalWrite(frontWhite2, LOW);
    digitalWrite(backRed1, LOW); digitalWrite(backRed2, LOW);   
    delay(50);
    Dabble.processInput();
  }

  stopCar();  // Just in case

 // Melody + light show (no movement)
  int melody[] = { 466, 494, 622, 466, 466, 466, 494, 622, 466, 466, 466, 494, 622, 466, 466 };
  int duration[] = { 80, 80, 100, 80, 160, 80, 80, 100, 80, 160, 80, 80, 100, 80, 160 };

  int size = sizeof(melody) / sizeof(int);  // Calculate how many notes

  for (int i = 0; i < size; i++) {
    tone(buzzerPin, melody[i], duration[i]);

    // Light chase effect
    if (i % 2 == 0) {
    digitalWrite(frontWhite1, HIGH);
    digitalWrite(frontWhite2, LOW);
    digitalWrite(backRed1, LOW);
    digitalWrite(backRed2, HIGH);
    digitalWrite(rgbDance1, HIGH);
    digitalWrite(rgbDance2, LOW);
    } else {
    digitalWrite(frontWhite1, LOW);
    digitalWrite(frontWhite2, HIGH);
    digitalWrite(backRed1, HIGH);
    digitalWrite(backRed2, LOW);
    digitalWrite(rgbDance1, LOW);
    digitalWrite(rgbDance2, HIGH);
    }
    delay(duration[i]);
    digitalWrite(rgbDance1, LOW);
    digitalWrite(rgbDance2, LOW);
    delay(50);

    Dabble.processInput();
  }
  noTone(buzzerPin);

  // Final pose: All lights ON for 3 seconds
  digitalWrite(frontWhite1, HIGH);
  digitalWrite(frontWhite2, HIGH);
  digitalWrite(backRed1, HIGH);
  digitalWrite(backRed2, HIGH);
  digitalWrite(rgbDance1, HIGH);
  digitalWrite(rgbDance2, HIGH);
  delay(3000);

  // Turn off RGB
  digitalWrite(rgbDance1, LOW);
  digitalWrite(rgbDance2, LOW);

  stopCar();
}