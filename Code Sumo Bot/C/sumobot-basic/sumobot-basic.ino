/***********************************************************************
* NOM DU FICHIER :    sumobot.ino
*
* DESCRIPTION :
*       A complete code example in one file for the Sumobot v5.2
*
* NOTES :
*
* AUTEUR :  Aurélien Lecouf
* DATE DE CREATION :  01/01/2023
*
* MODIFICATIONS :
* DATE        QUI                     DETAILS
* 01/01/2023  Corentin Poupry         Update comments and code normalization
* 10/09/2023  Naimeric Villafruela    Francisation des commentaires & modification de code
**/

// Création des variables pour les détecteurs de ligne avant
#define AV_DROIT 1
#define AV_GAUCHE 2

// Seuil de sensibilité des capteurs de ligne avant (à Régler en fonction du Dohyo)
#define SEUIL_DROIT 800
#define SEUIL_GAUCHE 800

// Déclaration des numéros de pin
#define D2 2
#define D3 3
#define D4 4
#define D5 5
#define D6 6
#define D7 7
#define D8 8
#define D9 9
#define D10 10
#define D11 11
#define D12 12
#define D13 13

/*
 * Pin du bouton GO (démarrage)
 */
const int GO = D2;

/*
 * Pins du capteur ultrason.
 *
 * notes: utilisation du capteur HCSR 04 pour la détection par ultrason
 */
const int ECHO = D3; // Récepteur des ultrasons
const int TRIGGER = D4; // Eméteur des ultrasons

/*
 * Pins de commande de vitesse des moteurs du sumobot
 *
 * notes: utilisation du pont en H avec une PWM de fréquence 976.56 Hz
 */
const int G_PWM = D5;
const int D_PWM = D6;

/*
 * Pins des LEDs du sumobot
 *
 * notes: les LEDs peuvent vous indiquer en combat des informations diverses
 * selon l'endroit d'où vous les appelez dans votre programme
 */
const int LED_G = D7;
const int LED_D = D8;

/*
 * Pins de direction de rotation du moteur gauche
 *
 * notes: utilise une pin analogique Arduino en tant que pin digital
 */
const int G_DIR_1 = D9;
const int G_DIR_2 = D10; 

/*
 * Pins analogiques pour les capteurs de ligne blanche
 *
 * notes: utilisation du capteur TCRT5000 avec un pin analogique Arduino pour la détection des lignes blanche. Lecture des valeurs analogiques des capteurs IR.
 */
const int TCRT_G = A0;
const int TCRT_D = A1;

/*
 * Pins de direction de rotation du moteur droit
 *
 * notes: utilise une pin analogique Arduino en tant que pin digital
 */
const int D_DIR_1 = A3;
const int D_DIR_2 = A2;

/**
 * La fonction `setup()` est appelée lorsqu'un sketch démarre et ne sera exécutée qu'une seule fois, après chaque mise sous tension ou réinitialisation de la carte Arduino
 * On l'utilise pour initialiser notre programme en définissant la direction des pins ainsi que d'autres actions.
 */
void setup() {
    pinMode(GO, INPUT_PULLUP);  // le pin GO est en mode entrée avec une résistance de pullup pour distinguer les 2 niveaux logique du bouton
    pinMode(ECHO, INPUT);       // le pin ECHO est en mode entrée (récepteur)
    pinMode(TRIGGER, OUTPUT);   // le pin TRIGGER est en mode sortie (émeteur)

    pinMode(G_PWM, OUTPUT);     // le pin G_PWM est en mode sortie
    pinMode(D_PWM, OUTPUT);     // le pin D_PWM est en mode sortie

    pinMode(LED_G, OUTPUT);     // le pin LED_G est en mode sortie
    pinMode(LED_D, OUTPUT);     // le pin LED_D est en mode sortie

    pinMode(G_DIR_1, OUTPUT);   // le pin G_DIR_1 est en mode sortie
    pinMode(G_DIR_2, OUTPUT);   // le pin G_DIR_2 est en mode sortie

    pinMode(TCRT_G,INPUT);      // le pin TCRT_G est en mode entrée
    pinMode(TCRT_D,INPUT);      // le pin TCRT_D est en mode entrée

    pinMode(D_DIR_1, OUTPUT);   // le pin D_DIR_1 est en mode sortie
    pinMode(D_DIR_2, OUTPUT);   // le pin D_DIR_2 est en mode sortie
}

