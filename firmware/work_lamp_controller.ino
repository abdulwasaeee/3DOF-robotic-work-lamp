0/*
  3DOF Robotic Work Lamp
  ----------------------
  Controller: Arduino UNO R4 Minima

  Motors:
    D5 - Waist MG996R
    D6 - Shoulder MG996R
    D7 - Elbow MG996R

  Joint selection:
    D2 - Waist button
    D3 - Shoulder button
    D4 - Elbow button

  Joystick:
    A0 - X axis
    D8 - Push button / Home

  Servos are powered from an external 5V supply.
  Arduino and servo supply MUST share GND.
*/

#include <Servo.h>

// -------------------------
// SERVOS
// -------------------------

Servo waist;
Servo shoulder;
Servo elbow;

const int WAIST_SERVO_PIN = 5;
const int SHOULDER_SERVO_PIN = 6;
const int ELBOW_SERVO_PIN = 7;


// -------------------------
// CONTROLLER
// -------------------------

const int WAIST_BUTTON = 2;
const int SHOULDER_BUTTON = 3;
const int ELBOW_BUTTON = 4;

const int JOYSTICK_X = A0;
const int JOYSTICK_BUTTON = 8;


// -------------------------
// POSITIONS
// -------------------------

int waistPos = 90;
int shoulderPos = 90;
int elbowPos = 90;


// -------------------------
// SAFE SOFTWARE LIMITS
// -------------------------

const int WAIST_MIN = 30;
const int WAIST_MAX = 150;

const int SHOULDER_MIN = 70;
const int SHOULDER_MAX = 110;

const int ELBOW_MIN = 60;
const int ELBOW_MAX = 120;


// -------------------------
// SELECTED JOINT
// -------------------------
// 0 = Waist
// 1 = Shoulder
// 2 = Elbow

int selectedJoint = 0;


// -------------------------
// JOYSTICK SETTINGS
// -------------------------

const int DEAD_LOW = 430;
const int DEAD_HIGH = 590;

unsigned long lastMove = 0;
unsigned long lastButton = 0;

const int MOVE_INTERVAL = 25;
const int BUTTON_DEBOUNCE = 200;


// =====================================================
// HOME
// =====================================================

void goHome() {

  Serial.println("Going HOME...");

  while (
    waistPos != 90 ||
    shoulderPos != 90 ||
    elbowPos != 90
  ) {

    if (waistPos < 90) waistPos++;
    if (waistPos > 90) waistPos--;

    if (shoulderPos < 90) shoulderPos++;
    if (shoulderPos > 90) shoulderPos--;

    if (elbowPos < 90) elbowPos++;
    if (elbowPos > 90) elbowPos--;

    waist.write(waistPos);
    shoulder.write(shoulderPos);
    elbow.write(elbowPos);

    delay(20);
  }

  Serial.println("HOME reached");
}


// =====================================================
// SETUP
// =====================================================

void setup() {

  Serial.begin(9600);

  pinMode(WAIST_BUTTON, INPUT_PULLUP);
  pinMode(SHOULDER_BUTTON, INPUT_PULLUP);
  pinMode(ELBOW_BUTTON, INPUT_PULLUP);

  pinMode(JOYSTICK_BUTTON, INPUT_PULLUP);

  waist.attach(WAIST_SERVO_PIN);
  shoulder.attach(SHOULDER_SERVO_PIN);
  elbow.attach(ELBOW_SERVO_PIN);

  waist.write(waistPos);
  shoulder.write(shoulderPos);
  elbow.write(elbowPos);

  Serial.println();
  Serial.println("==============================");
  Serial.println("  3DOF ROBOTIC WORK LAMP");
  Serial.println("==============================");

  Serial.println("RED    -> Waist");
  Serial.println("BLUE   -> Shoulder");
  Serial.println("YELLOW -> Elbow");

  Serial.println("Joystick -> Move selected joint");
  Serial.println("Joystick click -> HOME");

  Serial.println();
  Serial.println("Ready.");
}


// =====================================================
// LOOP
// =====================================================

void loop() {

  // ---------------------------------------------------
  // JOINT SELECTION
  // ---------------------------------------------------

  if (millis() - lastButton > BUTTON_DEBOUNCE) {

    if (digitalRead(WAIST_BUTTON) == LOW) {

      selectedJoint = 0;
      Serial.println("Selected: WAIST");

      lastButton = millis();
    }

    else if (digitalRead(SHOULDER_BUTTON) == LOW) {

      selectedJoint = 1;
      Serial.println("Selected: SHOULDER");

      lastButton = millis();
    }

    else if (digitalRead(ELBOW_BUTTON) == LOW) {

      selectedJoint = 2;
      Serial.println("Selected: ELBOW");

      lastButton = millis();
    }

    else if (digitalRead(JOYSTICK_BUTTON) == LOW) {

      goHome();

      lastButton = millis();
    }
  }


  // ---------------------------------------------------
  // READ JOYSTICK
  // ---------------------------------------------------

  int joystick = analogRead(JOYSTICK_X);


  // ---------------------------------------------------
  // MOVEMENT TIMER
  // ---------------------------------------------------

  if (millis() - lastMove >= MOVE_INTERVAL) {

    lastMove = millis();

    int direction = 0;
    int stepSize = 0;


    // Joystick LEFT

    if (joystick < DEAD_LOW) {

      direction = -1;

      stepSize = map(
        joystick,
        DEAD_LOW,
        0,
        1,
        3
      );

      stepSize = constrain(stepSize, 1, 3);
    }


    // Joystick RIGHT

    else if (joystick > DEAD_HIGH) {

      direction = 1;

      stepSize = map(
        joystick,
        DEAD_HIGH,
        1023,
        1,
        3
      );

      stepSize = constrain(stepSize, 1, 3);
    }


    // -------------------------------------------------
    // MOVE SELECTED JOINT
    // -------------------------------------------------

    if (direction != 0) {

      // WAIST

      if (selectedJoint == 0) {

        waistPos += direction * stepSize;

        waistPos = constrain(
          waistPos,
          WAIST_MIN,
          WAIST_MAX
        );

        waist.write(waistPos);
      }


      // SHOULDER

      else if (selectedJoint == 1) {

        shoulderPos += direction * stepSize;

        shoulderPos = constrain(
          shoulderPos,
          SHOULDER_MIN,
          SHOULDER_MAX
        );

        shoulder.write(shoulderPos);
      }


      // ELBOW

      else if (selectedJoint == 2) {

        elbowPos += direction * stepSize;

        elbowPos = constrain(
          elbowPos,
          ELBOW_MIN,
          ELBOW_MAX
        );

        elbow.write(elbowPos);
      }
    }
  }
}
