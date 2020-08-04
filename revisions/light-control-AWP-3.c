#include "mbed.h"
//#include "soundsensor.h"
//#include "lightsensor.h"
//#include "GROVE_LCD_RGB_Blacklight.h"
/*

In and Output currently not working due to embedded controller not being connected.

 */
// main() runs in its own thread in the OS
int main()
{

    //Input Outputs
    //Non-global voids haven't been used.
    AnalogIn pot(A0);
    DigitalOut blueLED(D4);
    DigitalOut greenLED(D3);
    DigitalOut redLED(D2);
    DigitalOut button(D8);
    AnalogIn sound(A1);

    //Int variables
    //Non-global voids haven't been used.
    int trigger = 1;
    int stop_trigger;
    int wait = 200000;

    //Test message
    printf("\nDebugging test\n");

    while (1) {
        //Trigger timer based
        if(trigger < 10) {
            trigger++;
            wait_us(wait);
            printf("value = %i \n", trigger);
        }
        //timer based access
        if(trigger >= 10) {
            if(pot > 0.3f && pot < 0.6f) {
                greenLED = 1;
                blueLED = 0;
                redLED = 0;
                printf("Level: 1 \r\n");
            }
            if (pot > 0.6f && pot < 0.8f) {
                greenLED = 0;
                blueLED = 1;
                redLED = 0;
                printf("Level: 2 \r\n");
            }
            if (pot > 0.9f && pot < 1) {
                greenLED = 0;
                blueLED = 0;
                redLED = 1;
                printf("Level: 3 \r\n");

            }
            //ressetting
            if(button == 1){
                stop_trigger++;
                trigger = 0;
                wait_us(wait);
            }
        }
    }
}