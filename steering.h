/*! \brief Turn the robot left or right depending on the degrees given
*
*	\param[in] int motor_left, value for the left motor
*  \param[in] int motor_right, value for the right motor
* 	\param[in] int speed, speed of the nxt.
*
*/
void turn(int motor_left, int motor_right, int turn_time) {
	left_speed = motor_left * speed;
	right_speed = motor_right * speed;
	motor[motorA] = left_speed;
	motor[motorC] = right_speed;
	wait1Msec(turn_time);
}

/*! \brief Stop the robot in a certain time
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

void avoid_obstacle(int turn_time){
	brake(100);
	turn(5, -5, turn_time*2);
	status = 1;
}

void handle_crossroad(int *next_crossroad, int turn_value, int reverse_turn_value, int turn_time){
	status = 3;
	if (*next_crossroad  == 0 ){
		brake(0);
	}
	// turn left @ crossroad
	else if (*next_crossroad  == 1){
		turn(reverse_turn_value, turn_value, turn_time);
	}
	// turn right @ crossroad
	else if (*next_crossroad == 2){
		turn(turn_value, reverse_turn_value, turn_time);
	}
	// go straight @ crossroad
	else if (*next_crossroad == 3){
		turn(turn_value, turn_value, turn_time);
	}
	if (*next_crossroad != 0){
		status = 1;
	}
	*next_crossroad = 0;
}

void drive(int left_sensor, int right_sensor, int sensor_lowest_value){
	int correction = 9;
	left_speed = (((left_sensor - sensor_lowest_value) / 3) * speed) - correction;
	right_speed = (((right_sensor - sensor_lowest_value) / 3) * speed) - correction;
	motor[motorA] = left_speed;
	motor[motorC] = right_speed;
	status = 1;
}
