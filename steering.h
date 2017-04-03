/*! \brief Turn the robot left or right depending on the degrees given
*
*	\param[in] int motor_left, value for the left motor
*  \param[in] int motor_right, value for the right motor
* 	\param[in] int speed, speed of the nxt.
*
*/
void turn(int motor_left, int motor_right, int speed) {
	int turn_time = 1000;

	motor[motorA] = motor_left * speed;
	motor[motorC] = motor_right * speed;
	wait1Msec(turn_time);
}

/*! \brief Stop the robot in a certain time
*
*	\param[in] int time_to_stop, the time in wich the robot must be stading still (1/10 second)
*/
void brake(int time_to_stop, int *speed){
	int default_speed = 10;
	int i = default_speed / time_to_stop;


	for (; time_to_stop >= 0; --time_to_stop) {
		motor[motorA] = (default_speed - i) * *speed;
		motor[motorC] = (default_speed - i) * *speed;
		wait1Msec(100);
	}

	motor[motorA] = 0;
	motor[motorC] = 0;
	clearSounds();
	*speed = 5;
}
