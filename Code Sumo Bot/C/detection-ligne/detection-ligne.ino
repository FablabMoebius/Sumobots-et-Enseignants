/***********************************************************************
* NOM DU FICHIER :    detection-ligne.ino
*
* DESCRIPTION :
*       Programme pour calibrer les capteur de détection de ligne
*
* NOTES :
*
* AUTEUR :  Naimeric Villafruela
* DATE DE CREATION :  10/09/2023
*
* MODIFICATIONS :
* DATE        QUI                     DETAILS
* 10/09/2023  Naimeric Villafruela    Ecriture du code initial
**/

// Création des variables pour les détecteurs de ligne avant
#define AV_DROIT 1
#define AV_GAUCHE 2

// déclaration des cateurs droits et gauches
const int TCRT_G = A0;
const int TCRT_D = A1;

// Seuil de sensibilité des capteurs de ligne avant (à Régler en fonction du Dohyo)
#define SEUIL_DROIT 800
#define SEUIL_GAUCHE 800

// Déclaration des numéros de pin
#define D7 7 // LED GAUCHE
#define D8 8 // LED DROITE


/*
 * Pins des LEDs du sumobot
 *
 * notes: les LEDs peuvent vous indiquer en combat des informations diverses
 * selon l'endroit d'où vous les appelez dans votre programme
 */
const int LED_G = D7;
const int LED_D = D8;

/**
 * La fonction `setup()` est appelée lorsqu'un sketch démarre et ne sera exécutée qu'une seule fois, après chaque mise sous tension ou réinitialisation de la carte Arduino
 * On l'utilise pour initialiser notre programme en définissant la direction des pins ainsi que d'autres actions.
 */
void setup() {
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