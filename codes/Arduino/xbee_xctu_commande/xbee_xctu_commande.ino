//*----------------------------------------------------------------------------
//*  Projet Robocup
//*----------------------------------------------------------------------------
//* File Name : commande_base_xbee.ino
//* Object :  wheels engine control (speed and direction of rotation)
//* Creation : Max CHANTREAU/Marius CROZET/Armand GUZZONATO  12/12/2022
//* Modification : Armand Guzzonato 12/12/2022
//*----------------------------------------------------------------------------

/********************************************************/
/*    Bibliotheques    */
/********************************************************/

#include <math.h>



/********************************************************/
/*    Global constants     */
/********************************************************/
//definition des pins
//moteur 1 (devant droit)
#define PIN_U_M1        2
#define PIN_SENS_M1     22
//moteur 2 (devant gauche)
#define PIN_U_M2        3
#define PIN_SENS_M2     23
//moteur 3 (derriere gauche)
#define PIN_U_M3        4
#define PIN_SENS_M3     24
//moteur 4 (derriere droit)
#define PIN_U_M4        5
#define PIN_SENS_M4     25

//  moteur + roue
//rapport de reduction
#define REDUCTION_ROUE  2.75
//rayon roue en m
#define RAYON_ROUE      0.05
//coef de prop du moteur en rad/s/V
#define SCALING         62.8

//sens de rotation
#define CCW             1
#define CW              0

//tension maximum en V
#define U_MAX           3.3
//vitesse de cap maximum en m/s
#define V_CAP_MAX       3.5

/********************************************************/
/*    Global variables     */
/********************************************************/
//Variable communication 
int v ; 
int i ;  
int j ; 
int compte ;
int taille = 27 ; 

char message ; 
char d[27] ; 

int v_c ; 
int v_d ; 
int v_u ; 

int ang_c ; 
int ang_d ; 
int ang_u ; 

// donnees dentree
double v_cap = 0.0 ;  // norme du cap m/s
double alpha = 0.0 ;   // angle du cap dans le repere robot en degree

// donnees sortie
double u1, u2, u3, u4;
int rapport1, rapport2, rapport3, rapport4;
int rapport_sens_1, rapport_sens_2, rapport_sens_3, rapport_sens_4;
int sens_m1 = CCW, sens_m3 = CCW;
int sens_m2 = CW, sens_m4 = CW;
String Temp_string;
String var1, var2;
char buffer[3];
int data[2];
int pos1,pos2;

// Initialisation 

void setup()
{
  //Initialisation Pins
  pinMode(PIN_U_M1, OUTPUT);
  pinMode(PIN_U_M2, OUTPUT);
  pinMode(PIN_U_M3, OUTPUT);
  pinMode(PIN_U_M4, OUTPUT);

  pinMode(PIN_SENS_M1, OUTPUT);
  pinMode(PIN_SENS_M2, OUTPUT);
  pinMode(PIN_SENS_M3, OUTPUT);
  pinMode(PIN_SENS_M4, OUTPUT);

  //mise a 0 des pins
  analogWrite(PIN_U_M1, 0);
  analogWrite(PIN_U_M2, 0);
  analogWrite(PIN_U_M3, 0);
  analogWrite(PIN_U_M4, 0);

  digitalWrite(PIN_SENS_M1, 0);
  analogWrite(PIN_SENS_M2, 0);
  analogWrite(PIN_SENS_M3, 0);
  analogWrite(PIN_SENS_M4, 0);

  Serial.begin(9600) ; 
  Serial2.begin(57600);
  Serial.print("Début programme") ;
  Serial.println(" ") ; 

  //valeurs arbitraires
  alpha = 0;
  v_cap = 0;
}


