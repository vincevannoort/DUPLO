/*! \brief Give the robot a certain speed
 *
 *	\param[in] int speed, the speed of the robot
 */
void drive(int speed){
	motor[motorA] = speed;
	motor[motorC] = speed;
}
/*! \brief Turn the robot left or right depending on the degrees given
 *
 *	\param[in] int degrees, positve degrees will result in a right turn, negative degrees will result in a left turn
 * 	\param[in] int speed, speed of the robot
 */
void turn(int degrees, int speed){
	// right turn
	if(degrees >= 0){
			motor[motorA] = (speed * 0.5);
			motor[motorC] = -(speed * 0.5);
		}
		//left turn
	else if (degrees < 0){
			motor[motorA] = -(speed * 0.5);
			motor[motorC] = (speed * 0.5);
	}
}
/*! \brief Stop the robot in a certain time
 *
 *	\param[in] int time_to_stop, the time in wich the robot must be stading still
 */
void brake(int time_to_stop, int *speed){
	motor[motorA] = 0;
	motor[motorC] = 0;
	*speed = 5;
}
