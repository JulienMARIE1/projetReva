/* compilation : 
apt-get install libbluetooth-dev
make wiiuse
su
make install
g++ wiimoteTest01.cpp -lwiiuse -lbluetooth -lm
*/



#include <iostream>
#include "libs/src/wiiuse.h"
#define MAX_WIIMOTES    4

using namespace std;
int main() {
  wiimote** wiimotes; //Declaration d'un tableau de pointeurs wiimote
  int found, connected;
 
  //Initialize an array of wiimote objects.
  //The parameter is the number of wiimotes I want to create.
  wiimotes =  wiiuse_init(MAX_WIIMOTES);
 
  //system("CLS");
  cout << "Wiimote Basic Test\n\n";
 
  //On cherche des wiimote
  //En paramètres: le tableau de wiimotes, le nombre MAX à chercher, le timout
  //Retourne le nombre de wiimotes trouvées
  found = wiiuse_find(wiimotes, MAX_WIIMOTES, 5);
 
  if (!found) {
    cout << "No wiimotes found.\n";
    //system("PAUSE");
    return 0;
  }
 
  //On connecte les wiimotes trouvées
  //paramètres: Le tableau de wiimotes, le nombre de wiimotes trouvées
  //retourne le nombre de wiimotes connectées
  connected = wiiuse_connect(wiimotes, found);
 
  if (connected)  cout << "Connected to " << connected << " wiimotes (of " << found << " found).\n";
  else {
    cout << "Failed to connect to any wiimote.\n";
  //  system("PAUSE");
    return 0;
  }
 
  //On allume la LED correspondant au numéro pour chaque wiimote (de 1 à 4)
  wiiuse_set_leds(wiimotes[0], WIIMOTE_LED_1);
  wiiuse_set_leds(wiimotes[1], WIIMOTE_LED_2);
  wiiuse_set_leds(wiimotes[2], WIIMOTE_LED_3);
  wiiuse_set_leds(wiimotes[3], WIIMOTE_LED_4);
 
  //Vibration pour les wiimotes 1 et 2 de 0.2 seconde
  //Arguments (wiimote à faire vibrer, 1 = vibration ; 0 = arret de la vibration)
  wiiuse_rumble(wiimotes[0], 1);
  wiiuse_rumble(wiimotes[1], 1);
  //usleep(200000);
  wiiuse_rumble(wiimotes[0], 0);
  wiiuse_rumble(wiimotes[1], 0);
 
  //Timout des wiimotes, valeur max pour éviter d'avoir des messages d'erreur
  wiiuse_set_timeout(wiimotes,MAX_WIIMOTES,0xFF,0xFF);
 
  //sleep(10);
 
  //Disconnect the wiimotes
  wiiuse_cleanup(wiimotes, MAX_WIIMOTES);
 
  return 0;
}	