/*
 * la fonction `loop()` boucle indéfiniement sur elle-même. Tout ce qui est contenu dans la loop est répété à l'infini.
 * Utilisez-la pour contrôler votre Sumobot.
 */
void loop() {
    stop();                                 // arrêt des moteurs.
    attenteGo();                           // attente d'appui sur le bouton GO
    attente5s();                             // attente réglementaire des 5 secondes

    while(digitalRead(GO) == HIGH) {
        autonome();                         // fonction de jeu du sumobot
    }
}

void autonome() {
    // On enregistre dans la variable ligne si une ligne blanche est détectée
    int ligne = ligneBlanche(); // 0 : pas de détection, 1 détection

//*************************
// DETECTION DE LA LIGNE
//*************************

    // si une ligne blanche est détectée, peu importe le côté, le sumobot interrompt sa recherche
    if (ligne > 0) {
        
//*************************
// CAPTEUR GAUCHE POSITIF
//*************************

        // si le capteur gauche voit une ligne blanche on tourne vers la droite
        // La puissance de déplacement des moteurs se règle entre 0 et 255 (255 = vitesse maximale)
        
        if (ligne == AV_GAUCHE) {
            // temps de rotation de votre robot après détection de ligne blanche
            int wait = random(50, 300); // A régler en fonction de la taille de la roue

            //On allume la LED du côté du capteur de ligne activé de façon à comprendre ce qui se passe
            digitalWrite(LED_G, HIGH);
            digitalWrite(LED_D, LOW);

            // le sumobot recule à toute vitesse pour revenir en jeu pendant 500 ms
            back(255, 255); // A régler
            delay(500); // A régler

            // et tourne à droite pendant un temps compris entre 50 et 300 ms
            turnRight(255, 255); // A régler
            delay(wait); // A régler

            // puis le sumobot revient en jeu, face avant en charge pendant 20 ms
            forward(200, 200); // A régler
            delay(20); // A régler
        }

//*************************
// CAPTEUR DROIT POSITIF
//*************************

        // si le capteur droit détecte une ligne blanche
        if (ligne == AV_DROIT) {
            // temps de rotation de votre robot après détection de ligne blanche
            int wait = random(50, 300);  // A régler

            //On allume la LED du côté du capteur de ligne activé de façon à comprendre ce qui se passe
            digitalWrite(LED_G, LOW);
            digitalWrite(LED_D, HIGH);

            // le sumobot recule à toute vitesse pour revenir en jeu pendant 500 ms
            back(255, 255);  // A régler
            delay(500);  // A régler

            // et tourne à gauche pendant un temps compris entre 50 et 300 ms
            turnLeft(255, 255);  // A régler
            delay(wait);  // A régler

            // puis le sumobot revient en jeu, face avant en charge pendant 20 ms
            forward(200, 200);  // A régler
            delay(20); // A régler
        }

    } else if (soundDistanceCheck(1.0, 37.0) == true) {
        // si l'adversaire est détecté entre 1 et 37 cm
        // (on laisse un décalage de distance dû à la profondeur de la pelle et du capteur ultrason)

        // on allume les 2 LEDs
        digitalWrite(LED_G, HIGH);
        digitalWrite(LED_D, HIGH);

        // et on va tout droit
        forward(150, 150);   // A régler
        delay(10);   // A régler
    } else {

//*************************
// STRATEGIE
//*************************

        // c'est ici que vous pouvez implémenter votre stratégie de recherche

        // on éteins les LEDs
        digitalWrite(LED_G, LOW);
        digitalWrite(LED_D, LOW);

        // le sumobot avance jusqu'à rencontrer une ligne blanche et tourne pour balayer le terrain
        forward(160 ,160);
    }

}

//************************
// FONCTIONS
//************************

