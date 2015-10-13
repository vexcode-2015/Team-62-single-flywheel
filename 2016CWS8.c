#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, dgtl12, ballHigh,       sensorTouch)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Motor,  port1,           feeder,        tmotorVex393TurboSpeed_HBridge, openLoop)
#pragma config(Motor,  port2,           LUflywheel,    tmotorVex393TurboSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           LDflywheel,    tmotorVex393TurboSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           LBMdrive,      tmotorVex393TurboSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port5,           LFdrive,       tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port6,           RBMdrive,      tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port7,           RFdrive,       tmotorVex393TurboSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           RDflywheel,    tmotorVex393TurboSpeed_MC29, openLoop, encoderPort, I2C_1)
#pragma config(Motor,  port9,           RUflywheel,    tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port10,          intake1,       tmotorVex393TurboSpeed_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX)

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!
int motorSpeed = 0;
//float velocity;
int waitTime = 270;
bool isOkayToShoot = true;

void speedUpFlywheel(){
	while(motorSpeed < 90){
		if(motorSpeed < 90)
			motorSpeed += 2;
		else
			motorSpeed = 90;

		motor[LUflywheel] = motorSpeed;
		motor[LDflywheel] = motorSpeed;
		motor[RUflywheel] = motorSpeed;
		motor[RUflywheel] = motorSpeed;

		wait1Msec(250);
	}
}

void slowDownFlywheel(){
	while(motorSpeed > 0){
		if(motorSpeed > 0)
			motorSpeed -= 2;
		else
			motorSpeed = 0;

		motor[LUflywheel] = motorSpeed;
		motor[LDflywheel] = motorSpeed;
		motor[RUflywheel] = motorSpeed;
		motor[RDflywheel] = motorSpeed;

		wait1Msec(250);
	}
}

task shooter(){
	while(true){
		if(vexRT(Btn6U)){
			if(!vexRT(Btn6D)){
				speedUpFlywheel();
				motor[LUflywheel] = 127;
				motor[LDflywheel] = 127;
				motor[RUflywheel] = 127;
				motor[RDflywheel] = 127;
			}


		}
		else if(vexRT(Btn6D)){
			if(!vexRT(Btn6U))
				slowDownFlywheel();
		}
		wait1Msec(25);
	}
}



task drive(){
	while(true){
		motor[LFdrive] 	= vexRT(Ch3);
		motor[LBMdrive]	= vexRT(Ch3);
		motor[RFdrive] 	= vexRT(Ch2);
		motor[RBMdrive]	= vexRT(Ch2);
		wait1Msec(25);
	}
}

task timeCheck () {
	while(!SensorValue(ballHigh))
}

task loadFire(){ // make this shit simpler
	while(true){
		clearTimer(T1);
		while(!SensorValue(ballHigh))
			motor[feeder] = 127;
		while(SensorValue(ballHigh)) {
			if(time1[T1] < waitTime)
				motor[feeder] = 0;
			else
				motor[feeder] = 127;
		}
	}

}

task intake(){ //make this shit simpler
	int swap = 0;
	while(true){
		if(vexRT(Btn8D)){
			startTask(loadFire);
			swap = 1;
		}

		else if(vexRT(Btn8U)){
			stopTask(loadFire);
			motor[feeder] = 0;
			swap = 0;
		}


		else if(vexRT(Btn5U)){
			motor[intake1] = 127;

		}
		else if(vexRT(Btn5D)){
			motor[feeder] = 127;
		}
		else if(swap == 0){
			motor[intake1] = 0;
			motor[feeder] = 0;
		}
		wait1Msec(25);
	}
}

void pre_auton()
{
	bStopTasksBetweenModes = true;
}


task autonomous()
{
	AutonomousCodePlaceholderForTesting();  // Remove this function call once you have "real" code.
}

task usercontrol()
{
	startTask(shooter);
	startTask(drive);
	startTask(intake);
}
