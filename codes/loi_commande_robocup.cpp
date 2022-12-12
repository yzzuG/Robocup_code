/********************************************************/
/*    Bibliotheques    */
/********************************************************/

#include <math.h>

/********************************************************/
/*    Global constants   */
/********************************************************/

//  moteur + roue
#DEFINE C_RES_MOTEUR 0
#DEFINE K_COURANT_MOTEUR 0
#DEFINE K_VITESSE_MOTEUR 0
#DEFINE R_MOTEUR 0
#DEFINE J_EQ 0
#DEFINE RAYON_ROUE 0


/********************************************************/
/*    Global variables    */
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



  while(true)
  {
    // ENTREE
    /********************************************************/
    // Reception du maitre

    /********************************************************/


    //CALCULS
    /********************************************************/
    //Calculs des tensions a delivree aux moteurs
    ///////////////////moteur 1
    u1 = tension_moteur_impair(alpha, v_cap);
    rapport1 = map(abs(u1), 0, 5, 0, 255);

    ///////////////////moteur 2
    u2 = tension_moteur_pair(alpha, v_cap);
    rapport2 = map(abs(u2), 0, 5, 0, 255);

    ///////////////////moteur 3
    u3 = tension_moteur_impair(alpha, v_cap);
    rapport3 = map(abs(u3), 0, 5, 0, 255);

    ///////////////////moteur 4
    u4 = tension_moteur_pair(alpha, v_cap);
    rapport4 = map(abs(u4), 0, 5, 0, 255);
    /********************************************************/
    //Determination du sens de rotation
    sens_m1 = sens_moteur(u1,alpha,1);
    sens_m2 = sens_moteur(u2,alpha,2);
    sens_m3 = sens_moteur(u3,alpha,3);
    sens_m4 = sens_moteur(u4,alpha,4);

    /********************************************************/


    //SORTIE
    /********************************************************/

    analogWrite(PIN_U_M1, rapport1);
    analogWrite(PIN_U_M2, rapport2);
    analogWrite(PIN_U_M3, rapport3);
    analogWrite(PIN_U_M4, rapport4);

    digitalWrite(PIN_SENS_M1, sens_m1);
    digitalWrite(PIN_SENS_M2, sens_m2);
    digitalWrite(PIN_SENS_M3, sens_m3);
    digitalWrite(PIN_SENS_M4, sens_m4);

    /********************************************************/
  }
}

/********************************************************/
/*  Fonction : tension_moteur_impair()                  */
/********************************************************/
/*  Variables d'entree :                                */
/*  alpha : type double                                 */
/*      angle du vecteur cap dans le repere robot       */
/*  v_cap : type double                                 */
/*      norme du vecteur cap                            */
/********************************************************/
/*  Variable de sortie :                                */
/*  u : type double                                     */
/*      valeur de tension appliquée au moteur           */
/********************************************************/
double tension_moteur_impair(double alpha, double v_cap)
{
  double omega, u;
  int signe = 1;

  omega = (1 / RAYON_ROUE) * v_cap * cos((45 - alpha) * PI / 180) * REDUCTION_ROUE;
  u = omega / SCALING;

  //protection
  if (u < 0)
  {
    signe = -1;
  }
  if (abs(u) > U_MAX)
  {
    u = signe * U_MAX;
  }
  else if ((abs(u) > 0.17) && (abs(u) < 0.33))
  {
    u = 0.25 * signe;
  }

  return u;
};



/********************************************************/
/*  Fonction : tension_moteur_pair()                  */
/********************************************************/
/*  Variables d'entree :                                */
/*  alpha : type double                                 */
/*      angle du vecteur cap dans le repere robot       */
/*  v_cap : type double                                 */
/*      norme du vecteur cap                            */
/********************************************************/
/*  Variable de sortie :                                */
/*  u : type double                                     */
/*      valeur de tension appliquée au moteur           */
/********************************************************/
double tension_moteur_pair(double alpha, double v_cap)
{
  double omega, u;
  int signe = 1;

  omega = (1 / RAYON_ROUE) * v_cap * cos((45 + alpha) * PI / 180) * REDUCTION_ROUE;
  u = omega / SCALING;

  if (u < 0)
  {
    signe = -1;
  }
  if (abs(u) > U_MAX)
  {
    u = signe * U_MAX;
  }
  else if ((abs(u) > 0.17) && (abs(u) < 0.33))
  {
    u = 0.25 * signe;
  }

  return u;
};
