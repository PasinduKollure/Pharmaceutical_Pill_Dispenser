
//Isaac Chang
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <iomanip>
using namespace std;
//struct holding required values
typedef struct
{
	int drugCount[4];
	int drugAngles[4];
	int codeList[8][8];
	int drugType;
	int dosage;
	int code[8];

}propertyStruct;

//function prototypes
bool yesOrNo();
int findDrugAngle(int drugType);
void startupMenu(propertyStruct & properties);
void dispenseDrug(int drugType, int dosage);
void resetCode(propertyStruct & properties);
void displayCodes(propertyStruct & properties);
void setQuantity(propertyStruct & properties);

bool yesOrNo()
{
	while (nNxtButtonPressed == -1){}
	if(nNxtButtonPressed == 2)
		return true;
	if(nNxtButtonPressed == 1)
		return false;
	else
		return false;
}



int findDrugAngle(int drugType)//a way to add current drug angle to the return
{
	if(drugType == 1)
		return 0;
	else if(drugType == 2)
		return 90;
	else if(drugType == 3)
		return 180;
	else
		return 270;
}

void startupMenu(propertyStruct & properties)
{
	displayString (0, "YES:Show Codes");
	displayString (1, "NO:Reset Codes");
	displayString (2, "Center:Set Amounts");
	displayString(4,"Touch 1:Exit");

	while (nNxtButtonPressed == -1 && SensorValue[S1] == 0){}

	eraseDisplay();

	if (nNxtButtonPressed == 2)
	{
		while(nNxtButtonPressed != -1){}
		displayCodes(properties);
	}

	else if (nNxtButtonPressed == 1)
	{
		while(nNxtButtonPressed != -1){}
		resetCode(properties);
	}

	else if (nNxtButtonPressed == 3)
	{
		while(nNxtButtonPressed != -1){}
		setQuantity(properties);
	}

	else if (SensorValue[S1] == 1){}

}

void dispenseDrug(int drugType, int dosage)
{
	nMotorEncoder[motorB] = 0;
	motor[motorB] = 5;
	while(nMotorEncoder[motorB] < findDrugAngle(drugType)){}
	motor[motorB] = 0;

	for(int i = 0; i < dosage; i++)
	{
		nMotorEncoder[motorA] = 0;
		motor[motorA] = 4;
		while(nMotorEncoder[motorA] < 180){}
		motor[motorA] = 0;
		wait1Msec(3000);
		motor[motorA] = 4;
		while(nMotorEncoder[motorA] < 360){}
		motor[motorA] = 0;
	}

	nMotorEncoder[motorB] = 0;
	motor[motorB] = 5;
	while(nMotorEncoder[motorB] < 360 - findDrugAngle(drugType) && findDrugAngle(drugType) != 0){}
	motor[motorB] = 0;
}

void resetCode(propertyStruct & properties)
{
	int count = 8;
	for (int i = 0; i < count; i++)
	{
		for (int j = 0; j < count; j++)
			properties.codeList[i][j] = abs(rand()% 2)+1;
	}
	displayString(0,"Reset Success");
	wait1Msec(2000);
	startupMenu(properties);
}

void displayCodes (propertyStruct & properties)
{
	int count = 8;
	for (int i = 0; i < count; i++)
	{
			displayString(i, "%d%d%d%d%d%d%d%d", properties.codeList[i][0],properties.codeList[i][1],properties.codeList[i][2],properties.codeList[i][3],
			properties.codeList[i][4],properties.codeList[i][5],properties.codeList[i][6],properties.codeList[i][7],);
	}

	while(nNxtButtonPressed != 1){}
	while(nNxtButtonPressed == 1){}
	eraseDisplay();
	startupMenu(properties);
}

void setQuantity (propertyStruct & properties)
{
	for (int i=0 ; i<4 ; i++)
	{
			displayString (0, "Set for drug %d",i+1);
			displayString(1,"yes for increase");
			displayString(2,"no for next");
			displayString(3,"Quantity: %d",properties.drugCount[i]);

			bool check = true;
			for (int j=0 ; j<5 && check == true; j++)
			{
				while(nNxtButtonPressed == -1){}
				if (nNxtButtonPressed == 2)
					if (properties.drugCount[i]<6)
						properties.drugCount[i]++;
					else
						check = false;
				else if(nNxtButtonPressed == 1)
					check = false;
				while(nNxtButtonPressed != -1){}
				displayString(3,"Quantity: %d",properties.drugCount[i]);
				wait1Msec(500);
			}
			eraseDisplay();
	}
	startupMenu(properties);
}


