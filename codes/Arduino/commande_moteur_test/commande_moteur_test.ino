/********************************************************/
/*    Bibliotheques    */
/********************************************************/

#include <math.h>

/********************************************************/
/*    Variables globales    */
/********************************************************/
// definition des pins
//moteur 1 (devant droit)
#define PIN_U_M1 2
#define PIN_SENS_M1 22
//moteur 2 (devant gauche)
#define PIN_U_M2 3
#define PIN_SENS_M2 23
//moteur 3 (derriere gauche)
#define PIN_U_M3 4
#define PIN_SENS_M3 24
//moteur 4 (derriere droit)
#define PIN_U_M4 5
#define PIN_SENS_4 25

//  moteur + roue
#define C_RES_MOTEUR 0
#define K_COURANT_MOTEUR 0
#define K_VITESSE_MOTEUR 0
#define R_MOTEUR 0
#define J_EQ 0
#define RAYON_ROUE 0

//autres
#define CCW 1
#define CW 0


/********************************************************/
/*    Variables    */
/********************************************************/
double omega1, omega2, omega3, omega4;  //Variables vitesse de rotation

// donnees dentree
double v_cap ;  // norme du cap m/s
double alpha = 0 ;   // angle du cap dans le repere robot en degree

// donnees sortie
double u1, u2, u3, u4;
double sens_m1 = 0, sens_m3 = 0;
double sens_m2 = 1, sens_m4 = 1;


void setup()
{
  /********************************************************/
  /*    Variables    */
  /********************************************************/
  double omega1, omega2, omega3, omega4;  //Variables vitesse de rotation

  // donnees dentree
  double v_cap ;  // norme du cap m/s
  double alpha = 0 ;   // angle du cap dans le repere robot en degree

  // donnees sortie
  double u1, u2, u3, u4;
  double rapport1,rapport2,rapport3,rapport4;
  double sens_m1 = 0, sens_m3 = 0;
  double sens_m2 = 1, sens_m4 = 1;

  //Initialisation Pins
  pinMode(PIN_U_M1,OUTPUT);
  pinMode(PIN_U_M2,OUTPUT);
  pinMode(PIN_U_M3,OUTPUT);
  pinMode(PIN_U_M4,OUTPUT);
  
  pinMode(PIN_SENS_M1,OUTPUT);
  pinMode(PIN_SENS_M2,OUTPUT);
  pinMode(PIN_SENS_M3,OUTPUT);
  pinMode(PIN_SENS_M4,OUTPUT);
  
  //mise a 0 des pins
  analogWrite(PIN_U_M1,0);
  analogWrite(PIN_U_M2,0);
  analogWrite(PIN_U_M3,0);
  analogWrite(PIN_U_M4,0);

  digitalWrite(PIN_SENS_M1,LOW);
  digitalWrite(PIN_SENS_M2,LOW);
  digitalWrite(PIN_SENS_M3,LOW);
  digitalWrite(PIN_SENS_M4,LOW);

  alpha = 0;
  v_cap = 3;

}

void loop() {
  // ENTREE
  /********************************************************/
  

  /********************************************************/


  //CALCUL
  /********************************************************/

  omega1 = (1 / RAYON_ROUE) * v_cap * cos((45 - alpha) * PI / 180);
  u1 = R_MOTEUR / K_COURANT_MOTEUR * ((J_EQ + K_COURANT_MOTEUR * K_VITESSE_MOTEUR / R_MOTEUR - C_RES_MOTEUR) * omega1);
  rapport1 = map(u1,0,5,0,255);

  omega2 = (1 / RAYON_ROUE) * v_cap * cos((alpha + 45) * PI / 180);
  u2 = R_MOTEUR / K_COURANT_MOTEUR * ((J_EQ + K_COURANT_MOTEUR * K_VITESSE_MOTEUR / R_MOTEUR - C_RES_MOTEUR) * omega2);
  rapport2 = map(u2,0,10,0,255);

  omega3 = (1 / RAYON_ROUE) * v_cap * cos((45 - alpha) * PI / 180);
  u3 = R_MOTEUR / K_COURANT_MOTEUR * ((J_EQ + K_COURANT_MOTEUR * K_VITESSE_MOTEUR / R_MOTEUR - C_RES_MOTEUR) * omega3);
  rapport3 = map(u3,0,10,0,255);
  
  omega4 = (1 / RAYON_ROUE) * v_cap * cos((alpha + 45) * PI / 180);
  u4 = R_MOTEUR / K_COURANT_MOTEUR * ((J_EQ + K_COURANT_MOTEUR * K_VITESSE_MOTEUR / R_MOTEUR - C_RES_MOTEUR) * omega4);
  rapport4 = map(u4,0,10,0,255);

  /********************************************************/


  //SORTIE
  /********************************************************/

  analogWrite(PIN_U_M1,rapport1);
  analogWrite(PIN_U_M2,rapport2);
  analogWrite(PIN_U_M3,rapport3);
  analogWrite(PIN_U_M4,rapport4);

  digitalWrite(PIN_SENS_M1,sens_m1);
  digitalWrite(PIN_SENS_M2,sens_m2);
  digitalWrite(PIN_SENS_M3,sens_m3);
  digitalWrite(PIN_SENS_M4,sens_m4);

  /********************************************************/
}
