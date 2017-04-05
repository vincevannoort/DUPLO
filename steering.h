/*! \file
 *  \brief This file provides the steering functions for the robot.
 */

/*! \brief Turn the robot left or right depending on the degrees given.
 *
 *  \param[in] int motor_left Value for the left motor
 *  \param[in] int motor_right Value for the right motor
 * 	\param[in] int turn_time Delay between each step in the turning process.
 */
void turn(int motor_left, int motor_right, int turn_time) {
	left_speed = motor_left * speed;
	right_speed = motor_right * speed;
	motor[motorA] = left_speed;
	motor[motorC] = right_speed;
	wait1Msec(turn_time);
}

/*! \brief Stop the robot in a given time.
 *
 *	\param[in] int time_to_stop, the time in which the robot must be standing still
 */
void brake(int time_to_stop){
	if (time_to_stop != 0) {
		for(int i = time_to_stop-1; i > 0; --i){
			if (left_speed <= 0 && right_speed <= 0){
				break;
			}
			float f = (float)i/(float)time_to_stop;
			left_speed *= f;
			right_speed *= f;
			motor[motorA] = left_speed;
			motor[motorC] = right_speed;
			wait1Msec(50);
		}
	} else {
		left_speed = 0;
		right_speed = 0;
		motor[motorA] = left_speed;
		motor[motorC] = right_speed;
	}
	clearSounds();
	status = 5;
	speed = 5;
}
/*! \brief Let the robot stop and then make a 180 degree turn.
 *
 *  \param[in] int turn_time The time in wich the robot makes the turn.
 */
void avoid_obstacle(int turn_time){
	brake(100);
	turn(5, -5, turn_time*2);
	status = 1;
}

/*! \brief Gives the robot instructions when it reaches a crossroad.
 *	 Check if a instruction is given by the app. If that is the case the robot will execute the given action.
 *	 If no option is given the robot will stop. It will clear the next_crossroad variable so new instructions
 *   can be given or the robot will stop at the next crossroad.
 *
 *  \param[in] Queue *next_crossroad_queue Pointer to the queue which contains the next instructions for crossroads
 *  \param[in] int turn_value Speed for the forward moving motor.
 *  \param[in] int reverse_turn_value Speed for the reverse moving motor.
 *  \param[in] int turn_time Time the robot waits between each step in the turning process.
 */
void handle_crossroad(Queue *next_crossroad_queue, int turn_value, int reverse_turn_value, int turn_time){
	int next_crossroad_queue_item = dequeue(next_crossroad_queue);
	status = 3;

	if (next_crossroad_queue_item  == 0 ){
		brake(0);
	} else {
		// turn left @ crossroad
		if (next_crossroad_queue_item  == 1){
			turn(reverse_turn_value, turn_value, turn_time);
		}
		// turn right @ crossroad
		else if (next_crossroad_queue_item == 2){
			turn(turn_value, reverse_turn_value, turn_time);
		}
		// go straight @ crossroad
		else if (next_crossroad_queue_item == 3){
			turn(turn_value, turn_value, turn_time/2);
		}
		status = 1;
	}
}

/*! \brief Let the robot follow a line.
 *
 *  \param[in] int left_sensor Value of the left light sensor.
 *  \param[in] int right_sensor Value of the right sensor.
 *  \param[in] int sensor_lowest_value The lowest value with a migration background.
*/
void drive(int left_sensor, int right_sensor, int sensor_lowest_value){
	int correction = 9;
	left_speed = (((left_sensor - sensor_lowest_value) / 3) * speed) - correction;
	right_speed = (((right_sensor - sensor_lowest_value) / 3) * speed) - correction;
	motor[motorA] = left_speed;
	motor[motorC] = right_speed;
	status = 1;
}

/*! \brief Handles a sharp turn, for example a 90 degree turn
 *
 *  \param[in] int turn_value Speed for the forward moving motor.
 *  \param[in] int reverse_turn_value Speed for the reverse moving motor.
 *  \param[in] int sensor_black_value Threshold value for a black reading.
 *  \param[in] int sensor_lowest_value Lowest value for a sensor reading.
 *  \param[in] tSensors first_sensor Sensor linked to the motor to turn to.
 *  \param[in] tSensors second_sensor Sensor linked to the motor to readjust to.
*/
void handle_sharp_turn(int turn_value, int reverse_turn_value, int sensor_black_value, int sensor_lowest_value, tSensors first_sensor, tSensors second_sensor){
	while(SensorValue(first_sensor) < sensor_black_value){
		turn(turn_value, reverse_turn_value, 2);
	}
	while(SensorValue(second_sensor) > (sensor_black_value)){
		turn(turn_value, reverse_turn_value, 2);
	}
	turn(reverse_turn_value, turn_value, 200);
	for(int i= 0; i < 500; i++) {
		drive(SensorValue(leftSensor), SensorValue(rightSensor), sensor_lowest_value);
	}
}
