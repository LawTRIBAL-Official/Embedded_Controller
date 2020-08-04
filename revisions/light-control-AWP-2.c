#include "mbed.h"
 

 //OLD ARDUINO PROGRAM FOR DIMMER LIGHTS WITH
 //ROTARY ANGLE SENSOR
 
#define ROTARY_ANGLE_SENSOR A0 
#define LED 2 
#define ABC_VREF 5 
#define GROVE_VVC 5
#define FULL_ANGLE 300
 
AnalogIn mySensor(ROTARY_ANGLE_SENSOR);
 
// main() runs in its own thread in the OS
 
int pinsInIt();
int getDegree();
 
int main()
{
 
    while(true) {
        int degrees;
        degrees = getDegree();
        printf("The angle between the mark and the starting position:");
        printf(degrees);
 
        int brightness;
        /*The degrees is 0~300, should be converted to be 0~255 to control the*/
        /*brightness of LED                                                   */
        brightness = map(degrees, 0, FULL_ANGLE, 0, 255); 
        controlBrightness(brightness);
 
        wait_us(500000000);
    }
}
 
void pinsInit() {
    pin_mode(ROTARY_ANGLE_SENSOR, INPUT);
    pin_mode(LED, OUTPUT);
}
 
 void controlBrightness(int brightness) {
    analogWrite(LED,brightness);
}
 
int getDegree() {
    int sensor_value = mySensor.read_u16();
    float voltage;
    voltage = (float)sensor_value*ADC_VREF/1023; 
    float degrees = (voltage*FULL_ANGLE)/GROVE_VVC;
    return degrees;
}


