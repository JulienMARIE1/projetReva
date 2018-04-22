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


void handle_event (struct wiimote_t* wm){
	
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_A)) {
		cout << "A pressed" << endl;
	}
	if (WIIUSE_USING_ACC(wm)) {
		printf("wiimote roll = %f [%f]\n", wm->orient.roll, wm->orient.a_roll);
		printf("wiimote pitch = %f [%f]\n", wm->orient.pitch, wm->orient.a_pitch);
		printf("wiimote yaw = %f\n", wm->orient.yaw);
	}

	if (WIIUSE_USING_IR(wm)) {
		int i = 0;
		/* go through each of the 4 possible IR sources */
		for (; i < 4; ++i) {
			/* check if the source is visible */
			if (wm->ir.dot[i].visible) {
				printf("IR source %i: (%u, %u)\n", i, wm->ir.dot[i].x, wm->ir.dot[i].y);
			}
		}
		printf("IR cursor: (%u, %u)\n", wm->ir.x, wm->ir.y);
		printf("IR z distance: %f\n", wm->ir.z);
	}

}



short any_wiimote_connected(wiimote** wm, int wiimotes) {
	int i;
	if (!wm) {
		return 0;
	}
	for (i = 0; i < wiimotes; i++) {
	/*	if (wm[i] && WIIMOTE_IS_CONNECTED(wm[i])) {
			return 1;
		}*/
	}
	return 0;
}

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
	
	wiiuse_motion_sensing(wiimotes[0], 1); // active accelerometre
	wiiuse_set_ir(wiimotes[0], 1); // active IR
	wiiuse_set_aspect_ratio(wiimotes[0], WIIUSE_ASPECT_16_9); 
	wiiuse_set_ir_position(wiimotes[0], WIIUSE_IR_ABOVE); 
	wiiuse_set_ir_vres(wiimotes[0], 1066, 600); 
		
	while (true) {
		if (wiiuse_poll(wiimotes, MAX_WIIMOTES)) {
	/*
	* This happens if something happened on any wiimote.
	* So go through each one and check if anything happened.
	*/

			for (int i = 0; i < MAX_WIIMOTES; ++i) {
				switch (wiimotes[i]->event) {
					case WIIUSE_EVENT:
						/* a generic event occurred */
						handle_event(wiimotes[i]);
						break;
					default: 
						cout << "default " << endl;
						break;
				}
			}
		}
	}
  //Timout des wiimotes, valeur max pour éviter d'avoir des messages d'erreur
  wiiuse_set_timeout(wiimotes,MAX_WIIMOTES,0xFF,0xFF);
 
  //sleep(10);
 
  //Disconnect the wiimotes
  wiiuse_cleanup(wiimotes, MAX_WIIMOTES);
 

  return 0;
}	
