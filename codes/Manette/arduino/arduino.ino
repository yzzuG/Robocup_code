#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//----- Adressage matériel -----
// En cas de non fonctionnement, mettez la ligne 8 en
// commentaire et retirez le commentaire à la ligne 9.

LiquidCrystal_I2C lcd(0x27, 20, 4);

//char v_cap_char;
//char alpha_char;
int data_temp[3] ;
int alpha_cap = 0, v_cap = 0 ;





void setup()
{
  lcd.begin(); // initialisation de l'afficheur
  Serial2.begin(57600);
  lcd.backlight();
}

void loop()
{
  if (Serial2.available()) {
    // Wait a bit for the entire message to arrive
    //delay(100);
    // Clear the screen
    lcd.clear();

    // Write all characters received with the serial port to the LCD.
    while (Serial2.available() > 0) {
      //data = Serial.parseInt();
      //Serial.parseInt();

      data_temp[0] = Serial2.read();
      data_temp[1] = Serial2.read();
      data_temp[2] = Serial2.read();
    }
  }




  if (data_temp[0] == 0xFB)
  {

    if (data_temp[2] == 0x04)
    {
      alpha_cap = 0;
    }
    else if (data_temp[1] == 0x02)
    {
      alpha_cap = -data_temp[2];
    }
    else if  (data_temp[1] == 0x01)
    {
      alpha_cap = data_temp[2];
    }
    else
    {
      alpha_cap = data_temp[2];
    }
  }
  else if(data_temp[0] == 0xFC)
  {
    if (data_temp[2] == 0x04)
    {
      v_cap = 0;
    }
    else
    {
      v_cap = data_temp[2];
    }
  }

  lcd.setCursor(0, 0);
  lcd.print("code data");
  lcd.setCursor(10, 0);
  lcd.print(data_temp[0]);
  lcd.setCursor(0, 2);
  lcd.print("alpha_cap");
  lcd.setCursor(12, 2);
  lcd.print("v_cap");
  lcd.setCursor(0, 3);
  lcd.print(alpha_cap);
  lcd.setCursor(12, 3);
  lcd.print(v_cap);
  lcd.setCursor(5, 3);
  lcd.print("deg");
  lcd.setCursor(17, 3);
  lcd.print("%");

}
