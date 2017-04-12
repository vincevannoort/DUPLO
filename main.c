#pragma config(Sensor, S1, leftSensor, sensorLightActive)
#pragma config(Sensor, S2, sonarSensor, sensorSONAR)
#pragma config(Sensor, S3, rightSensor, sensorLightActive)

/*! @file
 * \brief Run the robot
 * \section function main
 *
 * This function contains the main loop of the robot, it checks all inputs and acts on it.
 * First, we set a number of variables, status and speed are global.
 * Within the actual loop, we check our sensors and store their values.
 * If the robot is moving, it makes a sound.
 * Then we check for Bluetooth input.
 * After that, we check if we encounter an obstacle.
 * Next, we check for a crossroad.
 * Then we move on to checking for sharp turns.
 * Finally, we use our 'regular' function to follow a relatively straight line.
 *
 */

int status;
int left_speed;
int right_speed;
int speed;

#include "queue.h"
#include "steering.h"
#include "commands.h"


task main(){
	/*
	*	0 = stop
	*	1 = turn left
	*	2 = turn right
	*	3 = go straight
	*/
	Queue next_crossroad_queue;
 	init_queue(&next_crossroad_queue);

	/*
	*	Status describes the state the robot is currently in.
	*	0 = Robot does nothing
	*	1 = Folowing line
	*	2 = Making turn
	*	3 = On crossroad
	*	4 = Obstacle detected
	*	5 = Robot braked on his own
	*	6 = Robot received brake command
	*/
	status = 0;

	// Controlling robot's speed
	speed = 5;
	left_speed = 0;
	right_speed = 0;

	int turn_value = 7;
	int reverse_turn_value = -1;
	int turn_time = 900;
	int sensor_lowest_value = 30;
	int sensor_black_value = 45;
	int correction_black = 8;

	while (status >= 0)
	{
		// Check sensors
		int right_sensor = SensorValue(rightSensor);
		int left_sensor = SensorValue(leftSensor);
		int distance = SensorValue[sonarSensor];

		// Play sound
		if (status == 1) {
			playSoundFile("duplo.rso");
		}

		// Temporary debugging values
		nxtDisplayTextLine(1, "Status: %d", status);
		nxtDisplayTextLine(4, "Left: %d",left_sensor);
		nxtDisplayTextLine(5, "Right: %d",right_sensor);
		nxtDisplayTextLine(2, "Distance: %d",distance);

		// Check for bluetooth
		check_bluetooth(&next_crossroad_queue);

		// Obstacle detected
		if (distance < 25 && status != 5 && status != 6){
			avoid_obstacle(turn_time);
		}

		// Crossroad detected
		else if ((right_sensor < sensor_black_value && left_sensor < sensor_black_value) || status == 5){
			handle_crossroad(&next_crossroad_queue, turn_value, reverse_turn_value, turn_time);
		}

		// Sharp right turn detected
		else if (right_sensor < (sensor_black_value - correction_black)){
			handle_sharp_turn(turn_value, reverse_turn_value - 5, sensor_black_value, sensor_lowest_value, rightSensor, leftSensor);
		}

		// Sharp left turn detected
		else if (left_sensor < (sensor_black_value - correction_black)){
			handle_sharp_turn(reverse_turn_value - 5, turn_value, sensor_black_value, sensor_lowest_value, leftSensor, rightSensor);
		}

		// Following line
		else if ( status != 3 && status != 4 && status != 5 && status != 6){
			drive(left_sensor, right_sensor, sensor_lowest_value);
		}
	}
}
