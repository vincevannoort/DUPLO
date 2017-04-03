#pragma config(Sensor, S3, rightSensor, sensorLightActive)
#pragma config(Sensor, S2, sonarSensor, sensorSONAR)
#pragma config(Sensor, S1, leftSensor, sensorLightActive)

int status;
int left_speed;
int right_speed;
int speed;

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
	status = 0;

	/*
	* Controlling robot's speed
	*/
	speed = 5;
	left_speed = 0;
	right_speed = 0;

	int turn_value = 7;
	int reverse_turn_value = -1;
	int turn_time = 900;
	int correction = 9;

	int sensor_lowest_value = 30;
	int sensor_black_value = 45;

	while (status >= 0)
	{
		int right_sensor = SensorValue(rightSensor); // white is 60 black is 30
		int left_sensor = SensorValue(leftSensor); // white is 60 black is 30
		int distance = SensorValue[sonarSensor];

		if (status == 1) {
			playSoundFile("duplo.rso");
		}

		// Temporary debugging values
		nxtDisplayTextLine(1, "Status: %d", status);
		nxtDisplayTextLine(4, "Left: %d",left_sensor);
		nxtDisplayTextLine(5, "Right: %d",right_sensor);
		nxtDisplayTextLine(2, "Distance: %d",distance);

		// Check for bluetooth
		check_bluetooth(&next_crossroad);

		// Obstacle detected
		if (distance < 20 && status != 5){
			avoid_obstacle(turn_time);
		}

		// Crossroad detected
		else if (right_sensor < sensor_black_value && left_sensor < sensor_black_value){
			handle_crossroad(&next_crossroad, turn_value, reverse_turn_value, turn_time);
		}

		// Following line
		else if ( status != 3 && status != 4 && status != 5){
			left_speed = (((left_sensor - sensor_lowest_value) / 3) * speed) - correction;
			right_speed = (((right_sensor - sensor_lowest_value) / 3) * speed) - correction;
			motor[motorA] = left_speed;
			motor[motorC] = right_speed;
			status = 1;
		}
	}
}
