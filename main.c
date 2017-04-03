#pragma config(Sensor, S3, rightSensor, sensorLightActive)
#pragma config(Sensor, S2, sonarSensor, sensorSONAR)
#pragma config(Sensor, S1, leftSensor, sensorLightActive)
#include "steering.h"
#include "commands.h"
//#include "monitoring.h"

task main(){
	/*
	*	0 = stop
	*	1 = turn left
	*	2 = turn right
	*	3 = go straight
	*/
	int next_crossroad = 0;

	/*
	*	Status describes the state the robot is currently in.
	*	0 = Robot does nothing
	*	1 = Folowing line
	*	2 = Making turn
	*	3 = On crossroad
	*	4 = Obstacle detected
	*	5 = Robot received stop command
	*/
	int status = 0;

	/*
	*	The direction the robot is turning to
	*	0 = Straight
	*	1 = Left
	*	2 = Right
	*/
	int direction = 0;
	int speed = 5;
	int correction = 9;
	int turn_value = 7;
	int reverse_turn_value = -1;
	int turn_time = 900;
	int left_speed = 0;
	int right_speed = 0;

	int sensor_lowest_value = 30;
	int sensor_black_value = 35;

	while (status >= 0)
	{
		if (status == 1)
			playSoundFile("duplo.rso");
		int right_sensor = SensorValue(rightSensor); // white is 60 black is 30
		int left_sensor = SensorValue(leftSensor); // white is 60 black is 30
		int distance = SensorValue[sonarSensor];

		// Temporary debugging values
		nxtDisplayTextLine(4, "Left: %d",left_sensor);
		nxtDisplayTextLine(5, "Right: %d",right_sensor);
		nxtDisplayTextLine(2, "Distance: %d",distance);

		// Check for bluetooth
		check_bluetooth(&status, &next_crossroad, &speed, left_speed, right_speed);

		// Obstacle detected
		if ( distance < 15 ){
			if (status != 5) {
				brake(100,&speed, &left_speed, &right_speed, &status);
				turn(5, -5, speed, turn_time*2);
			}
			//status = 4;
		}

		// Crossroad detected
		else if (right_sensor < sensor_black_value && left_sensor < sensor_black_value){
			status = 3;
			if ( next_crossroad  == 0 ){
				brake(100,&speed, &left_speed, &right_speed, &status);
			}
			// turn left @ crossroad
			else if (next_crossroad  == 1){
				turn(reverse_turn_value, turn_value, speed, turn_time);
			}
			// turn right @ crossroad
			else if (next_crossroad == 2){
				turn(turn_value, reverse_turn_value, speed, turn_time);
			}
			// go straight @ crossroad
			else if (next_crossroad == 3){
				turn(turn_value, turn_value, speed, turn_time);
			}
			status = 1;
			next_crossroad = 0;
		}

		// Following line
		else if ( status != 3 && status != 4){
			left_speed = (((left_sensor - sensor_lowest_value) / 3) * speed) - correction;
			right_speed = (((right_sensor - sensor_lowest_value) / 3) * speed) - correction;
			motor[motorA] = left_speed;
			motor[motorC] = right_speed;
			status = 1;
		}
	}
}
