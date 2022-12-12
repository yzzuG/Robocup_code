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
#define PIN_SENS_M4 25

//  moteur + roue
#define REDUCTION_ROUE 3
#define RAYON_ROUE 2.75
//coef de prop du moteur en rad/s/V
#define SCALING 31.42

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
double rapport1, rapport2, rapport3, rapport4;
double sens_m1 = 0, sens_m3 = 0;
double sens_m2 = 1, sens_m4 = 1;





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

  digitalWrite(PIN_SENS_M1, LOW);
  digitalWrite(PIN_SENS_M2, LOW);
  digitalWrite(PIN_SENS_M3, LOW);
  digitalWrite(PIN_SENS_M4, LOW);

  alpha = 0;
  v_cap = 3;

}

void loop() {
  // ENTREE
  /********************************************************/


  /********************************************************/


  //CALCUL
  /********************************************************/


  //Calculs des tensions a delivree aux moteurs
  ///////////////////moteur 1
  omega1 = (1 / RAYON_ROUE) * v_cap * cos((45 - alpha) * PI / 180) * REDUCTION_ROUE;
  u1 = SCALING * omega1;
  if (u1 > 5)
  {
    u1 = 5;
  }
  else if ((u1 > 0.17) && (u1 < 0.33))
  {
    u1 = 0.25;
  }
  rapport1 = map(u1, 0, 5, 0, 255);

  ///////////////////moteur 2
  omega2 = (1 / RAYON_ROUE) * v_cap * cos((alpha + 45) * PI / 180) * REDUCTION_ROUE;
  u2 = SCALING * omega2;
  if (u2 > 5)
  {
    u2 = 5;
  }
  else if ((u2 > 0.17) && (u2 < 0.33))
  {
    u2 = 0.25;
  }
  rapport2 = map(u2, 0, 5, 0, 255);

  ///////////////////moteur 3
  omega3 = (1 / RAYON_ROUE) * v_cap * cos((45 - alpha) * PI / 180) * REDUCTION_ROUE;
  u3 = SCALING * omega3;
  if (u3 > 5)
  {
    u3 = 5;
  }
  else if ((u3 > 0.17) && (u3 < 0.33))
  {
    u3 = 0.25;
  }
  rapport3 = map(u3, 0, 5, 0, 255);

  ///////////////////moteur 4
  omega4 = (1 / RAYON_ROUE) * v_cap * cos((alpha + 45) * PI / 180) * REDUCTION_ROUE;
  u4 = SCALING * omega4;
  if (u4 > 5)
  {
    u4 = 5;
  }
  else if ((u4 > 0.17) && (u4 < 0.33))
  {
    u4 = 0.25;
  }
  rapport4 = map(u4, 0, 5, 0, 255);

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


double tension_moteur_impair(double alpha, double v_cap)
{
  double omega, u;

  omega = (1 / RAYON_ROUE) * v_cap * cos((45 - alpha) * PI / 180) * REDUCTION_ROUE;
  u = SCALING * omega;
  if (u > 5)
  {
    u = 5;
  }
  else if ((u > 0.17) && (u < 0.33))
  {
    u = 0.25;
  }

  return u;
}
