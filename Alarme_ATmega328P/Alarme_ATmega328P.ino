#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // 0x27 pour LCD 16*2 caractères

int etatAlarme = 0; // variable qui enregistre l'état de l'alarme
int etatBouton = 0; // variable qui enregistre l'état du bouton
int etatCapteur = 0; // variable qui enregistre l'état du capteur

const int relais = 7; // le relais est connecté à la broche 7 de la carte Adruino
const int bouton = 2; // le bouton est connecté à la broche 2 de la carte Adruino
const int capteur = 4; // le capteur est connecté à la broche 4 de la carte Adruino

const int delaisArmement = 10000; // Délais avant armement de l'alarme en milisecondes

void setup()
{
	pinMode(relais, OUTPUT); // le relais est une sortie
	pinMode(bouton, INPUT_PULLUP); // le bouton est une entrée
	pinMode(capteur, INPUT_PULLUP); // le capteur est une entrée

	digitalWrite(relais, HIGH); // le relais est desactivé
		
	etatBouton = HIGH; // initialise l'état du bouton comme "relaché"
	etatCapteur = HIGH; // initialise l'état du capteur comme "pas de détection"
  
	lcd.init(); // initialisation du lcd 
	lcd.backlight();
	lcd.clear();
	lcd.setCursor(3,0);
	lcd.print("ATMega 328P ");
	lcd.setCursor(3,1);
	lcd.print("Start v0.1");
}

void loop()
{

// ---------------------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------Si alarme OFF---------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------------------------



	while(etatAlarme == 0) 
		{
		etatBouton = digitalRead(bouton); // Lecture de l'état du bouton      
		if(etatBouton == 0) // si appui sur le bouton pour l'activation de l'alarme
			{
			etatAlarme = 1;
			lcd.clear();
			lcd.backlight();
			lcd.setCursor(3,0);
			lcd.print("Alarme ON ");
			delay(delaisArmement); // visualisation pendant X secondes du texte puis extinction écran
			lcd.noBacklight();
			}
      
		}


// ---------------------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------Si alarme ON---------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------------------------


	while(etatAlarme == 1) 
		{

		etatBouton = digitalRead(bouton); // Lecture de l'état du bouton      
 		etatCapteur = digitalRead(capteur); // Lecture de l'état du capteur     

		if(etatBouton == 0) // si demande d'arret de l'alarme en appuyant sur le bouton
			{
			lcd.clear();
			lcd.backlight();
			lcd.setCursor(3,0);
			lcd.print("Alarme OFF ");
			digitalWrite(relais, HIGH); //désactivation du contacteur d'alimentation de sirène
			etatAlarme = 0;
			delay(5000); // visualisation pendant 5 secondes du texte puis extinction écran
			lcd.noBacklight();
			}      

		if((etatCapteur == 0) && (etatBouton == 1) && (etatAlarme == 1)) // Si le capteur a détecté qqe chose + bouton relaché + état alarme activé, alors...
			{
			lcd.clear();
			lcd.backlight();
			lcd.setCursor(0,0);
			lcd.print("Activation sys.");
			lcd.setCursor(6,1);
			lcd.print("en cours");
			delay(3000);


                	for(int i = 0; i < 16; i++)//----------------Temporisation avant sirène------------------------------------------------
				{
				etatBouton = digitalRead(bouton); // Lecture de l'état du bouton
				if((etatBouton != 0) && (etatAlarme == 1))
					{
					lcd.setCursor(i,0);
					lcd.print("*");
					delay(delaisArmement / 60);
					}
				}

			for(int i = 0; i < 16; i++)
				{
				etatBouton = digitalRead(bouton); // Lecture de l'état du bouton
				if((etatBouton != 0) && (etatAlarme == 1))
					{
					lcd.setCursor(i,1);
					lcd.print("*");
					delay(delaisArmement / 60);
					}
				}

			for(int i = 0; i < 16; i++)
				{
				etatBouton = digitalRead(bouton); // Lecture de l'état du bouton
				if((etatBouton != 0) && (etatAlarme == 1))
					{
					lcd.setCursor(i,0);
					lcd.print(" ");
					delay(delaisArmement / 60);
					}
				}

			for(int i = 0; i < 16; i++)
				{
				etatBouton = digitalRead(bouton); // Lecture de l'état du bouton
				if((etatBouton != 0) && (etatAlarme == 1))
					{
					lcd.setCursor(i,1);
					lcd.print(" ");
					delay(delaisArmement / 60);
					}
				} //------------------------------Fin de temporisation avant sirène--------------------------------------------






			if((etatBouton != 0) && (etatAlarme == 1))
				{
				digitalWrite(relais, LOW); //Activation du contact de sirène
				lcd.clear();
				lcd.backlight();
				lcd.setCursor(4,0);
				lcd.print("Intrusion");
				}
			} // accolades de: if((etatCapteur == 0) && (etatBouton == 1) && (etatAlarme == 1))
                  


              } // accolades de: while(etatAlarme == 1)

		
	} // accolades de: void loop()
