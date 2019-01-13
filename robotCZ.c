/*
 * Author           - Nick Zana
 * Date             - 1.10.19
 * Description      - A library to make RobotC development for EV3 robots easy
 */

#define RADIUS 2.75
#define CIRCUMFERENCE 17.27
#define WHEEL_DISTANCE 12.75
#define PI 3.14159265359

enum DistanceUnit {
  METERS,
  CENTIMETERS,
  KILOMETERS
};

//TODO: Currently, all speed is of unit power.
//Should implement method for unit conversion.
enum SpeedUnit {
  //ROBOT
  MOTOR_POWER,
  //Metric
  CMPS, // Centimeters/Second
  MPS, // Meters/Second
  KMPH, // Kilometers/Hour
  //Imperial
  IPS, // Inches/Second
  FPS, // Feet/Second
  MPH // Miles/Hour
};

enum Direction {
  Left,
  Right
};

enum TurnType {
  Pivot,
  Stationary
};

void print(float number) {
  displayCenteredTextLine(3, "%5.2f", number);
}

/**
 * void setPrimaryMotorPower()   - Sets the power of the drive motors
 *
 * @param int power              - The power to set the motor at
 */
void setPrimaryMotorsPower(int power) {
  motor[LeftMotor] = power;
  motor[RightMotor] = power;
}

/**
 * void clearEncoders()         - Reset the motorEncoders for tracking a new motion
 */
void clearEncoders() {
    nMotorEncoder[LeftMotor] = 0;
    nMotorEncoder[RightMotor] = 0;
}

/**
 * int degreesForDistance()      - Gets the degrees that a wheel needs to turn
 *                               - to go a certain distance
 * @param int distance           - The distance along the circumference of the
 *                               - wheel needed
 * @param DistanceUnit unit      - The unit for the given distance
 */
int degreesForDistance(float distance, DistanceUnit unit) {
  if (unit == METERS) {
    distance = distance * 100;
  }
  int degrees = (int)((distance / CIRCUMFERENCE) * 360.0 + 0.5);
  return degrees;
}

/*
 * float distanceInCMForPivotTurnDegrees() - Get the distance that a wheel
                                           - needs to move along its
                                           - circumference in order to turn
                                           - the given number of degrees
                                           - in a Pivot turn
 * @param float degrees                    - The number of degrees that the
                                           - the robot should
 */
float distanceInCMForPivotTurnDegrees(float degrees) {
  float distance = (degrees / 360.0) * 2 * PI * WHEEL_DISTANCE;
  return distance;
}

/*
 * void turn()                    - Turn the robot a certain number of degrees
 * @param TurnType type           - The type of turn to be performed
 * @param float degrees           - The degrees
 * @param Direction dir           - Whether the robot should turn left or right
 * @param int power               - The power the turning motor(s) should go at
 */
void turn(TurnType type, float degrees, Direction dir, int power) {
  if (type == Pivot) {
    tMotor stationaryMotor;
    tMotor movingMotor;
    if(dir == Left) {
           stationaryMotor = motorB;
           movingMotor = motorC;
    } else {
           stationaryMotor = motorC;
           movingMotor = motorB;
    }

    float distanceForTurningMotor = distanceInCMForPivotTurnDegrees(degrees);
    float degreesToTurn =
          degreesForDistance(distanceForTurningMotor, CENTIMETERS);

    clearEncoders();

    motor[movingMotor] = power;
    while(nMotorEncoder[movingMotor] < degreesToTurn) {
         displayCenteredTextLine(3, "%d %5.2f", nMotorEncoder[motorB],
            nMotorEncoder[motorC] * CIRCUMFERENCE / 360.0);
    }

  } else if (type == Stationary) {
    tMotor forwardMotor;
    tMotor backwardMotor;
    if (dir == Left) {
      forwardMotor = RightMotor;
      backwardMotor = LeftMotor;
    } else {
      forwardMotor = LeftMotor;
      backwardMotor = RightMotor;
    }

    float distanceForMotors = distanceInCMForPivotTurnDegrees(degrees)/2.;
    float degreesToTurn = degreesForDistance(distanceForMotors,
      CENTIMETERS);


    clearEncoders();
    motor[forwardMotor] = power;
    motor[backwardMotor] = -1* power;

    while(nMotorEncoder[forwardMotor] < degreesToTurn) {
      displayCenteredTextLine(3, "%d %5.2f", nMotorEncoder[motorB],
         nMotorEncoder[motorC] * CIRCUMFERENCE / 360.0);
       }
  }

  setPrimaryMotorsPower(0);
}

/**
 * void moveDegrees()                  - The base function for moving the robot
 *
 * @param int degrees           - The number of degrees that the motor should
 *                                turn
 * @param int speed             - The percentage of the motor speed that the
 *                              - robot should go at when traveling
 * @param bool verbose          - Enter verbose mode to display movement
 *                              - information on the screen during operation
 */
void moveDegrees(int degrees, int power, bool verbose) {
    clearEncoders();

    setPrimaryMotorsPower(power);
    if (verbose) {
      if(degrees > 0) {
        while (nMotorEncoder[motorB] < degrees) {
            displayCenteredTextLine(3, "%d %5.2f", nMotorEncoder[motorB],
                nMotorEncoder[motorB] * CIRCUMFERENCE / 360.0);
        }
      } else {
        while (nMotorEncoder[motorB] > degrees) {
            displayCenteredTextLine(3, "%d %5.2f", nMotorEncoder[motorB],
                nMotorEncoder[motorB] * CIRCUMFERENCE / 360.0);
        }
      }
    } else {
      if(degrees > 0) {
        while(nMotorEncoder[motorB] < degrees) {}
      } else {
        while(nMotorEncoder[motorB] > degrees) {}
      }
    }
    setPrimaryMotorsPower(0);
}

/**
 * void move()                  - An abstracted moveDegrees that works
                                - in distances rather than motor degrees
 *
 * @param DistanceUnit unit     - the unit of the distance
 * @param int distance          - the distance that the robot should move
 * @param int speed             - The percentage of the motor speed that the
 *                              - robot should go at when traveling
 * @param bool verbose          - Enter verbose mode to display movement
 *                              - information on the screen during operation
 */
void move(int distance,
  DistanceUnit unit,
  int power,
  bool verbose) {

  //Calculates the number of degrees that the motors should turn
  int degrees = degreesForDistance(distance, unit);
  moveDegrees(degrees, power, verbose);
}

/**
 * float moveUntilTouch()       - Move the robot until the touch sensor
 *                              - is pressed
 * @param int speed             - The speeed at which the robot should
 *                              - move
 */
float moveUntilTouch(int speed) {
  clearEncoders();
  setPrimaryMotorsPower(speed);
  while(SensorValue(touchSensor) == 0) {}
  setPrimaryMotorsPower(0);
  float degreesTurned = nMotorEncoder[LeftMotor];
  return degreesTurned;
}
