#pragma config(Sensor, S3, lightSensor, sensorLightActive)
#pragma config(Sensor, S2, sonarSensor, sensorSONAR)
#pragma config(Sensor, S1, colorPort,  sensorColorNxtRED)
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
	int speed = 12;
	while (status >= 0){
		// white is 60 black is 30
		int right_sensor = SensorValue(lightSensor);
		// white is 47 black is 20
		int left_sensor = SensorValue[colorPort];

		nxtDisplayTextLine(4, "Left: %d",left_sensor);
		nxtDisplayTextLine(5, "Right: %d",right_sensor);

		int distance = SensorValue[sonarSensor];
		nxtDisplayTextLine(2, "Distance: %d",distance);
		if ( distance < 15 ){
			brake(10);
		}


		// Crossroad
		if (right_sensor < 35 && left_sensor < 25){
			brake(10);
			status = 3;
		}
		else if ( status != 3){
			motor[motorA] = (((left_sensor - 20) / 3) + ((left_sensor - 20) / 30)) * 4;
			motor[motorC] = (((right_sensor - 30)/3)) * 4;
		}
	}
}
