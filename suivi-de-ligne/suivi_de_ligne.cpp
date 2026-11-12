/* File: tp0.cpp
 * Author:
 * Source: https://github.com/platu/libsensehat-cpp
 *
 * 
 */

#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <sensehat.h>

using namespace std;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono;      // system_clock, seconds, milliseconds

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Début constantes

// Fin constantes
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Début sous-programmes

void initalisation()     // On défini toutes les fonctions des deux moteurs
{
    pwmInit(0);
    pwmPeriod(0, 10000);
    pwmDutyCycle(0, 0);
    pwmEnable(0); 
    pwmInit(1);
    pwmPeriod(1, 10000);
    pwmDutyCycle(1, 0);
    pwmEnable(1);
}

void avancer(unsigned int vitesse) // Fonction pour avancer, les deux moteurs prennent la valeur de vitesse
{
    pwmDutyCycle(0, vitesse);
    pwmDutyCycle(1, vitesse);  
}

void tournerD(unsigned int vitesse) // Fonction pour tourner à droite, moteur gauche prend la valeur de vitesse, et moteur droit que 0,15 de la vitesse
{
    pwmDutyCycle(1, vitesse);
    pwmDutyCycle(0, vitesse/5);
}

void tournerG(unsigned int vitesse)   // Fonction pour tourner à gauche, moteur droit prend la valeur de vitesse, et moteur gauche que 0,15 de la vitesse
{
    pwmDutyCycle(1, vitesse/5);
    pwmDutyCycle(0, vitesse);
}

void stop() // Fonction stop,, les deux moteur à l'arret
{ 

    pwmDutyCycle(0, 0);
    pwmDutyCycle(1, 0);
}

void lireCapteurs(int &droite, int &gauche){

    int cD = 0;
    int cG = 0;
    for (int i = 0; i <= 5; i++)
    {
        cD = cD + gpioGetInput(17);
        cG = cG + gpioGetInput(16);
        sleep_for(milliseconds(10));
    }
    
    if( cD > 2){
        droite = 1;
    } else {
        droite = 0;
    }

    if( cG > 2){
        gauche = 1;
    } else {
        gauche = 0;
    }

}


// Fin sous-programmes
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int main()
{
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Début variables

       int etatCaptD;
       int etatCaptG;
       unsigned int vitesse;

    // Fin variables
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   
    if (senseInit())
    {
        cout << "Sense Hat initialization Ok." << endl;
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // Début instructions;

        gpioSetConfig(17, in);
        gpioSetConfig(16, in);
        initalisation();
        cout << "Entrez une vitesse max (65 conseillé) : ";
        cin>> vitesse;
        bool continuer = true;
        
        do { 
            // etatCaptD=gpioGetInput(17);
            // etatCaptG=gpioGetInput(16);

            lireCapteurs(etatCaptD, etatCaptG);

            cout << "CG: " << etatCaptG << " | CD: " << etatCaptD << endl;
            if (etatCaptD == 1 && etatCaptG == 1)  // si les deux capteurs sont égaux à 1 (détecte du blanc), on appelle la fonction avancer
                avancer(vitesse);
            else if (etatCaptD == 1 && etatCaptG == 0) // si CaptD = 1 et CaptG = 0 (détect du noir), on appelle la fonction tournerD
                tournerD(vitesse);
            else if (etatCaptD == 0 && etatCaptG == 1) // si CaptD = 0 et CaptG = 1, on appelle la fonction tournerG
                tournerG(vitesse);
            else {   // Sinon, (si les deux capteurs détecte du noir) on appelle la fonction stop
                stop();
                continuer = false;
                }
        } while(continuer == true);


        //moteurGauche();
        //moteurDroite();

        // Fin instructions
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        cout << "Press joystick button to quit." << endl;
        senseWaitForJoystickEnter();

        senseShutdown();
        cout << "Sense Hat shut down." << endl;
    }

    return EXIT_SUCCESS;
}
