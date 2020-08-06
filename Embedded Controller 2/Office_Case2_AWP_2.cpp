#include "mbed.h"
#include "DHT.h" //DHT Library 
#include "stm32746g_discovery_lcd.h" //Touch Screen
#include "stm32746g_discovery_ts.h" //Touch Screen

//GLOBAL
DHT Sensor (A0,SEN51035P); //Defining the sensor with it's name
DigitalOut myLED (D2); //Defining the led.
DigitalOut myLED2 (D3);
AnalogIn Loudness_sensor (A3);

Thread thread;


void soundSensor_thread();

//Small raw database
struct location_information
{
    char bulding[50]; //building
    char room[50]; //area within the building
    float temp[10]; //10 due to Calvin 
};

// main() runs in its own thread in the OS
int main()
{

    thread.start(soundSensor_thread);

    int err;
    err = Sensor.readData();

    //Debugging message for int initialization. 
    printf("\r Debugging Test message");
    printf("\r **********************");
    
    while (1)
    {
        myLED = 1;
        err = Sensor.readData();

        if (err == 0)
        {
            //Test print message    
            printf("Temperature is %4.2f C \r\n",Sensor.ReadTemperature(CELCIUS));

            /*
            printf("Temperature is %4.2f F \r\n",Sensor.ReadTemperature(FARENHEIT));
            printf("Temperature is %4.2f K \r\n",Sensor.ReadTemperature(KELVIN));
            printf("Humidity is %4.2f \r\n",Sensor.ReadHumidity());
            printf("Dew point is %4.2f  \r\n",Sensor.CalcdewPoint(Sensor.ReadTemperature(CELCIUS), Sensor.ReadHumidity()));
            printf("Dew point (fast) is %4.2f  \r\n",Sensor.CalcdewPointFast(Sensor.ReadTemperature(CELCIUS), Sensor.ReadHumidity()));
            */
        } else

        //Initialize if current temp is above max temp
        
        
        //Turns LED off, once not updating.
        myLED = 0;
        wait_us(100000);
    }
}

//Checks the noise level in the room (supposed to)
//The Loudness Sensor is VERY VERY hard to adjust when it comes to the frequence. 
void soundSensor_thread()
{
    //Variables used 
    int loud_noises; //Tracks the amount of times it went over the noise level
    float loud_noise = 0.2f; //A set float for the noise levle to be above, before triggering.

    while (true)
    {
        //If the Loudsensors float is above the loud_noise limit, trigger function.
        if(Loudness_sensor > loud_noise)
        {
            myLED2 = 1;  
            printf("Loudness_sensor: %f \r\n", Loudness_sensor.read());
            wait_us(100000);
            printf("\r \n Sound Works \r \n");
            myLED2 = 0; 
            loud_noises++; //Adds to loud_noise counter.
            printf("LOUD NOISES; %i \r\n",loud_noises);
        }
    }
}

