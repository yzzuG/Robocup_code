/********************************************************/
/*    Bibliotheques    */
/********************************************************/

#include <math.h>

/********************************************************/
/*    Variables globales    */
/********************************************************/

//  moteur + roue
#DEFINE C_RES_MOTEUR 0
#DEFINE K_COURANT_MOTEUR 0
#DEFINE K_VITESSE_MOTEUR 0
#DEFINE R_MOTEUR 0
#DEFINE J_EQ 0
#DEFINE RAYON_ROUE 0


/********************************************************/
/*    Variables    */
/********************************************************/
double u1,u2,u3,u4;
double sens_m1 = 0, sens_m3 = 0;
double sens_m2 = 1, sens_m4 = 1;
//double v1, v2, v3, v4;
double omega1, omega2, omega3, omega4;

// donnees dentree
double v_cap ;  // norme du cap m/s

double alpha = 0 ;   // angle du cap dans le repere robot en degree


/********************************************************/
/*    Main    */
/********************************************************/
void main(void)
{
  omega1 = (1/RAYON_ROUE)* v_cap * cos((45-alpha)*PI/180);
  u1 = R_MOTEUR/K_COURANT_MOTEUR * ((J_EQ + K_COURANT_MOTEUR*K_VITESSE_MOTEUR/R_MOTEUR - C_RES_MOTEUR)* omega1);

  omega2 = (1/RAYON_ROUE)* v_cap * cos((alpha+45)*PI/180);
  u2 = R_MOTEUR/K_COURANT_MOTEUR * ((J_EQ + K_COURANT_MOTEUR*K_VITESSE_MOTEUR/R_MOTEUR - C_RES_MOTEUR)* omega2);

  omega3 = (1/RAYON_ROUE)* v_cap * cos((45-alpha)*PI/180);
  u3 = R_MOTEUR/K_COURANT_MOTEUR * ((J_EQ + K_COURANT_MOTEUR*K_VITESSE_MOTEUR/R_MOTEUR - C_RES_MOTEUR)* omega3);

  omega4 = (1/RAYON_ROUE)* v_cap * cos((alpha+45)*PI/180);
  u4 = R_MOTEUR/K_COURANT_MOTEUR * ((J_EQ + K_COURANT_MOTEUR*K_VITESSE_MOTEUR/R_MOTEUR - C_RES_MOTEUR)* omega4);
}
