/***********************************************************************
* NOM DU FICHIER :    detection-ligne.ino
*
* DESCRIPTION :
*       Programme pour calibrer les capteur de détection de ligne
*
* NOTES :
* Sumobot board Version 5.2
*
* AUTEUR :  Naimeric Villafruela
* DATE DE CREATION :  10/09/2023
*
* MODIFICATIONS :
* DATE        QUI                     DETAILS
* 10/09/2023  Naimeric Villafruela    Ecriture du code initial
**/


// déclaration des cateurs droits et gauches
const int TCRT_G = A0; //A0
const int TCRT_D = A1; //A1

// Seuil de sensibilité des capteurs de ligne avant (à Régler en fonction du Dohyo)

#define SEUIL_DROIT 800
#define SEUIL_GAUCHE 800

/*
 * Pins des LEDs du sumobot
 *
 * notes: les LEDs peuvent vous indiquer en combat des informations diverses
 * selon l'endroit d'où vous les appelez dans votre programme
 */
// Déclaration des numéros de pin
#define LED_G 7 // LED GAUCHE
#define LED_D 8 // LED DROITE

/**
 * La fonction `setup()` est appelée lorsqu'un sketch démarre et ne sera exécutée qu'une seule fois, après chaque mise sous tension ou réinitialisation de la carte Arduino
 * On l'utilise pour initialiser notre programme en définissant la direction des pins ainsi que d'autres actions.
 */
void setup() {

  pinMode (LED_G, OUTPUT);
  pinMode (LED_D, OUTPUT);
  pinMode (TCRT_G, INPUT);
  pinMode (TCRT_D, INPUT);

  Serial.begin(9600);
  Serial.println("Programme démarré");

}

/*
 * la fonction `loop()` boucle indéfiniement sur elle-même. Tout ce qui est contenu dans la loop est répété à l'infini.
 * Utilisez-la pour contrôler votre Sumobot.
 */
void loop() {
  int CGAUCHE = analogRead(TCRT_G);
  int CDROIT = analogRead(TCRT_D);
  Serial.print("Capteurs Gauche | Droit : ");
  Serial.print(CGAUCHE);
  Serial.print(" | ");
  Serial.println(CDROIT);
  
    if (CGAUCHE <= SEUIL_GAUCHE) {
      digitalWrite(LED_G, LOW);
    } else {
      digitalWrite(LED_G, HIGH);
      Serial.println("allumé G");
    }
    if (CDROIT <= SEUIL_DROIT) {
      digitalWrite(LED_D, LOW);
    } else {
      digitalWrite(LED_D, HIGH);
      Serial.println("allumé D");
    }

  delay(500);
}