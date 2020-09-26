
/***************************************************

  This is the control code for the Voodoo Manufacturing gravity sort system.
  Designed for aligner pair sorting, but adaptable to order sorting.
  Hardware and software designed and built by Russell Bingham
  (August 2019; contact: rbingham97@gmail.com - questions welcome!).

  Software system characterization:
    Inputs:
      - Bin number from Serial (VPS)
    Outputs:
      - Correct hardware state
      - Confirmation of sorting success (TODO)

  Developed based on the Adafruit I2C Servo Driver example code and library.

 ****************************************************/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// define master board with the default address 0x40
Adafruit_PWMServoDriver pwm0 = Adafruit_PWMServoDriver();
// define left board with next address 0x41
Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x41,&Wire);
// define right board with address 0x42
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x42,&Wire);

// Tweak these values to match the physical characteristics of the servos in use
#define SERVO_LEFT  150 // pulse length to turn servo to "9 o'clock"
#define SERVO_RIGHT  600 // pulse length for "3 o'clock"
#define SERVO_CENTER 375 // "12 o'clock"
#define SERVO_ACTIVE_OFFSET 135 //TWEAK for channel width
#define SERVO_TOP_OFFSET 200
#define SERVO_GATE_OFFSET 225
#define SERVO_DECISION_OFFSET 200

// Tweak these values to determine the wait times for ergonomic/throughput optimization
#define SECOND_LENGTH 1000 // num of milliseconds in a second
#define SORT_TIME 2 // TO TWEAK: num of seconds to wait for operator to insert piece

uint8_t binNum;

uint16_t servoRestVals[48] = {300, 395, 345, 415, 300, 375, 375, 375, // board 0, 0-7 - 0-4 in use
                              375, 375, 375, 375, 375, 375, 375, 375, // 8-15 - unused
                              410, 365, 410, 375, 385, 370, 390, 380, // board 1, 0-7 - far left column
                              365, 360, 380, 400, 355, 380, 380, 370, // 8-15 - middle left column
                              390, 360, 355, 365, 360, 365, 385, 385, // board 2, 0-7 - middle right
                              350, 370, 370, 365, 360, 375, 380, 360
                             }; // 8-15 - far right


void setup() {
  Serial.begin(9600);
  Serial.println("Initializing...");

  pwm0.begin();
  pwm0.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

  pwm1.begin();
  pwm1.setPWMFreq(60);

  pwm2.begin();
  pwm2.setPWMFreq(60);

  Serial.println("Bin Sorting Active!");

  activateHardware(33);

  delay(10);
}


// inputs: I2C board instance, servo number 0-15, boolean to turn on/off
// function handles all flapper configurations (different locations/orientations)
void controlFlapper(Adafruit_PWMServoDriver board, uint8_t boardNum, uint8_t servoNum, bool active) {

  uint16_t pulselen;

  // separate by target board
  if (boardNum == 0) { // top board

    // this needs refactoring, but for now leaving it expanded for ease of prototyping
    if (servoNum == 0) { // top level flapper
      if (active) { // send pieces to the right
        pulselen = servoRestVals[16 * boardNum + servoNum] + SERVO_TOP_OFFSET;
      }
      else { // pieces go left
        pulselen = servoRestVals[16 * boardNum + servoNum];
      }
    }

    else { // second level flappers + gate flappers
      if (active) {
        if (servoNum == 1) { // left side
          pulselen = servoRestVals[16 * boardNum + servoNum] - SERVO_DECISION_OFFSET;
        }
        else if (servoNum == 2) { // right side
          pulselen = servoRestVals[16 * boardNum + servoNum] + SERVO_DECISION_OFFSET;
        }
        else if (servoNum == 3) { // left gate
          pulselen = servoRestVals[16 * boardNum + servoNum] - SERVO_GATE_OFFSET;
        }
        else { // right gate
          pulselen = servoRestVals[16 * boardNum + servoNum] + SERVO_GATE_OFFSET;
        }
      }
      else { // stay vertical
        pulselen = servoRestVals[16 * boardNum + servoNum];
      }
    }

    board.setPWM(servoNum, 0, pulselen);
  } // end top board case

  else { // module boards
    if (active) {
      if (servoNum < 8) { // left side column
        pulselen = servoRestVals[16 * boardNum + servoNum] - SERVO_ACTIVE_OFFSET;
      }
      else { // right side column
        pulselen = servoRestVals[16 * boardNum + servoNum] + SERVO_ACTIVE_OFFSET;
      }
    }
    else {
      pulselen = servoRestVals[16 * boardNum + servoNum];
    }

    board.setPWM(servoNum, 0, pulselen); // actually send signal to motor
  } // end module boards case
}

// function to control the state of the gate at the top of the system
// isolated for readability
void controlGate(bool active) {

  controlFlapper(pwm0, 0, 3, active); // left flapper
  controlFlapper(pwm0, 0, 4, active); // right

}

// helper to select and activate the correct combination of motors for the bin input
void activateHardware(uint8_t binNum) {

  if (binNum == 33) { // calibration/setup (deactivate all flappers for placement)
    for (uint8_t servoNum = 0; servoNum < 16; servoNum++) { //master board
      controlFlapper(pwm0, 0, servoNum, false);
    }
    for (uint8_t servoNum = 0; servoNum < 16; servoNum++) { //left module
      controlFlapper(pwm1, 1, servoNum, false);
    }
    for (uint8_t servoNum = 0; servoNum < 16; servoNum++) { //right module
      controlFlapper(pwm2, 2, servoNum, false);
    }
    Serial.println("Configuration set to reset");
  }
  else { // normal operation
    if (binNum <= 16) { // left side
      // control top level
      controlFlapper(pwm0, 0, 0, false);

      // control second level
      controlFlapper(pwm0, 0, 1, binNum < 9);

      // control tower level
      controlFlapper(pwm1, 1, binNum - 1, true);
    }
    else { // right side
      // control top level
      controlFlapper(pwm0, 0, 0, true);

      // control second level
      controlFlapper(pwm0, 0, 2, binNum > 24);

      // control tower level
      controlFlapper(pwm2, 2, binNum - 17, true);
    }
    Serial.print("Configuration set to bin ");
    Serial.println(binNum);
  }
}


// helper to control the validity of inputs
bool isValidBin(uint8_t input) {
  if (input > 0 && input <= 33) {
    return true;
  }
  else {
    return false;
  }
}

uint8_t parseInput() {
  // first section largely from an arduino serial input tutorial:
  // https://forum.arduino.cc/index.php?topic=396450.0
  String receivedInput = "X";   // a string to store the received data
  static byte ndx = 0;

  boolean newData = false;
  char rc;

  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();

    if (rc != '\n') {
      receivedInput = receivedInput + rc;
      ndx++;
    }
    else {
      newData = true;
    }
  }

  // end code adapted from tutorial

  uint8_t outInt = (receivedInput.substring(3)).toInt();

  return outInt;

}


void loop() {
  uint8_t temp = 0;
  if (Serial.available() > 0) {
    temp = parseInput();
    if (temp != 0) { // if new input, accept it
      binNum = temp;
    }
  }

  if (isValidBin(binNum)) {
    if (temp != 0) { // new input
      activateHardware(binNum); // set flapper states
      delay(500);
      controlGate(true); // open gate
    }
    delay(SECOND_LENGTH * SORT_TIME); // wait
    controlGate(false); // close gate
    if (temp != 0) {
      activateHardware(33); // reset
    }
  }
  else {
    Serial.println("Please enter a valid bin number (1-33)");
    activateHardware(33); // reset
    delay(1000);
  }

}
