#include <iostream>
#include <string> 
#include <stdio.h>
#include <iomanip>
#include <conio.h>
#include <unistd.h>
#include "Serial.h"



using namespace std;

#define XB_START_DELIMITER        0x7E
#define XB_TX_REQ_FRAME           0x10
#define XB_BROADCAST_ADD64        ((unsigned long long)0x000000000000FFFF)
#define XB_CIBLE_1                ((unsigned long long)0x0013A20041C88990)
#define XB_CIBLE_2                ((unsigned long long)0x0013A20040DA7CA1)
#define XB_FRAME_ID               0x01
#define XB_TX_OPTIONS             0x00
#define XB_BROADCAST_RADIUS       0x00

/** Globals variables **/
/***************************************************************************************************/
// Moteurs

#define PAS_DEG 5		// en deg
#define PAS_VCAP 5		// en %


// Communication
tstring commPortName(TEXT("COM1"));
Serial serial(commPortName, 57600);

const int taille = 12 ; 
char xb_tx_packet[256];
char data_to_send[32] ; 
char xb_checksum ; 
int k ; 
unsigned short xb_frame_length = 0;

// ---------------------------------------------------------------------
//         ENVOI D'UNE TRAME XBEE TX Request (DigiMesh 2.4)
// ---------------------------------------------------------------------
//     data = tableau contenant les données à envoyer
//     length = nombre d'octets à envoyer
//     dest64 = adresse xbee (64b) du destinataire
// ---------------------------------------------------------------------
void xb_send_data(char* data, unsigned short length, unsigned long long dest64)
{
	xb_frame_length = 14 + length;

	xb_tx_packet[0] = XB_START_DELIMITER;
	xb_tx_packet[1] = (char)(xb_frame_length >> 8);
	xb_tx_packet[2] = (char)xb_frame_length;
	xb_tx_packet[3] = XB_TX_REQ_FRAME;
	xb_tx_packet[4] = XB_FRAME_ID;
	xb_tx_packet[5] = (char)(dest64 >> 56);
	xb_tx_packet[6] = (char)(dest64 >> 48);
	xb_tx_packet[7] = (char)(dest64 >> 40);
	xb_tx_packet[8] = (char)(dest64 >> 32);
	xb_tx_packet[9] = (char)(dest64 >> 24);
	xb_tx_packet[10] = (char)(dest64 >> 16);
	xb_tx_packet[11] = (char)(dest64 >> 8);
	xb_tx_packet[12] = (char)dest64;
	xb_tx_packet[13] = 0xFF;
	xb_tx_packet[14] = 0xFE;
	xb_tx_packet[15] = XB_BROADCAST_RADIUS;
	xb_tx_packet[16] = XB_TX_OPTIONS;

	for ( k = 0; k < length ;k++)
	{
		xb_tx_packet[17 + k] = data[k];
	}
	
	xb_checksum = 255;
	for (k = 3;k < (xb_frame_length + 3);k++)
	{
		xb_checksum -= xb_tx_packet[k];
	}

	xb_tx_packet[17 + length] = xb_checksum;

	serial.write(xb_tx_packet, xb_frame_length + 4);

	for(int i =0 ; i< 17+length ; i++)
		{
			int a = xb_tx_packet[i] ; 
		}
	
	serial.write(data, length);
}

// ---------------------------------------------------------------------
//         CONTROLE MANETTE
// ---------------------------------------------------------------------
//         Direction par 5 degrés
// ---------------------------------------------------------------------
//     commande = touche appuyée
//    alpha_cap = angle en degrés
// ---------------------------------------------------------------------

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

// ---------------------------------------------------------------------
//         Direction générale par axes 
// ---------------------------------------------------------------------
//     commande = touche appuyée
//    alpha_cap = angle en degrés
// ---------------------------------------------------------------------

int direction(char commande, int alpha_cap)
{
	/** PRESS Z **/
	if (commande == 'z')
	{
		return 0;
	}

	/** PRESS Q **/
	else if (commande == 'd')
	{
		return 90;
	}

	/** PRESS S **/
	else if (commande == 's')
	{
		return 180;
	}

	/** PRESS D **/
	else if (commande == 'q')
	{
		return 270;
    }
    else{
        return 0;
    }
};

// ---------------------------------------------------------------------
//         Direction générale par axes 
// ---------------------------------------------------------------------
//     commande = touche appuyée
//    v_cap = angle en degrés
// ---------------------------------------------------------------------

