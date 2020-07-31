#include "mbed.h"

DigitalOut blueLED(D4);
DigitalOut buttonON(D8);
AnalogIn   ain(A0);


//#define BLINKING_RATE     500ms

//Testing update for GitHub
//Testing update for GitHub2
int main() {

    //button on and off
    /*while(true) {
        if(buttonON) {
            if(blueLED == 1) {
                blueLED = 0;
            }
            else {
                blueLED = 1;
            }
        }
    }*/

    //AnaLogIn control
    while (1) {
        if (ain > 0.3f) {
            blueLED = 1;
        } else {
             blueLED = 0;
        }
    }

}