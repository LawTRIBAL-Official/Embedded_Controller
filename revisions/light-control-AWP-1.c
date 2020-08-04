#include "mbed.h"
 
 
AnalogIn pot(A0);
 /*
 
 In and Output currently not working due to embedded controller not being connected.
 
  */
// main() runs in its own thread in the OS
int main()
{
    DigitalOut blueLED(D4);
    DigitalOut greenLED(D3);
    DigitalOut redLED(D2);
    DigitalOut buttonON(D8);
 
    float ain;
 
    printf("\nDebugging test\n");
 
    while (1) {
 
        ain = pot.read();  
 
        if(pot > 0.3f && pot < 0.6f){
            greenLED = 1; 
            blueLED = 0;
            redLED = 0;
            
            printf("%f \r\n", ain);
        }
        if (pot > 0.6f && pot < 0.9f){
            greenLED = 0;
            blueLED = 1;
            redLED = 0;
            printf("%f \r\n", ain);
        }
        if (pot > 0.9f && pot < 1) {
            greenLED = 0;
            blueLED = 0;
            redLED = 1;
            printf("%f \r\n", ain);        
        }
        if (buttonON = 1) {
            if (buttonON == 1 {
                greenLED = 1;
                blueLED = 1;
                redLED = 1;
            }
            else {
                greenLED = 1;
                blueLED = 1;
                redLED = 1;
            }
        }
        else if (pot < 0.3f) {
            greenLED = 0;
            blueLED = 0;
            redLED = 0;
        }
    }
}
 
 
 

