/*! \brief Turn the robot left or right depending on the degrees given
*
*	\param[in] int motor_left, value for the left motor
*  \param[in] int motor_right, value for the right motor
* 	\param[in] int speed, speed of the nxt.
*
*/
void turn(int motor_left, int motor_right, int speed, int turn_time) {

	motor[motorA] = motor_left * speed;
	motor[motorC] = motor_right * speed;
	wait1Msec(turn_time);
}

/*! \brief Stop the robot in a certain time
*
*	\param[in] int time_to_stop, the time in which the robot must be standing still
*/
void brake(int time_to_stop, int *speed, int *left_speed, int *right_speed){
	if (time_to_stop != 0) {
		for(int i = time_to_stop-1; i > 0; --i){
			if (*left_speed <= 0 && *right_speed <= 0){
				break;
			}
			float f = (float)i/(float)time_to_stop;
			*left_speed *= f;
			*right_speed *= f;
			motor[motorA] = *left_speed;
			motor[motorC] = *right_speed;
			wait1Msec(50);
		}
	} else {
		motor[motorA] = 0;
		motor[motorC] = 0;
	}
	clearSounds();
	status = 5;
	*speed = 5;
}

void avoid_obstacle(int *speed, int *left_speed, int *right_speed, int turn_time){
	brake(100, speed, left_speed, right_speed);
	turn(5, -5, *speed, turn_time*2);
	status = 1;
}
