/*! @file
 * \brief This file provides the steering functions for the robot.
 * \section file steering
 */

/*!
 * \brief Turn the robot in a direction depending on the values provided.
 * \section turnfunction turn
 *
 * First, we only turn with the provided values, then we start checking our sensor and stop once it sees black.
 *
 * \param[in] int motor_left Value for the left motor
 * \param[in] int motor_right Value for the right motor
 * \param[in] int sensor_black_value A threshold value, below which we assume the sensor value is black
 * \param[in] tSensors sensor The sensor we need to check the value of.
 */
void turn(int motor_left, int motor_right, int sensor_black_value, tSensors sensor){
	left_speed = motor_left * speed;
	right_speed = motor_right * speed;
	motor[motorA] = left_speed;
	motor[motorC] = right_speed;
	wait1Msec(500);
	while(SensorValue(sensor) > (sensor_black_value + 5)){
		displayCenteredTextLine(3, "%d  %d", SensorValue(sensor), sensor_black_value);
	}
	while(SensorValue(sensor) < (sensor_black_value - 5)){
		displayCenteredTextLine(3, "%d  %d", SensorValue(sensor), sensor_black_value);
	}
}

/*!
 * \brief Stop the robot in a given time.
 * \section brakefunction brake
 *
 * Stop the robot within a certain time based on the provided parameter.
 * If it is 0, we stop immediately, else we slow down in steps.
 * Once the speed of both motors reaches 0, we end our loop.
 * After that, we stop the sound, reset the status and speed (factor).
 *
 * \param[in] int time_to_stop, the number of steps in which the robot stops
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

/*!
 * \brief Let the robot stop and then make a 180 degree turn.
 * \section obstaclefunction avoid_obstacle
 *
 * If we encounter an obstacle, we first brake using our brake function.
 * After that, we turn 180 degrees and reset the status.
 *
 * \param[in] int turn_time The time in wich the robot makes the turn.
 */
void avoid_obstacle(int turn_time){
	brake(100);
	left_speed = 5;
	right_speed = -5;
	motor[motorA] = left_speed * speed;
	motor[motorC] = right_speed * speed;
	wait1Msec(turn_time*2);
	status = 1;
}

/*!
 * \brief Gives the robot instructions when it reaches a crossroad.
 * \section crossroadfunction handle_crossroad
 *
 * If we encounter a crossroad, we decide which way to go based on input from the app, connected via Bluetooth.
 * Bluetooth input is stored in a Queue, we dequeue one value from it and act based on that value.
 * If the Queue is emtpy, it returns 0, so we brake and wait until another value is provided.
 *
 * \param[in] Queue *next_crossroad_queue Pointer to the queue which contains the next instructions for crossroads
 * \param[in] int turn_value Speed for the forward moving motor.
 * \param[in] int reverse_turn_value Speed for the reverse moving motor.
 * \param[in] int sensor_black_value A threshold value, below which we assume the sensor value is black.
 */
void handle_crossroad(Queue *next_crossroad_queue, int turn_value, int reverse_turn_value, int sensor_black_value){
	int next_crossroad_queue_item = dequeue(next_crossroad_queue);
	status = 3;
	displayCenteredTextLine(3, "%d", next_crossroad_queue_item);
	if (next_crossroad_queue_item  == 0 ){
		brake(0);
	} else {
		// turn left @ crossroad
		if (next_crossroad_queue_item  == 1){
			turn(reverse_turn_value, turn_value, sensor_black_value, rightSensor);
		}
		// turn right @ crossroad
		else if (next_crossroad_queue_item == 2){
			turn(turn_value, reverse_turn_value, sensor_black_value, leftSensor);
		}
		// go straight @ crossroad
		else if (next_crossroad_queue_item == 3){
			motor[motorA] = 10;
			motor[motorC] = 10;
			wait1Msec(400);
			}
		status = 1;
	}
}

/*!
 * \brief Let the robot follow a line.
 * \section drivefunction drive
 *
 * Let the motor speed depend on sensor input on that side of the robot.
 * The higher (lighter/whiter) the input value, the higher the motor speed.
 *
 * \param[in] int left_sensor Value of the left light sensor.
 * \param[in] int right_sensor Value of the right sensor.
 * \param[in] int sensor_lowest_value The lowest value with a migration background.
*/
void drive(int left_sensor, int right_sensor, int sensor_lowest_value){
	int correction = 14 + speed / 5;
	left_speed = (((left_sensor - sensor_lowest_value) / 3) * speed) - correction;
	right_speed = (((right_sensor - sensor_lowest_value) / 3) * speed) - correction;
	motor[motorA] = left_speed;
	motor[motorC] = right_speed;
	status = 1;
}

/*!
 * \brief Handles a sharp turn, for example a 90 degree turn
 * \section sharpturnfunction handle_sharp_turn
 *
 * We turn until either our sensor doesn't see black anymore or both our sensors do.
 * If our one sensor doesn't see black anymore, the loop finishes and readjust slightly in the other direction.
 * If both see black, we change the status to 5, so that during the next main loop, we end up at the crossroad function.
 *
 * \param[in] int turn_value Speed for the forward moving motor.
 * \param[in] int reverse_turn_value Speed for the reverse moving motor.
 * \param[in] int sensor_black_value Threshold value for a black reading.
 * \param[in] int sensor_lowest_value Lowest value for a sensor reading.
 * \param[in] tSensors first_sensor Sensor linked to the motor to turn to.
 * \param[in] tSensors second_sensor Sensor linked to the motor to readjust to.
*/
void handle_sharp_turn(int turn_value, int reverse_turn_value, int sensor_black_value, int sensor_lowest_value, tSensors first_sensor, tSensors second_sensor){
	while(SensorValue(second_sensor) > (sensor_black_value)){
		left_speed = turn_value * speed;
		right_speed = reverse_turn_value * speed;
		motor[motorA] = left_speed;
		motor[motorC] = right_speed;
		if(SensorValue(first_sensor) < sensor_black_value - 5 && SensorValue(second_sensor) < sensor_black_value - 5){
			status = 5;
			break;
		}
	}
	if (status != 5) {
		left_speed = reverse_turn_value * speed;
		right_speed = turn_value * speed;
		motor[motorA] = left_speed;
		motor[motorC] = right_speed;
		wait1Msec(75);
		for(int i= 0; i < 500; i++){
			drive(SensorValue(leftSensor), SensorValue(rightSensor), sensor_lowest_value);
		}
	}

}
