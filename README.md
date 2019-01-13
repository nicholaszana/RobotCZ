# RobotCZ

A library to make RobotC development for EV3 robots easy

## Getting Started

```
cd ~/Your/Project/Directory/
git clone https://github.com/nicholaszana/RobotCZ.git
```

Extract "robotCZ.c" to the same folder as the file that contains your main task.

Then in your main project file, add the following above your main task
```
#include "robotCZ.c"
```

That's it! Now you're ready to start programming your robot.

### Setting up your motors and sensors

Name your left motor LeftMotor.
Name your right motor RightMotor.
Name your touch sensor touchSensor.

The top of your file should look something like this:

```
#pragma config(Sensor, S1,     touchSensor,    sensorEV3_Touch)
#pragma config(Motor,  motorB,          LeftMotor,     tmotorNXT, PIDControl    , encoder)
#pragma config(Motor,  motorC,          RightMotor,    tmotorNXT, PIDControl    , encoder)

```
