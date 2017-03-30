#pragma config(Sensor, S3, lightSensor, sensorLightActive)
#pragma config(Sensor, S2, sonarSensor, sensorSONAR)
#pragma config(Sensor, S1, colorPort,  sensorColorNxtRED)
#include "steering.h"
#include "commands.h"
//#include "monitoring.h"

task main(){
	/*
	*	0 = stop
	*	1 = turn left
	*	2 = turn right
	* 3 = go straight
	*/
	int next_crossroad = 0;
	/*
	*		Status describes the state the robot is currently in.
	*   0 = Robot does nothing
	*   1 = Folowing line
	*   2 = Making turn
	* 	3 = On crossroad
	*  	4 = Obstacle detected
	*		5 = Robot received stop command
	*/
	int status = 0;

	/*
	* 	The direction the robot is turning to
	* 	0 = Straight
	*		1 = Left
	*		2 = Right
	*/
	int direction = 0;
	int speed = 5;
	int correction = 9;
	int turn_value = 7;
	int reverse_turn_value = -1;
	int turn_time = 1000; // milliseconds


	while (status >= 0)
	{
		playSoundFile("duplo.rso");
		int right_sensor = SensorValue(lightSensor); // white is 60 black is 30
		int right_sensor_lowest = 30;
		int left_sensor = SensorValue[colorPort]; // white is 47 black is 20
		int left_sensor_lowest = 20;
		int distance = SensorValue[sonarSensor];

		// Temporary debugging values
		nxtDisplayTextLine(4, "Left: %d",left_sensor);
		nxtDisplayTextLine(5, "Right: %d",right_sensor);
		nxtDisplayTextLine(2, "Distance: %d",distance);

		// Check for bluetooth
		check_bluetooth(&status, &next_crossroad, &speed);

		// Obstacle detected
		if ( distance < 15 ){
			brake(10,&speed);
			status = 4;
		}

		// Crossroad detected
		else if (right_sensor < 35 && left_sensor < 30){
			status = 3;
			if ( next_crossroad  == 0 ){
				brake(10,&speed);
			}
			// turn left @ crossroad
			else if (next_crossroad  == 1){
				motor[motorC] = turn_value * speed;
				motor[motorA] = reverse_turn_value * speed;
				wait1Msec(turn_time);
			}
			// turn right @ crossroad
			else if (next_crossroad == 2){
				motor[motorA] = turn_value * speed;
				motor[motorC] = reverse_turn_value * speed;
				wait1Msec(turn_time);
			}
			// go straight @ crossroad
			else if (next_crossroad == 3){
				motor[motorA] = turn_value * speed;
				motor[motorC] = turn_value * speed;
				wait1Msec(turn_time);
			}
			status = 1;
			next_crossroad = 0;
		}
		// Following line
		else if ( status != 3 && status != 4 && status != 5){
			motor[motorA] = ((((left_sensor - left_sensor_lowest) / 3) + ((left_sensor - left_sensor_lowest) / right_sensor_lowest)) * speed) - correction;
			motor[motorC] = ((((right_sensor - right_sensor_lowest) / 3)) * speed) - correction;
		}
	}
}