/**
 * Cette fonction va attendre que l'utilisateur presse sur le bouton GO avant de finir
 */

void attenteGo() {
    // on crée une variable pour l'état des LEDs
    bool led = false; // 0 (false) etteint / 1 (true) allumé

    // tant que le bouton poussoir est à l'état haut (grâce a l'argument PULLUP)
    while(digitalRead(GO) == HIGH) {
        // on fait clignoter les 2 leds pour montrer qu'on attend le go
        if (led) {
            digitalWrite(LED_G, LOW);
            digitalWrite(LED_D, HIGH);
        }
        else {
            digitalWrite(LED_G, HIGH);
            digitalWrite(LED_D, LOW);
        }

        // on inverse la variable booléenne pour inverser l'état des LEDs
        led = !led;

        // on répète l'opération toutes les 200ms, pour créer le clignotement
        delay(200);
    }
    // quand le bouton est pressé, on quitte la boucle while au dessus
    // et on éteint les LEDs
    digitalWrite(LED_G, LOW);
    digitalWrite(LED_D, LOW);
}

/**
 * Cette fonction va attendre 5 secondes avant de finir.
 * Elle permet de définir le comportement du robot lors de l'attente réglementaire
 */

// C'est ici qu'on peu aussi faire de la musique...
void attente5s() {
    //on répète cette boucle 5 fois, avec une attente de 0.250 * 4 = 1s pour chaque itération
    for(int i = 0; i < 5; i++) {
        digitalWrite(LED_G, LOW);
        digitalWrite(LED_D, LOW);
        delay(250);
        digitalWrite(LED_G, HIGH);
        digitalWrite(LED_D, LOW);
        delay(250);
        digitalWrite(LED_G, HIGH);
        digitalWrite(LED_D, HIGH);
        delay(250);
        digitalWrite(LED_G, LOW);
        digitalWrite(LED_D, HIGH);
        delay(250);
    }
}

/**
 * Cette fonction va calculer la distance entre le robot et l'obstacle en face de lui.
 */
float soundDistance() {
    // le ping est déclenché par une impulsion au niveau HAUT de 2 microsecondes ou plus
    // on donne une courte impulsion au niveau BAS avant pour assurer une impulsion HAUT propre
    digitalWrite(TRIGGER, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER, HIGH);
    delayMicroseconds(5);
    digitalWrite(TRIGGER, LOW);

    // la même broche est utilisée pour lire le signal du ping
    // une impulsion HIGH dont la durée est le temps (en microsecondes) entre l'envoi du ping et la réception de son écho
    // on ajoute un timeout de 25ms en troisième argument pour le cas où aucun objet n'est devant le capteur
    long duration = pulseIn(ECHO, HIGH, 25000);

    // on attend le minimum de 200µs, c'est le temps de réaction du capteur
    delayMicroseconds(200);

    // la vitesse du son est de 340 m/s ou 29 microsecondes par centimètre
    // le ping fait l'aller et retour de la distance à la cible, on divise donc par 2
    // On retourne la distance en cm à la cible
    return duration / 29 / 2;
}

/**
 * Cette fonction permet de vérifier si un adversaire a été détecté en utilisant un seuil de détection.
 * Le kit sumobot vous est fournit avec un capteur HCSR-04 renvoyant une mesure allant jusqu'à 4m!
 *
 * Pour éviter de détecter tout ce qui entoure votre sumobot, insérez une valeur à l'appel de la fonction soundDistanceCheck(..),
 * en renseignant dans distance la limite maximale, et dans limite_inferieure la distance minimale
 */
boolean soundDistanceCheck(float distance_min, float distance_max) {
    float mesure = soundDistance();

    return mesure > distance_min && mesure < distance_max;

}

/**
 * Cette fonction permet d'avancer tant qu'elle est appelée.
 * La vitesse de rotation est donnée par les paramètres pGauche (vitesse du moteur gauche) et pDroit (vitesse du moteur droit).
 * Plus ces valeurs sont grandes, plus votre robot sera rapide (valeur comprise entre 0 et 255 inclus).
 */
