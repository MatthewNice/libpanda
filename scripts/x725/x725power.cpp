/*
 Author: Matt Bunting
 Copyright (c) 2020 Arizona Board of Regents
 All rights reserved.

 Permission is hereby granted, without written agreement and without
 license or royalty fees, to use, copy, modify, and distribute this
 software and its documentation for any purpose, provided that the
 above copyright notice and the following two paragraphs appear in
 all copies of this software.

 IN NO EVENT SHALL THE ARIZONA BOARD OF REGENTS BE LIABLE TO ANY PARTY
 FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 IF THE UNIVERSITY OF CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF
 SUCH DAMAGE.

 THE ARIZONA BOARD OF REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE PROVIDED HEREUNDER
 IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION
 TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// for i2c:
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#define CHARGED_THRESHOLD (0.1) // in "amps" to determine range

#define x725_ADDRESS (0x36)

#define CMD_VOLTAGE
#define CMD_CAPACITY
#define CMD_CURRENT	// unsure if this is current, seems to roughly correspond

typedef enum {
	CHARGING,
	DISCHARGING,
	CHARGED
} BatteryState;

int i2c_write_byte( int i2cFile, unsigned char cmd, unsigned char value) {
	unsigned char buffer[2];

	// Two bytes must be written, first the command (register), then the value
	buffer[0] = cmd;
	buffer[1] = value;

	int length = 2;
	if( write(i2cFile, buffer, length) != length )
	{
		printf("Failed to write to the i2c\n");
	}

	return 0;
}

int i2c_read_word( int i2cFile, unsigned char cmd) {
	unsigned char buffer[2];

	// First we need to write a byte for the register to be read
	buffer[0] = cmd;
	int length = 1;			//<<< Number of bytes to write
	if (write(i2cFile, buffer, length) != length)
	{
		printf("Failed to write to i2c\n");
	}

	// Then we can begin reading from that register onward
	length = 2;
	if (read(i2cFile, buffer, length) != length)
	{
		printf("Failed to read from i2c\n");
	}

	int result = (((int)buffer[0])<<8) + (int)buffer[1];

	return result;
}



int main(int argc, char **argv) {
	printf("Running %s\n", argv[0]);

	// if a single argument is passed, it will be the name of the script to be run prior to system shutdown.
	char scriptToRunBeforeShutdown[] = "scriptToRunBeforeShutdown";
	char* exitScript = scriptToRunBeforeShutdown;
	if (argc == 2) {
		exitScript = argv[1];
	}

	printf("The command \'%s\' will be run on exit\n", exitScript);

	// This opens the raspberry pi device file:
	char i2cFilename[] = "/dev/i2c-1";
	int i2cFile;
	if ((i2cFile = open(i2cFilename, O_RDWR)) < 0)
	{
		printf("Unable to open the i2c file");
		return -1;
	}

	if (ioctl(i2cFile, I2C_SLAVE, x725_ADDRESS) < 0)
	{
		printf("Unable ot talk to x725 i2c address\n");
		return -1;
	}

	bool keepRunning = true;

	float voltage, capacity, current;
	int result;

	BatteryState batteryState;

	// For forking into an exit script that can be killed
	pid_t  exitScriptPid = -1;

	while(keepRunning) {
		sleep(5); // only operate once every 5 seconds

		// Read voltage:
		result = i2c_read_word(i2cFile, 2);
		voltage = ((float)result) * 1.25 /1000/16;
		printf("Voltage  = %7.5fV\n", voltage);

		// Read capacity:
		result = i2c_read_word(i2cFile, 4);
		capacity = result / 256.0;
		printf("Capacity = %5.2f%%\n", capacity);

		// Read Current:
		// I am unsure if this actually corresponds to current
		result = i2c_read_word(i2cFile, 20);
		if(result & 0x8000) {
			current = 2.0/1000.0 * (float)((result & 0x7FFF) - 0x8000);
		} else {
			current = 2.0/1000.0 * (float)(result & 0x7FFF);
		}
		printf("Batt Current = %0.3f\n", current);

		printf("Chargin state: %s\n",
			   current > CHARGED_THRESHOLD ? "Charging" :
			   current < -CHARGED_THRESHOLD ? "Discharging" :
			   "Charged" );

		if ((current < CHARGED_THRESHOLD ) && (current > -CHARGED_THRESHOLD )) {
			batteryState = CHARGED;
		} else if ( current < 0 ) {
			batteryState = DISCHARGING;
		} else {
			batteryState = CHARGING;
		}

//		for(int i = 0; i < 2; i+=2) {
//			result = i2c_read_word(i2cFile, i);
//			printf("cmd %d = %03d %03d\n", i, result & 0xFF, result>>8);
//		}
//		for(int i = 6; i < 18; i+=2) {
//			result = i2c_read_word(i2cFile, i);
//			printf("cmd %d = %03d %03d\n", i, result & 0xFF, result>>8);
//		}


		if ( capacity <= 0 ) {
			printf("Batteries are drained!  Shutting Down...\n");
			keepRunning = false;
		}

		if ( voltage < 3.2 ) {
			printf("Battery voltage is low!  Shutting Down...\n");
			keepRunning = false;
		}

		if ( batteryState == DISCHARGING ) {
			printf("External Power loss!  Running upload script...\n");


//			exitScriptPid = fork();
//			if (exitScriptPid == 0) {
//				// child process
//			} else {
				// parent process
				system(exitScript);
				keepRunning = false;
//			}
			// This is blocking call until the exit script

			printf("Shutting Down...\n");

		}
	}

//	kill(exitScriptPid, SIGKILL);

	system("x725shutdown");

	return 0;
}
