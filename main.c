#pragma config(Sensor, S3, lightSensor, sensorLightActive)
#pragma config(Sensor, S2, sonarSensor, sensorSONAR)
#pragma config(Sensor, S1, colorPort,  sensorColorNxtFULL)
//#include "steering.h"
//#include "monitoring.h"
//#include "commands.h"
task main()
{
	wait10Msec(50);
	/*
	*		Status describes the state the robot is currently in.
	*   0 = Robot does nothing
	*   1 = Folowing line
	*   2 = Making turn
	* 	3 = On crossroad
	*  	4 = Obstacle detected
	*/
	int status = 0;
	/*
	* 	The direction the robot is turning to
	* 	0 = Straight
	*		1 = Left
	*		2 = Right
	*/
	int direction = 0;

	while (status >= 0){
			displayTextLine(2, "  While");
		//if(status == 1){
		//	if(check_obstacle()){
		//			brake(10);
		//	}
		//	else if(check_crossroad()){
		//			brake(10);
		//	}
		//	else if (check_track()){

		//	}
		int right_sensor = SensorValue(lightSensor);
		// 1 = black 6 = white
		int left_sensor = SensorValue[colorPort];
		nxtDisplayTextLine(4, "Color: %d",left_sensor);
		nxtDisplayTextLine(5, "Light: %d",right_sensor);
		if(right_sensor < 45 && left_sensor == 1){
			displayTextLine(3, " Forward");
			motor[motorA] = 20;
			motor[motorC] = 20;
		}
		 //Left turn
		else if (right_sensor > 45 && left_sensor == 1 ){
			displayTextLine(3, " Left");
			status = 2;
			direction = 1;
			motor[motorA] = 5;
			motor[motorC] = 20;
		}
		//// Right turn
		else if (right_sensor < 45 && left_sensor == 6){
			displayTextLine(3, " Right");
			status = 2;
			direction = 1;
			motor[motorA] = 20;
			motor[motorC] = 5;
		}
	}
}
