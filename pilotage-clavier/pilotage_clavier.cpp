/* File: Projet.cpp
 * Author:
 * Source: https://github.com/platu/libsensehat-cpp
 *
 * Patron du code source des séances de travaux pratiques
 */

#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <sensehat.h>
#include <console_io.h>


using namespace std;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono;      // system_clock, seconds, milliseconds





// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Début constantes
const unsigned MOT_G = 0;
const unsigned MOT_D = 1;
const unsigned CAP_G = 16;
const unsigned CAP_D = 17;
const int STATUS_ROW = 10;
const int ACTION_ROW = 12;

const rgb_pixel_t black = {.color = {0, 0, 0}};
const rgb_pixel_t white = {.color = {255, 255, 255}};
const rgb_pixel_t red = {.color = {255, 0, 0}};
const rgb_pixel_t orange = {.color = {255, 128, 0}};
const rgb_pixel_t yellow = {.color = {255, 255, 0}};
const rgb_pixel_t green = {.color = {0, 255, 0}};
const rgb_pixel_t cyan = {.color = {0, 255, 255}};
const rgb_pixel_t blue = {.color = {0, 0, 255}};
const rgb_pixel_t purple = {.color = {255, 0, 255}};
const rgb_pixel_t pink = {.color = {255, 128, 128}};



// Fin constantes
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Début sous-programmes

void initialiserRobot() {


    gpioSetConfig(CAP_G, in);
    gpioSetConfig(CAP_D, in);

    pwmInit(MOT_G);
    pwmPeriod(MOT_G, 10000);
    pwmDutyCycle(MOT_G, 0);
    pwmEnable(MOT_G);
    pwmInit(MOT_D);
    pwmPeriod(MOT_D, 10000);
    pwmDutyCycle(MOT_D, 0);
    pwmEnable(MOT_D);
}

void avancer(unsigned int vitesse) {
    pwmDutyCycle(MOT_G, vitesse);
    pwmDutyCycle(MOT_D, vitesse);
}

void tournerGauche(unsigned int vitesse) {
    pwmDutyCycle(MOT_G, vitesse/2);  
    pwmDutyCycle(MOT_D, vitesse);  
}

void tournerDroite(unsigned int vitesse) {
    pwmDutyCycle(MOT_G, vitesse);  
    pwmDutyCycle(MOT_D, vitesse/2);  
}

void arreter() {
    pwmDutyCycle(MOT_G, 0);
    pwmDutyCycle(MOT_D, 0);
}

void ralentir(unsigned int vitesse){
    pwmDutyCycle(MOT_G, vitesse/2);
    pwmDutyCycle(MOT_D, vitesse/2);
}

void afficherMenu()
{
    // Console initialization
    clearConsole();

    // Display header and instructions
    gotoxy(1, 1);
    std::cout << "========================================" << std::endl;
    std::cout << "        Arrow Keys Detection Demo       " << std::endl;
    std::cout << "========================================" << std::endl;
    gotoxy(1, 5);
    std::cout << "Instructions:" << std::endl;
    std::cout << "  - Use arrow keys to navigate" << std::endl;
    std::cout << "  - Press 'Q' to quit" << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    // Display initial status
    gotoxy(1, STATUS_ROW);
    std::cout << "Status: Waiting for input...";
    gotoxy(1, ACTION_ROW);
    std::cout << "Action: ";
    std::cout << std::flush;
}

void afficherGagnant(){
    const rgb_pixel_t R = { .color = {255, 0, 0} };   // couleur rouge 
    const rgb_pixel_t B = { .color = {0, 0, 0} };     // couleur noire
    
    rgb_pixels_t drapeau_RougeNoir = { .array = {
        { R, R, R, R, R, R, R, R },
        { R, R, R, R, R, R, R, R },
        { R, R, R, R, R, R, R, R },
        { R, R, R, R, R, R, R, R },
        { B, B, B, B, B, B, B, B },
        { B, B, B, B, B, B, B, B },
        { B, B, B, B, B, B, B, B },
        { B, B, B, B, B, B, B, B } }
    };
    senseSetRGBpixels(drapeau_RougeNoir);
}



void afficherTouches()
{
    int key_count = 0;
    char c;
    float vitesse = 70;
    int capteurG = gpioGetInput(16);
    int capteurD = gpioGetInput(17);
    int a = 10;
    int cpt = 1;
    
    afficherMenu();
  
    // Main task
    do {
        // Get the number of keys in the keyboard buffer
        key_count = keypressed();
        
        // Print the key ascii code if a single key is pressed
        if (key_count == 1) {
            c = std::cin.get();

            // Clear and update status line
            gotoxy(1, STATUS_ROW);
            clearEOL();
            std::cout << "Status: Key pressed";

            // Clear and display action
            gotoxy(1, ACTION_ROW);
            clearEOL();
            std::cout << "Action: Key = [" << c
                        << "] (ASCII: " << static_cast<int>(c) << ")";
            std::cout << std::flush;

            // Stop the program if 'q' is pressed
            if (toupper(c) == 'Q') {
                gotoxy(1, ACTION_ROW);
                clearEOL();
                std::cout << "Action: Quitting..." << std::flush;
            }
        }
        // Detect arrow keys if the key count is greater than 1
        else if (key_count > 1) {
            // Clear and update status line
            gotoxy(1, STATUS_ROW);
            clearEOL();
            std::cout << "Status: Arrow key detected";

            // Clear and display action
            gotoxy(1, ACTION_ROW);
            clearEOL();
            std::cout << "Action: ";

            switch (getArrowKey()) {
                case UP:
                    avancer(vitesse + a * cpt);
                    cpt += 1;
                    std::cout << "↑ UP - Accelerate";
                    break;
                case DOWN:
                    ralentir(vitesse + a * cpt);
                    cpt -= 1;
                    std::cout << "↓ DOWN - Slow down";
                    break;
                case RIGHT:
                    tournerDroite(vitesse);
                    std::cout << "→ RIGHT - Turn right";
                    break;
                case LEFT:
                    tournerGauche(vitesse);
                    std::cout << "← LEFT - Turn left";
                    break;
                default:
                    std::cout << "? Unknown key sequence";
                    break;
            }
            std::cout << std::flush;
        }
        capteurG = gpioGetInput(16);
        capteurD = gpioGetInput(17);

    } while (capteurD == 1 || capteurG == 1);
    arreter();
    afficherGagnant();
}


// Fin sous-programmes
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int main() {
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Début variables
    // Fin variables
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   

    if (senseInit()) {
        cout << "Sense Hat initialization Ok." << endl;
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // Début instructions	
        initialiserRobot();
        afficherTouches();

        // Fin instructions
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        cout << "Press joystick button to quit." << endl;
        senseWaitForJoystickEnter();

        senseShutdown();
        cout << "Sense Hat shut down." << endl;
    }

    return EXIT_SUCCESS;
}