int velocity(char commande, int v_cap)
{
	if(commande == 'o')
	{
		if (v_cap <= 95)
		{
			return PAS_VCAP;
		}
		if (v_cap <= 100)
		{
			return 0;
		}
		else
		{
			return 0;
		}
	}
	else if (commande == 'l')
	{
		if (v_cap >= 5)
		{
			return -PAS_VCAP;
		}
		else if (v_cap > 0)
		{
			return 0;
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

// ---------------------------------------------------------------------
//        Fonction affichage de la manette
// ---------------------------------------------------------------------
void readme(void)
{
	printf("\n");
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

// ---------------------------------------------------------------------
//         Création de la trame de data
// ---------------------------------------------------------------------
//     commande = touche appuyée
//    v_cap = angle en degrés
// ---------------------------------------------------------------------
/*
char * trame_to_send(int v_cap , int alpha_cap)
{
		// ------------------------ Création de la trame de data -------------------------------- 
	
	char trame[9] ;  
	// Début 
	trame[0] = '@' ;

	// Envoie de la vitesse
	if (v_cap>=100)
    {
        trame[3] = v_cap%10 ; 
        alpha_cap = v_cap/10 ;
        trame[2] = v_cap%10 ; 
        trame[1]= v_cap/10 ;
    }
    else if (v_cap<100 && v_cap>=10 )
    {
        trame[3] = v_cap%10 ; 
        alpha_cap = v_cap/10 ;
        trame[2] = v_cap%10 ; 
    }
	else
	{
		trame[3] = v_cap ; 
	}

	// Séparateur 

	trame[4] = '#' ;

	// Envoie de l'angle

	    if (alpha_cap>=100)
    {
        trame[7] = alpha_cap%10 ; 
        alpha_cap = alpha_cap/10 ;
        trame[6] = alpha_cap%10 ; 
        trame[5]= alpha_cap/10 ;
    }
    else if (alpha_cap<100 && alpha_cap>=10 )
    {
        trame[7] = alpha_cap%10 ; 
        alpha_cap = alpha_cap/10 ;
        trame[6] = alpha_cap%10 ; 
    }
	else
	{
		trame[7] = alpha_cap ; 
	}
	// Séparateur 

	trame[8] = '#' ;

	return data_to_send ;
}

*/
int main()
{
	/** Variables **/
	
	char commande;
	string phrase = "";

	int v_cap = 0;			// en %
	int alpha_cap = 0;		// en degree

	int alpha_add = 0;
	int v_cap_add = 0;

    bool shiftButton =0;
	bool stop = 0 ; 

    std::cout << " Lancement du programme " << std::endl;
	printf("Start conditions :      ");
	printf("angle : %d",alpha_cap);
	printf("  		v_cap : %d\n",v_cap);

	readme() ; 
	
	while (stop==0)
	{
		
		commande = getch();
        
        if(commande == 'p' && shiftButton == 0)
        {
            shiftButton = 1 ;
            commande = getch();
        }
		else if(commande == 'p' && shiftButton == 1)
        {
            shiftButton = 0 ;
            commande = getch();
        }
		
       printf("p = %d \n", shiftButton) ; 
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
		}
		else if ((commande == 'o') || (commande == 'l'))
		{
			v_cap_add = velocity(commande,v_cap);
			v_cap = v_cap + v_cap_add;
		}
		else if (commande == 'b')
		{
			printf("\n\n!Good bye!\n\n");
			v_cap = 0 ; 
			alpha_cap = 0 ; 
			stop = 1;
		}

		printf("Nouvelle valeur d'alpha = %d \n" , alpha_cap) ; 
		printf("Nouvelle valeur de la vitesse = %d \n", v_cap) ; 
		printf("\n") ;  

		// ------------------------ Création de la trame de data -------------------------------- 
		// Variables nécéssaires 

		int v_modif = v_cap ;  
		int alpha_modif = alpha_cap ; 
		
		// Remise à 0 de la trame à envoyer

		for (int i = 0 ; i < 9 ; i++)
		{
			data_to_send[i] = 48 ; // 48 étant la valeur de 0 en hexa
		}

		// Début 
		data_to_send[0] = '@' ;

		// Envoie de la vitesse
		if (v_modif>=100)
    	{
        	data_to_send[3] = v_modif%10 + 48 ; 
        	v_modif = v_modif/10 ;
        	data_to_send[2] = v_modif%10 + 48 ; 
        	data_to_send[1]= v_modif/10 + 48 ;
    	}
    	else if (v_modif<100 && v_modif>=10 )
    	{
        	data_to_send[3] = v_modif%10 + 48; 
        	v_modif = v_modif/10;
        	data_to_send[2] = v_modif%10 + 48 ; 
    	}
		else
		{
			data_to_send[3] = v_modif + 48; 
		}

		// Séparateur 

		data_to_send[4] = '#' ;

		// Envoie de l'angle

	    if (alpha_modif>=100)
    	{
        	data_to_send[7] = alpha_modif%10 + 48; 
        	alpha_modif = alpha_modif/10 ;
        	data_to_send[6] = alpha_modif%10 + 48;
        	data_to_send[5]= alpha_modif/10 + 48;
    	}
    	else if (alpha_modif<100 && alpha_modif>=10 )
    	{
        	data_to_send[7] = alpha_modif%10 + 48; 
        	alpha_modif = alpha_modif/10 ;
        	data_to_send[6] = alpha_modif%10 + 48; 
    	}
		else
		{
			data_to_send[7] = alpha_modif + 48; 
		}

		// Séparateur / Fin

		data_to_send[8] = '#' ;

		// Affichage de la trame de données 

		for(int i =0 ; i<9 ; i++)
		{
			int a = data_to_send[i] ; 
		}
		xb_send_data(data_to_send,9,XB_CIBLE_1) ; 

	}
}
