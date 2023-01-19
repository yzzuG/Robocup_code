// File: manette_v2.cpp
// Description: Serial communication console program for Windows 
// Copyright (c) 2023 Armand Guzzonato

#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <conio.h>
#include <unistd.h>
#include "library\ceSerial.h"

using namespace std;
using namespace ce;

/** Globals variables **/
/***************************************************************************************************/

#define PAS_DEG 5		// en deg
#define PAS_VCAP 10		// en %

/***************************************************************************************************/

int direction(char commande, int alpha_cap);
int velocity(char commande, int v_cap);
void Readme(void);




int main() {


	/** Variables **/
	/***************************************************************************************************/
	bool successFlag;

	char commande;
	string phrase = "";

	bool stop = 0;

	int v_cap = 0;			// en %
	int alpha_cap = 0;		// en degree
	unsigned char alpha_cap_uart;
	unsigned char alpha_cap_uart_signe;
	unsigned char v_cap_uart;
	char dir_data = 0xFB;
	char vel_data = 0XFC;
	char data_id;

	int alpha_add = 0;
	int v_cap_add = 0;
	/***************************************************************************************************/

	
	/**	Serial port definition **/
	/***************************************************************************************************/
	#ifdef ceWINDOWS
		ceSerial com("\\\\.\\COM4",57600,8,'N',1); // Windows
	#endif
	/***************************************************************************************************/


	/**	Open the Serial Port COMX **/
	/***************************************************************************************************/
	printf("Opening port %s.\n",com.GetPort().c_str());
	if (com.Open() == 0) 
	{
		/*for (int i =0; i<10; i++)
		{
			printf("#");
			usleep(500*1000);
		}*/
		printf("\nOK.\n\n\n");
		usleep(1000*1000);
	}
	else 
	{
		printf("Error.\n");
		return 1;
	}
	/***************************************************************************************************/
	

	/** Tips **/
	/***************************************************************************************************/
	Readme();
	commande = getch();

	printf("\n\n\n\n Start conditions :      ");
	printf("angle : %d",alpha_cap);
	printf("  		v_cap : %d\n",v_cap);
	/***************************************************************************************************/


	/**	loop **/
	/***************************************************************************************************/
	while (stop == 0)
	{
		commande = getch();


		/**	direction control **/
		/***************************************************************************************************/
		if ((commande == 'z') || (commande == 'q') || (commande == 's') || (commande == 'd'))
		{
			alpha_add = direction(commande, alpha_cap);
			alpha_cap = alpha_cap + alpha_add;

			data_id = dir_data;
			alpha_cap_uart = (unsigned char) (abs(alpha_cap));


			if (alpha_cap < 0)
			{
				alpha_cap_uart_signe = 0x02;		// signe moins
			}
			else if(alpha_cap == 0)
			{
				alpha_cap_uart = 0x04;
				alpha_cap_uart_signe = 0x01;		// signe plus
			}
			else 
			{
				alpha_cap_uart_signe = 0x01;		// signe plus
			}

			/*successFlag = SendData(0xFB,alpha_cap_uart,alpha_cap_uart_signe);
			
			
			printf("0x%02X %02X           ",(char)alpha_cap_uart_signe,alpha_cap_uart);
			printf("angle : %d deg    | valeur en hexa :  0x%04X\n",alpha_cap,alpha_cap);*/
			printf("angle : %d deg\n",alpha_cap);

		}
		else if ((commande == 'o') || (commande == 'l'))
		{
			v_cap_add = velocity(commande,v_cap);
			v_cap = v_cap + v_cap_add;

			data_id = vel_data;

			v_cap_uart = (unsigned char) (abs(v_cap));
			if(v_cap == 0)
			{
				v_cap_uart = 0x04;
			}

			printf("v_cap : %d %\n",v_cap);
		}
		else if (commande == 'b')
		{
			printf("\n\n!Good bye!\n\n");
			stop = 1;
		}



		/**	Send Data **/
		/***************************************************************************************************/
		if (data_id == dir_data)
		{
			successFlag=com.WriteChar(data_id);
			successFlag=com.WriteChar(alpha_cap_uart_signe);
			successFlag=com.WriteChar(alpha_cap_uart);
		}
		else if (data_id == vel_data)
		{
			successFlag=com.WriteChar(data_id);
			successFlag=com.WriteChar(0x01);
			successFlag=com.WriteChar(v_cap_uart);
		}
		/***************************************************************************************************/

		usleep(100000);

	}


	/**	Close Serial port **/
	/***************************************************************************************************/
	printf("Closing port %s.\n",com.GetPort().c_str());
	com.Close();
	/***************************************************************************************************/

    return 0;
}





/**	Functions **/
/***************************************************************************************************/

int direction(char commande, int alpha_cap)
{
	/** PRESS Z **/
	if (commande == 'z')
	{
		if ((alpha_cap > 0) && (alpha_cap <= 90))
		{
			return -PAS_DEG;
		}
		else if ((alpha_cap < 0) && (alpha_cap >= -90))
		{
			return PAS_DEG;
		}
		else
		{
			return 0;
		}
	}

	/** PRESS Q **/
	else if (commande == 'q')
	{
		if ((alpha_cap < -90) && (alpha_cap >= -180))
		{
			return PAS_DEG;
		}
		else if ((alpha_cap <= 0) && (alpha_cap > -90))
		{
			return -PAS_DEG;
		}
		else if (alpha_cap == 180)
		{
			return -360;
		}
		else
		{
			return 0;
		}
	}

	/** PRESS S **/
	else if (commande == 's')
	{
		if ((alpha_cap > -180) && (alpha_cap <= -90))
		{
			return -PAS_DEG;
		}
		else if ((alpha_cap < 180) && (alpha_cap >= 90))
		{
			return PAS_DEG;
		}
		else
		{
			return 0;
		}
	}

	/** PRESS D **/
	else if (commande == 'd')
	{
		if ((alpha_cap >= 0) && (alpha_cap < 90))
		{
			return PAS_DEG;
		}
		else if ((alpha_cap <= 180) && (alpha_cap > 90))
		{
			return -PAS_DEG;
		}
		else if (alpha_cap == -180)
		{
			return 360;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
};

int velocity(char commande, int v_cap)
{
	if(commande == 'o')
	{
		if (v_cap < 100)
		{
			return PAS_VCAP;
		}
		else
		{
			return 0;
		}
	}
	else if (commande == 'l')
	{
		if (v_cap > 0)
		{
			return -PAS_VCAP;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}	
};

void Readme(void)
{
	printf("************************	Tips	***********************************************\n");
	printf("                        _____                                   _____\n");
	printf("                       |     |                                 |     |\n");
	printf("                 0 deg |  z  |                                 |  O  |v_cap up\n");
	printf("                       |_____|                                 |_____|\n");
	printf("        _____           _____            _____                  _____\n");
	printf("       |     |         |     |          |     |                |     |\n");
	printf("-90 deg|  q  |         |  s  |180 deg   |  d  |90 deg          |  L  |v_cap down\n");
	printf("       |_____|         |_____|          |_____|                |_____|\n\n");
	printf(" Press b button if you want to stop this program.\n");
	printf("***********************************************************************************\n");
	printf("Press any button of keybord");

}