task main()
{

	//setting sensor types
	SensorType[S1] = sensorTouch;
	SensorType[S2] = sensorTouch;
	SensorType[S4] = sensorSONAR;
	SensorType[S3] = sensorColorNxtFULL;

	//instance variables
	propertyStruct properties; // stores values needed
	//setting values in properties struct


			//For testing sets arrays in memory
				for (int i=0 ; i<8 ; i++)
				{
					for (int j=0 ; j<8 ; j++)
					{
						properties.codeList[i][j] = 5;
					}
				}
			properties.codeList[0][0] = 2;
			//end of testing


	for(int i=0 ; i<4 ; i++)
	{
		properties.drugCount[i] = 0;
	}

	//for testing
	for (int i=0 ; i<4 ; i++)
	{
		properties.drugCount[i] = 1;
	}
	//can be set up in start up menu


	//Main Check Loop - drug count loop
	while (properties.drugCount[0]>0 && properties.drugCount[1]>0 && properties.drugCount[2]>0 && properties.drugCount[3]>0)
	{
		bool checkPoint1 = false;
		bool checkPoint2 = false;
		bool checkPoint3 = false;

		//Checkpoint 1 - customer presence
		do
		{
			while (SensorValue[S4] > 75)
			{
				displayString(0,"Searching");
			}
			time1[T1] = 0 ;
			while(SensorValue[S4] < 75 && time1[T1] < 1000){}
			if (time1[T1] < 1000)
			{
				checkPoint1 = false;
			}
			else
			{
				playSoundFile("Activate.rso");
				displayString(0,"Calibrating");
				playSoundFile("Calibrating.rso");
				checkPoint1 = true;
				time1[T1] = 0;
				while (nNxtButtonPressed != 3 && time1[T1] < 3000){}
				if (time1[T1] < 3000)
				{
					while(nNxtButtonPressed == 3){}
					startupMenu(properties);
					checkPoint1 = false;
				}
			}
			eraseDisplay();
		}while(checkPoint1 == false);

		//Checkpoint 2 - Color sense card
		do
		{
			displayString(0,"Hello!");
			displayString(1,"I am Baymax");
			displayString(2,"Place card");
			displayString(3,"Then press yes");

			while(nNxtButtonPressed != 2){}
			while(nNxtButtonPressed == 2){}

			eraseDisplay();

			displayString(0,"Scanning");

			//Scanning sequence
			properties.drugType = SensorValue[S3];
			motor[motorC] = -35;
			nMotorEncoder[motorC] = 0;
			while (nMotorEncoder[motorC] > -295){}
			motor[motorC] = 0;
			properties.dosage = SensorValue[S3];
			wait1Msec(1000);
			motor[motorC] = 35;
			while (nMotorEncoder[motorC] < 0){}
			motor[motorC] = 0;

			eraseDisplay();
			if (properties.dosage <= 4 && properties.drugType <=4)
			{
				displayString(0,"Your choices:");
				displayString(1,"Drug Type: %d",properties.drugType);

				if (properties.dosage == 1)
					displayString(2,"Dosage: %d Week",properties.dosage);
				else
					displayString(2,"Dosage: %d Weeks ",properties.dosage);

 				displayString(3,"Is This Correct?");

 				checkPoint2 = yesOrNo();
 				eraseDisplay();
 			}
 		}while(checkPoint2 == false);

 		//Checkpoint 3 - Code Input
		do
		{
			displayString(0,"Input your code");
			displayString(1,"Enter on prompt");


			for (int k=0 ; k<8 ; k++)
			{

				displayString(3,"Enter digit %d",k+1);
				while (SensorValue[S1] !=1 && SensorValue[S2] != 1){}
				if (SensorValue[S1] == 1)
					properties.code[k] = 1;
				else if (SensorValue[S2] == 1)
					properties.code[k] = 2;
				while (SensorValue[S1] != 0 || SensorValue[S2] != 0){}
				wait1Msec(500);
				eraseDisplay();
			}

			//for testing entered code through
			bool check = false;
			for(int j=0 ; j<8 && check == false ; j++)
			{
				check = true;
				for (int k=0 ; k<8 ; k++)
				{
					if (properties.code[k] != properties.codeList[j][k]) //even if it doesnt match one thing check will equal false
					check = false;
				}
			}

			if (check == false)
			{
				displayString(0,"Non-existent Code");
				wait1Msec(3000);
			}
			if(check == true)
			{
				displayString(0,"Your Code:");
				displayString(1, "%d%d%d%d%d%d%d%d", properties.code[0],properties.code[1],properties.code[2],properties.code[3],
				properties.code[4],properties.code[5],properties.code[6],properties.code[7],);
				displayString(2,"Is This Correct?");
				playSoundFile("Transmitted.rso");
				while(bSoundActive){}
				for (int i=0 ; i<8 ; i++)
				{
					if (properties.code[i] == 1)
						playSoundFile("1.rso");
					else if (properties.code[i] == 2)
						playSoundFile("2.rso");
				}
				checkPoint3 = yesOrNo();
			}
			eraseDisplay();
		}while(checkPoint3 == false);

		displayString(0,"Dispensing");
		playSoundFile("Unloading.rso");
		dispenseDrug(properties.drugType,properties.dosage);
		properties.drugCount[properties.drugType] -= properties.dosage;
		eraseDisplay();
	}

}
