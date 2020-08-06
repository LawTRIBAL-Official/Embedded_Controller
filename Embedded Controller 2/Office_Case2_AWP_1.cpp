#include "mbed.h"
#include "DHT.h" //DHT Library 
 
 
DHT Sensor (A0,SEN51035P); //Defining the sensor with it's name
DigitalOut myLED (D2); //Defining the led.
 
 
 
 
// main() runs in its own thread in the OS
int main()
{
 
    int err;
    err = Sensor.readData();
 
    //Debugging message for int initialization. 
    printf("\r Debugging Test message");
    printf("\r **********************");
    
    while (1) {
        myLED = 1;
        err = Sensor.readData();
 
        if (err == 0) {
 
            //Test print messages
            printf("Temperature is %4.2f C \r\n",Sensor.ReadTemperature(CELCIUS));
            printf("Temperature is %4.2f F \r\n",Sensor.ReadTemperature(FARENHEIT));
            printf("Temperature is %4.2f K \r\n",Sensor.ReadTemperature(KELVIN));
            printf("Humidity is %4.2f \r\n",Sensor.ReadHumidity());
            printf("Dew point is %4.2f  \r\n",Sensor.CalcdewPoint(Sensor.ReadTemperature(CELCIUS), Sensor.ReadHumidity()));
            printf("Dew point (fast) is %4.2f  \r\n",Sensor.CalcdewPointFast(Sensor.ReadTemperature(CELCIUS), Sensor.ReadHumidity()));
        } else
        
        //Turns LED off, once not updating.
        myLED = 0;
        wait_us(100000);
 
    }
}
 
 
