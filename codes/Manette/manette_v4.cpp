// File: manette_v3.cpp
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
#define PAS_VCAP 5		// en %

/***************************************************************************************************/

int directionShift(char commande, int alpha_cap);
int direction(char commande, int alpha_cap);
int velocity(char commande, int v_cap);
void sendtrameDigimeshFirst();

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
	char stop_data = 0xFF;

	int alpha_add = 0;
	int v_cap_add = 0;

    bool shiftButton =0;
	/***************************************************************************************************/

	
	/**	Serial port definition **/
	/***************************************************************************************************/
	#ifdef ceWINDOWS
		ceSerial com("\\\\.\\COM4",9600,8,'N',1); // Windows
	#endif
	/***************************************************************************************************/


	/**	Open the Serial Port COMX **/
	/***************************************************************************************************/
	printf("Opening port %s.\n",com.GetPort().c_str());
	if (com.Open() == 0) 
	{
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
        
        if(commande == 'p')
        {
            shiftButton = 1;
            commande = getch();
        }
        else
        {
            shiftButton =0;
        }

        
		/**	direction control **/
		/***************************************************************************************************/
		if ((commande == 'z') || (commande == 'q') || (commande == 's') || (commande == 'd'))
		{

            if(shiftButton == 1)
            {
                alpha_add = directionShift(commande, alpha_cap);
                alpha_cap = alpha_cap + alpha_add;
            }
            else
            {
                alpha_cap = direction(commande, alpha_cap);
            }

			printf("angle : %d deg\n",alpha_cap);

		}
		else if ((commande == 'o') || (commande == 'l'))
		{
			v_cap_add = velocity(commande,v_cap);
			v_cap = v_cap + v_cap_add;
		}
		else if (commande == 'b')
		{
			printf("\n\n!Good bye!\n\n");
			stop = 1;
		}



		/**	Send Data **/
		/***************************************************************************************************/
		
		
		
		/***************************************************************************************************/

		usleep(100*1000);

	}


	/**	Reset command before closing **/
	/***************************************************************************************************/

	/***************************************************************************************************/


	/**	Close Serial port **/
	/***************************************************************************************************/
	printf("Closing port %s.\n",com.GetPort().c_str());
	com.Close();
	/***************************************************************************************************/

    return 0;
}




/**	Functions **/
/***************************************************************************************************/

int directionShift(char commande, int alpha_cap)
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

int direction(char commande, int alpha_cap)
{
	/** PRESS Z **/
	if (commande == 'z')
	{
		return 0;
	}

	/** PRESS Q **/
	else if (commande == 'q')
	{
		return 90;
	}

	/** PRESS S **/
	else if (commande == 's')
	{
		return 180;
	}

	/** PRESS D **/
	else if (commande == 'd')
	{
		return 270;
    }
    else{
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

void sendtrameDigimesh(int alpha_cap, int v_cap)
{
	int alpha_u, alpha_d, alpha_c;
	int v_u, v_d,v_c;
	com.WriteChar(0x7E);	//Start delimiter
	com.WriteChar(0x00);	//Length MSB
	com.WriteChar(0x17);	//length LSB
	com.WriteChar(0x10);	//Frame type
	com.WriteChar(0x01);	//frame ID

	//destination 64 bits
	com.WriteChar(0x00);
	com.WriteChar(0x13);
	com.WriteChar(0xA2);
	com.WriteChar(0x00);
	com.WriteChar(0x41);
	com.WriteChar(0xC8);
	com.WriteChar(0x89);
	com.WriteChar(0x90);

	com.WriteChar(0xFF);	//destination 16 bits MSB
	com.WriteChar(0xFE);	//destination 16 bits LSB

	com.WriteChar(0x00);	//broadcast radius
	com.WriteChar(0x00);	//option

	//data
	alpha_c = alpha_cap%100;
	alpha_d = alpha_cap%10;
	alpha_u = alpha_cap%1;

	v_c = v_cap%100;
	v_d = v_cap%10;
	v_u = v_cap%1;

	com.WriteChar('@');
	com.WriteChar(0x30+alpha_c);
	com.WriteChar(0x30+alpha_d);
	com.WriteChar(0x30+alpha_u);
	com.WriteChar('#');
	com.WriteChar(0x30+v_c);
	com.WriteChar(0x30+v_d);
	com.WriteChar(0x30+v_u);
	com.WriteChar('#');

	//Checksum
		//add all bits of the packets excluding the start and lenght
		//just save last 8bits
		//Subtract from 0xFF

	com.WriteChar(0x00);
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