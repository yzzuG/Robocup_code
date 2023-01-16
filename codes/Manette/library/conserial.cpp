// File: main.cpp
// Description: Serial communication console program for Windows 
// Copyright (c) 2023 Armand Guzzonato

#include <stdio.h>
#include "ceSerial.h"
using namespace std;
using namespace ce;

int main()
{
#ifdef ceWINDOWS
	ceSerial com("\\\\.\\COM4",9600,8,'N',1); // Windows
#endif

	printf("Opening port %s.\n",com.GetPort().c_str());
	if (com.Open() == 0) {
		printf("OK.\n");
	}
	else {
		printf("Error.\n");
		return 1;
	}

	bool successFlag;
	printf("Writing.\n");
	char s[]="Hello";
	successFlag=com.Write(s); // write string
	successFlag=com.WriteChar('!'); // write a character

	printf("Waiting 3 seconds.\n");
	ceSerial::Delay(3000); // delay to wait for a character

	printf("Reading.\n");
	char c=com.ReadChar(successFlag); // read a char
	if(successFlag) printf("Rx: %c\n",c);

	printf("Closing port %s.\n",com.GetPort().c_str());
	com.Close();

	return 0;
}
