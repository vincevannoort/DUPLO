/*! \brief Give the robot a certain speed
 *
 *	\param[in] int speed, the speed of the robot
 */
//void drive(int speed){
	//motor[motorA] = speed;
	//motor[motorC] = speed;
//}
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
 *	\param[in] int time_to_stop, the time in wich the robot must be stading still
 */
void brake(int time_to_stop){
	motor[motorA] = 0;
	motor[motorC] = 0;
}