void forward(int pGauche, int pDroit) {
    // on met en place l'avancement au niveau du pont en H
    digitalWrite(G_DIR_1, HIGH);
    digitalWrite(G_DIR_2, LOW);
    digitalWrite(D_DIR_1, HIGH);
    digitalWrite(D_DIR_2, LOW);

    // on met la PWM à la valeur demandée
    analogWrite(G_PWM, pGauche);
    analogWrite(D_PWM, pDroit);
}

/**
 * Cette fonction permet de reculer tant qu'elle est appelée.
 * La vitesse des moteurs est donnée par les paramètres pGauche (vitesse du moteur gauche) et pDroit (vitesse du moteur Droit).
 * Plus ces valeurs sont grandes, plus votre robot sera rapide (valeur comprise entre 0 et 255).
 */
void back(int pGauche, int pDroit)
{
    // on met en place le recul au niveau du pont en H
    digitalWrite(G_DIR_1, LOW);
    digitalWrite(G_DIR_2, HIGH);
    digitalWrite(D_DIR_1, LOW);
    digitalWrite(D_DIR_2, HIGH);

    // on met la PWM à la valeur demandée
    analogWrite(G_PWM, pGauche);
    analogWrite(D_PWM, pDroit);
}

/**
 * Cette fonction permet de tourner à gauche tant qu'elle est appelée.
 * La vitesse de rotation est donnée par les paramètres pGauche (vitesse du moteur gauche) et pDroit (vitesse du moteur droit).
 * Plus ces valeurs sont grandes, plus votre robot sera rapide (valeur comprise entre 0 et 255 inclus).
 *
 * remarque: le tour se fait sur place, on n'avance pas ni ne recule
 */
void turnLeft(int pGauche, int pDroit) {
    // on met en place le tour à gauche au niveau du pont en H
    digitalWrite(G_DIR_1, LOW);
    digitalWrite(G_DIR_2, HIGH);
    digitalWrite(D_DIR_1, HIGH);
    digitalWrite(D_DIR_2, LOW);

    // on met la PWM à la valeur demandée
    analogWrite(G_PWM, pGauche);
    analogWrite(D_PWM, pDroit);
}

/*
 * Cette fonction sert à détecter les lignes blanches.
 *
 * notes: fonctionne avec les pins analogiques A0 et A1.
 */
int ligneBlanche()
{
    // si le capteur de ligne blanche avant gauche ou droit est activé
    if (analogRead(TCRT_G) <= SEUIL_GAUCHE) {
        return AV_GAUCHE;
    } else if (analogRead(TCRT_D) <= SEUIL_DROIT) {
        return AV_DROIT;
    } else {
        return 0;
    }
}

/**
 * Cette fonction permet de tourner à droite tant qu'elle est appelée.
 * La vitesse de rotation est donnée par les paramètres pGauche (vitesse du moteur gauche) et pDroit (vitesse du moteur droit).
 * Plus ces valeurs sont grandes, plus votre robot sera rapide (valeur comprise entre 0 et 255 inclus).
 *
 * remarque: le tour se fait sur place, on n'avance pas ni ne recule
 */
void turnRight(int pGauche, int pDroit) {
    // on met en place le tour à droite au niveau du pont en H
    digitalWrite(G_DIR_1, HIGH);
    digitalWrite(G_DIR_2, LOW);
    digitalWrite(D_DIR_1, LOW);
    digitalWrite(D_DIR_2, HIGH);

    // on met la PWM à la valeur demandée
    analogWrite(G_PWM, pGauche);
    analogWrite(D_PWM, pDroit);
}

/**
 * Cette fonction va arrêter les moteurs
 */
void stop() {
    // on met à l'état bas les différents pin de direction
    digitalWrite(G_DIR_1, LOW);
    digitalWrite(G_DIR_2, LOW);
    digitalWrite(D_DIR_1, LOW);
    digitalWrite(D_DIR_2, LOW);

    // on met la PWM à 0 pour les moteurs
    analogWrite(G_PWM,0);
    analogWrite(D_PWM,0);
}