void loop() 
{
  // ENTREE
  /********************************************************/
  // Reception du maitre

if(Serial2.available()) 
  { 
    v = 0 ;
    while(Serial2.available())  
    {
      message = Serial2.read();
      d[v] = message ;
      v = v + 1 ; 
    }

    /*
    for ( i = 0; i <taille; i++)
    {
      Serial.print(d[i]) ;
    }
    Serial.println("") ; 
    */ 

    for (j= 0 ; j<taille;j++)
    { 
      if (d[j] == '@')
      {
        compte = 1 ; 
      }
      else if(compte == 1)
      {
        v_c = (d[j]-48)*100; 
        compte = 2 ; 
      }
      else if(compte == 2)
      {
        v_d = (d[j]-48)*10 ; 
        compte = 3 ;
      }
      else if(compte == 3)
      {
        v_u = (d[j]-48) ; 
        compte = 4 ; 
      }
      else if(compte==4 && d[j]=='#')
      {
        compte = 5 ; 
      }
      else if(compte == 5)
      {
        ang_c = (d[j]-48)*100 ; 
        compte = 6; 
      }
      else if(compte == 6)
      {
       ang_d = (d[j]-48)*10 ; 
        compte = 7;
      }
      else if(compte == 7)
      {
        ang_u = (d[j]-48) ; 
        compte = 8; 
      }
      else if(compte == 8 && d[j] =='#')
      {
        compte = 0 ; 

      }
    }
  }
      v_cap = v_c + v_d + v_u ;
      alpha = ang_c + ang_d + ang_u ; 
      Serial.print("Vitesse : ") ; 
      Serial.print(v_cap) ; 
      Serial.println("") ; 
      Serial.print("Angle :") ; 
      Serial.print(alpha) ; 
      Serial.println("") ; 

  //règle de trois sur la valeur de V_CAP_MAX
//  v_cap = map(v_cap, 0, 100, 0, V_CAP_MAX);
  v_cap = v_cap*V_CAP_MAX/100 ;   
  Serial.print("Vitesse 2 : ") ; 
  Serial.print(v_cap) ; 
  Serial.println("") ;  

  /********************************************************/
  //CALCULS
  /********************************************************/
  //Calculs des tensions a delivree aux moteurs
  ///////////////////moteur 1
   u1 = tension_moteur_impair(alpha, v_cap);
  rapport1 = map(abs(u1), 0, 3.3, 0, 255);

  ///////////////////moteur 2
  u2 = tension_moteur_pair(alpha, v_cap);
  rapport2 = map(abs(u2), 0, 3.3, 0, 255);

  ///////////////////moteur 3
  u3 = tension_moteur_impair(alpha, v_cap);
  rapport3 = map(abs(u3), 0, 3.3, 0, 255);

  ///////////////////moteur 4
  u4 = tension_moteur_pair(alpha, v_cap);
  rapport4 = map(abs(u4), 0, 3.3, 0, 255);
  /********************************************************/
  //Determination du sens de rotation
  sens_m1 = sens_moteur(u1, alpha, 1);
  sens_m2 = sens_moteur(u2, alpha, 2);
  sens_m3 = sens_moteur(u3, alpha, 3);
  sens_m4 = sens_moteur(u4, alpha, 4);

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


  //AFFICHAGE DEBBUGAGE
  

  Serial.print("Moteur1 : ");
  Serial.print(abs(u1));
  Serial.print("    ");
  Serial.print(sens_m1);

  Serial.print("                            Moteur2 : ");
  Serial.print(abs(u2));
  Serial.print("    ");
  Serial.println(sens_m2);

  Serial.print("Moteur4 : ");
  Serial.print(abs(u4));
  Serial.print("    ");
  Serial.print(sens_m4);

  Serial.print("                            Moteur3 : ");
  Serial.print(abs(u3));
  Serial.print("    ");
  Serial.println(sens_m3);
  Serial.println("");


  /********************************************************/
delay(1000) ; 
}

/********************************************************/
/*                    Fonctions                         */
/********************************************************/



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

  omega = (1 / RAYON_ROUE) * v_cap * cos((45 + alpha) * PI / 180) * REDUCTION_ROUE;
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

  omega = (1 / RAYON_ROUE) * v_cap * cos((45 - alpha) * PI / 180) * REDUCTION_ROUE;
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



/********************************************************/
/*  Fonction : sens_moteur()                            */
/********************************************************/
/*  Variables d'entree :                                */
/*  u : type double                                     */
/*      valeur de U delivree par larduino au moteur     */
/*  alpha : type double                                 */
/*      angle du vecteur cap dans le repere robot       */
/*  id_moteur : type int                                */
/*      numero du moter                                 */
/********************************************************/
/*  Variable de sortie :                                */
/*  sens_rotation : type int                            */
/*      sens de rotation du moteur (CW ou CCW)          */
/********************************************************/
int sens_moteur(double u, double alpha, int id_moteur)
{
  int sens_rotation;

  //moteur 1 et 4
  if ((id_moteur == 1) || (id_moteur == 4))
  {
    if (u < 0)
    {
      sens_rotation = CW;
    }
    else
    {
      sens_rotation = CCW;
    }
  }

  //moteur 2 et 3
  if ((id_moteur == 2) || (id_moteur == 3))
  {
    if (u < 0)
    {
      sens_rotation = CCW;
    }
    else
    {
      sens_rotation = CW;
    }
  }

  return sens_rotation;
}
