#pragma config(Sensor, S3, lightSensor, sensorLightActive)
#pragma config(Sensor, S2, sonarSensor, sensorSONAR)
#pragma config(Sensor, S1, colorPort,  sensorColorNxtRED)
#include "steering.h"
//#include "monitoring.h"
//#include "commands.h"

long nLastXmitTimeStamp = nPgmTime;
long nDeltaTime         = 0;

const int kMaxSizeOfMessage = 30;
const int INBOX = 5;

task main(){
	// Bluetooth vars
	TFileIOResult nBTCmdRdErrorStatus;
	int nSizeOfMessage;
	ubyte nRcvBuffer[kMaxSizeOfMessage];

	/*
	*	0 = stop
	*	1 = turn left
	*	2 = turn right
	* 3 = go straight
	*/
	int next_crossroad = 0;
	/*
	*		Status describes the state the robot is currently in.
	*   0 = Robot does nothing
	*   1 = Folowing line
	*   2 = Making turn
	* 	3 = On crossroad
	*  	4 = Obstacle detected
	*		5 = Robot received stop command
	*/
	int status = 0;

	/*
	* 	The direction the robot is turning to
	* 	0 = Straight
	*		1 = Left
	*		2 = Right
	*/
	int direction = 0;
	int speed = 5;
	int correction = 9;
	int turn_value = 7;
	int reverse_turn_value = -1;
	int turn_time = 1000; // milliseconds

	while (status >= 0)
	{
		// Check to see if a message is available

		nSizeOfMessage = cCmdMessageGetSize(INBOX);

		if (nSizeOfMessage > kMaxSizeOfMessage)
			nSizeOfMessage = kMaxSizeOfMessage;
		if (nSizeOfMessage > 0){
			nBTCmdRdErrorStatus = cCmdMessageRead(nRcvBuffer, nSizeOfMessage, INBOX);
			nRcvBuffer[nSizeOfMessage] = '\0';
			string s = "";
			stringFromChars(s, (char *) nRcvBuffer);
			if (s == "UP"){
				displayCenteredTextLine(3,"We go up");
				status = 1;
			}
			else if (s == "LEFT"){
				displayCenteredTextLine(3,"To the left it is");
				next_crossroad = 1;
			}
			else if (s == "RIGHT"){
				displayCenteredTextLine(3,"Make a rigth turn");
				next_crossroad = 2;
			}
			else if (s == "DOWN"){
				status = 5;
				brake(10);
			}

			displayCenteredTextLine(4, s);
		}


		int right_sensor = SensorValue(lightSensor); // white is 60 black is 30
		int right_sensor_lowest = 30;
		int left_sensor = SensorValue[colorPort]; // white is 47 black is 20
		int left_sensor_lowest = 20;
		int distance = SensorValue[sonarSensor];

		// Temporary debugging values
		nxtDisplayTextLine(4, "Left: %d",left_sensor);
		nxtDisplayTextLine(5, "Right: %d",right_sensor);
		nxtDisplayTextLine(2, "Distance: %d",distance);

		// Obstacle detected
		if ( distance < 15 ){
			brake(10);
			status = 4;
		}

		// Crossroad detected
		else if (right_sensor < 35 && left_sensor < 30){
			status = 3;
			if ( next_crossroad  == 0 ){
				brake(10);

			}
			// turn left @ crossroad
			else if (next_crossroad  == 1){
				motor[motorC] = turn_value * speed;
				motor[motorA] = reverse_turn_value * speed;
				wait1Msec(turn_time);
			}
			// turn right @ crossroad
			else if (next_crossroad == 2){
				motor[motorA] = turn_value * speed;
				motor[motorC] = reverse_turn_value * speed;
				wait1Msec(turn_time);
			}
			// go straight @ crossroad
			else if (next_crossroad == 3){
				motor[motorA] = turn_value * speed;
				motor[motorC] = turn_value * speed;
				wait1Msec(turn_time);
			}
			status = 1;
			next_crossroad = 0;
		}
		// Following line
		else if ( status != 3 && status != 4 && status != 5){
			motor[motorA] = ((((left_sensor - left_sensor_lowest) / 3) + ((left_sensor - left_sensor_lowest) / right_sensor_lowest)) * speed) - correction;
			motor[motorC] = ((((right_sensor - right_sensor_lowest) / 3)) * speed) - correction;
		}
	}
}
