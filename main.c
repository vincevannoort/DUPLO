#pragma config(Sensor, S3, lightSensor, sensorLightActive)
#pragma config(Sensor, S2, sonarSensor, sensorSONAR)
#pragma config(Sensor, S1, colorPort,  sensorColorNxtRED)
#include "steering.h"
//#include "monitoring.h"
//#include "commands.h"
task main()
{
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
	int speed = 4;


	while (status >= 0){
		int right_sensor = SensorValue(lightSensor); // white is 60 black is 30
		int left_sensor = SensorValue[colorPort]; // white is 47 black is 20
		int distance = SensorValue[sonarSensor];

		// Temporary debugging values
		nxtDisplayTextLine(4, "Left: %d",left_sensor);
		nxtDisplayTextLine(5, "Right: %d",right_sensor);
		nxtDisplayTextLine(2, "Distance: %d",distance);

		// Obstacle detected
		if ( distance < 15 ){
			brake(10);
			status = 4;
		}

		// Crossroad detected
		else if (right_sensor < 35 && left_sensor < 25){
			brake(10);
			status = 3;
		}

		// Following line
		else if ( status != 3 && status != 4){
			motor[motorA] = (((left_sensor - 20) / 3) + ((left_sensor - 20) / 30)) * speed;
			motor[motorC] = (((right_sensor - 30)/3)) * speed;
		}
	}
}
