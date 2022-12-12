/********************************************************/
/*    Bibliotheques    */
/********************************************************/

#include <math.h>

/********************************************************/
/*    Variables globales    */
/********************************************************/
#DEFINE V_MAX


/********************************************************/
/*    Variables    */
/********************************************************/
//destination
double x_d_b0, y_d_b0;
//distance entre robot et point
double distance;
//destination relatif repere monde
double x_d_b0_relatif, y_d_b0_relatif;
//destination repere robot
double x_d_br, y_d_br;

//Variables d'entree
//position robot dans le repere monde
double x_r_b0, y_r_b0;
// angle du robot dans le repere monde --> angle de passage de B0 à Brobot
double theta;

//Variables de sortie
double alpha_cap;
double norme_cap; //en m/s





/********************************************************/
/*    Main    */
/********************************************************/
void main(void)
{
  //calcul point destination relatif par rapport au robot dans repere monde
  x_d_br_relatif = x_d_b0 - x_r_b0;
  y_d_br_relatif = y_d_b0 - y_r_b0;

  //calcul point destination dans repere robot --> vecteur cap
  x_d_br = x_d_br_relatif*cos(theta) + y_d_br_relatif*sin(theta);
  y_d_br = -x_d_br_relatif*sin(theta) + y_d_br_relatif*cos(theta);


  //calcul send out data
  //calcul norme du cap
  distance = sqrt(x_d_br*x_d_br+y_d_br*y_d_br);
  if(distance > 0.2)
  {
    norme_cap = 100;  //en pourcentage
  }
  else if(distance < 0.1)
  {
    norme_cap = 20;   //en pourcentage
  }
  else if(distance = 0)
  {
    norme_cap = 0;    //en pourcentage
  }
  else
  {
    norme_cap = 50;   //en pourcentage
  }
  norme_cap = norme_cap/100*V_MAX;

  //calcul angle cap
  alpha_cap = arccos(x_d_br_relatif*cos(theta)/distance);

}
