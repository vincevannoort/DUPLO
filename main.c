#pragma config(Sensor, S3, lightSensor, sensorLightActive)
#pragma config(Sensor, S2, sonarSensor, sensorSONAR)
#pragma config(Sensor, S1, colorPort,  sensorColorNxtFULL)
#include "steering.h"
//#include "monitoring.h"
//#include "commands.h"
task main()
{
	wait10Msec(50);
	/*
	*		Status describes the state the robot is currently in.
	*   0 = Robot does nothing
	*   1 = Folowing line
	*   2 = Making turn
	* 	3 = On crossroad
	*  	4 = Obstacle detected
	*/
	int status = 0;
	/*
	* 	The direction the robot is turning to
	* 	0 = Straight
	*		1 = Left
	*		2 = Right
	*/
	int direction = 0;

	// max speed is 50
	int speed = 10;
	while (status >= 0){
				// 0 is darker 100 is lighter
		int right_sensor = SensorValue(lightSensor);
		// 1 = black 6 = white
		int left_sensor = SensorValue[colorPort];

		nxtDisplayTextLine(4, "Color: %d",left_sensor);
		nxtDisplayTextLine(5, "Light: %d",right_sensor);

		int distance = SensorValue[sonarSensor];
		nxtDisplayTextLine(2, "Distance: %d",distance);
		int temp_once = 1;
		if ( distance < 15 ){
			brake(10);
			if (temp_once == 1) {
				PlaySoundFile("allahu.rso");
				temp_once = 0;
			}
		}

		// Forward
		if(right_sensor > 45 && left_sensor == 6){
			displayTextLine(3, " Forward");
			status = 1;
			drive(speed);
		}

		// Crossroad
		else if (right_sensor < 45 && left_sensor == 1){

			if (status != 3){
				drive(-5);
				wait1Msec(250);
				brake(10);
			}
			status = 3;
		}

		//Left turn
		else if (right_sensor > 45 && left_sensor == 1 ){
			displayTextLine(3, " Left");
			status = 2;
			direction = 1;
			turn(-12,speed);
		}

		// Right turn
		else if (right_sensor < 45 && left_sensor == 6){
			displayTextLine(3, " Right");
			status = 2;
			direction = 2;
			turn(12,speed);
		}
	}
}
