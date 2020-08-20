#include "mbed.h"
#include "DHT.h" //DHT Library 
#include "stm32746g_discovery_lcd.h" //Touch Screen
#include "stm32746g_discovery_ts.h" //Touch Screen
#include "Grove_LCD_RGB_Backlight.h" //Backlight display



/*

TODO, FIX LCD SCREEN TO LIVE UPDATE.

*/





//GLOBAL
DHT Sensor1 (A0, SEN51035P); //Defining the Humidity Temperature sensor 1
DHT Sensor2 (A1, SEN51035P); //Defining the Humidity Temperature sensor 2
DigitalOut myLED (D2); //Defining the Red led. [GOOD]
DigitalOut myLED2 (D3); //Defining the Green led. [HOT]
DigitalOut myLED3 (D4); //Defining the Blue led. [COLD]
DigitalOut fucking_buzzer (D5); //Defining the total nightmare
AnalogIn Loudness_sensor (A3); //Loudness sensor instead of sound sensor
DigitalIn button (D7); // Button input
DigitalIn SwitchRoom (D8); //Button to switch room
Grove_LCD_RGB_Backlight mini_display (D14, D15); // Backlight displayed



//Threads
Thread ss_thread;
Thread room1_thread;
Thread room2_thread;
Thread Print_To_Screen;
Thread temp_checking;


//Voids
void soundSensor();
int loud_noises; //Tracks the amount of times it went over the noise level

void room1();
void room2();
void PrintToScreen();
void temp_check();

//Global Variables
int wait = 1000000;

//Temperature Levels
float cold = 29;
float hot = 30;


//Room 1
int err1;
float r1_temp;
float r1_humid;

//Room 2
int err2;
float r2_temp;
float r2_humid;

//Rooms
int roomnr = 0;

char room_1[25]; //room1 text to TouchScreen
char temp_1[25]; //Temp text to TouchScreen
char humid_1[25]; //Temp text to TouchScreen
    
char room_2[25]; //room2 text to TouchScreen
char temp_2[25]; //Temp text to TouchScreen
char humid_2[25]; //Temp text to TouchScreen

char noise[25]; //Noise text to TouchScreen

//Small raw database
//Currently not in use
/*struct location_information
{
    char room[50]; //area within the building
    float noise[10]; //10 due to Calvin 
};
*/

// main() runs in its own thread in the OS
int main()
{
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, LCD_FB_START_ADDRESS);
    BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);
    BSP_LCD_Clear(LCD_COLOR_BLACK);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);

    //Uses the thread
    ss_thread.start(soundSensor);
    room1_thread.start(room1);
    room2_thread.start(room2);
    Print_To_Screen.start(PrintToScreen);
    temp_checking.start(temp_check);

    char char1[25]; //Temp text to TouchScreen
    char char2[25]; //Noise text to TouchScreen
    
    while (true)
    {
        //Havne't found a use for this yet
    }
    wait_us(wait);
}

//Checks the noise level in the room (supposed to)
//The Loudness Sensor is VERY VERY hard to adjust when it comes to the frequence. 
void soundSensor()
{
    
    char loud_text[25];
    float loud_noise = 0.4f; //A set float for the noise levle to be above, before triggering.

    while (true)
    {
        
        //Loudness sensor function
        //if button, reset the noise counter
        //else if sensor is above loud_nose level proceed
        if(button)
        {
            loud_noises = 0;
        }
        else if (Loudness_sensor > loud_noise)
        {
            //printf("\n add noise %i \n", loud_noises); //Debugging
            loud_noises++;
        }


        if (loud_noises > 15) {
            //printf("\n fucking buzzer \n"); //Debugging
            fucking_buzzer = 1;
            myLED2 = 0; //Turns blue LED off

        }
        else 
        {
            fucking_buzzer = 0;
            //printf("\n fucking_buzzer else\n"); //DEBUGGING
            myLED2 = 1; //Turns green LED on
        }

        //Bring data to Grove_LCD_RGB_Backlight
        mini_display.clear();
        sprintf(loud_text, "Noise: %i",loud_noises);
        mini_display.print(loud_text);
        
        wait_us(wait);
    }
}


//Room 1 void threaded
void room1()
{
    while(true)
    {
        err1 = Sensor1.readData();
        r1_temp = Sensor1.ReadTemperature(CELCIUS);
        r1_humid = Sensor1.ReadHumidity();

        //printf("\n Update room 1 \n"); // debugging

        wait_us(wait);
    }
}

//Room 2 void threaded
void room2()
{
    while(true)
    {


        err2 = Sensor2.readData();
        r2_temp = Sensor2.ReadTemperature(CELCIUS);
        r2_humid = Sensor2.ReadHumidity();

        //printf("\n Update room 2 \n"); //Debugging

        wait_us(wait);
    }
}

void PrintToScreen ()
{
    while(true)
    {
        //Currently using same temperature sensor
        sprintf(room_1, "Office: 1");
        sprintf(temp_1, "Temp: %0.0f",r1_temp);
        sprintf(humid_1, "Humid: %0.0f",r1_humid);
        sprintf(noise, "Noise: %i",loud_noises);
        sprintf(room_2, "Office: 2");
        sprintf(temp_2, "Temp: %0.0f",r2_temp);
        sprintf(humid_2, "Humid: %0.0f",r2_humid);
        sprintf(noise, "Noise: %i",loud_noises);
        
        if(SwitchRoom)
        {
            roomnr++;
            BSP_LCD_Clear(LCD_COLOR_BLACK); //Clear upon button press
            if(roomnr == 3)
            {
                roomnr = 0;
            }
        }

        switch(roomnr)
        {
            case 0:
                BSP_LCD_DisplayStringAt(0, LINE(1), (uint8_t *)room_1, LEFT_MODE);
                BSP_LCD_DisplayStringAt(0, LINE(2), (uint8_t *)temp_1, LEFT_MODE);
                BSP_LCD_DisplayStringAt(0, LINE(3), (uint8_t *)humid_1, LEFT_MODE);
                BSP_LCD_DisplayStringAt(0, LINE(4), (uint8_t *)noise, LEFT_MODE);

                BSP_LCD_DisplayStringAt(0, LINE(1), (uint8_t *)room_2, RIGHT_MODE);
                BSP_LCD_DisplayStringAt(0, LINE(2), (uint8_t *)temp_2, RIGHT_MODE);
                BSP_LCD_DisplayStringAt(0, LINE(3), (uint8_t *)humid_2, RIGHT_MODE);
                BSP_LCD_DisplayStringAt(0, LINE(4), (uint8_t *)noise, RIGHT_MODE);
            break;
                
            case 1:
                BSP_LCD_DisplayStringAt(0, LINE(1), (uint8_t *)room_1, CENTER_MODE);
                BSP_LCD_DisplayStringAt(0, LINE(2), (uint8_t *)temp_1, CENTER_MODE);
                BSP_LCD_DisplayStringAt(0, LINE(3), (uint8_t *)humid_1, CENTER_MODE);
                BSP_LCD_DisplayStringAt(0, LINE(4), (uint8_t *)noise, CENTER_MODE);
            break;

            case 2:
                BSP_LCD_DisplayStringAt(0, LINE(1), (uint8_t *)room_2, CENTER_MODE);
                BSP_LCD_DisplayStringAt(0, LINE(2), (uint8_t *)temp_2, CENTER_MODE);
                BSP_LCD_DisplayStringAt(0, LINE(3), (uint8_t *)humid_2, CENTER_MODE);
                BSP_LCD_DisplayStringAt(0, LINE(4), (uint8_t *)noise, CENTER_MODE);
            break;
        }
        wait_us(wait);
    }
}

void temp_check()
{
    while(true)
    {
        //If temp is above hot (30C)
        if(r1_temp > hot || r2_temp > hot)
        {
            if(r1_temp > hot)
            {
                //printf("\n test 1 \n"); //Debugging
                BSP_LCD_DisplayStringAt(0, LINE(7), (uint8_t *)"                  ", CENTER_MODE);
                BSP_LCD_DisplayStringAt(0, LINE(5), (uint8_t *)"Room 1 too hot", CENTER_MODE);
                myLED = 1; //Turns red LED on
            }
            if(r2_temp > hot)
            {
                //printf("\n test 2 \n"); //Debugging
                BSP_LCD_DisplayStringAt(0, LINE(8), (uint8_t *)"                  ", CENTER_MODE);
                BSP_LCD_DisplayStringAt(0, LINE(6), (uint8_t *)"Room 2 too hot", CENTER_MODE);
                myLED = 1; //Turns red LED on
            }
            if (r1_temp > hot && r2_temp > hot)
            {
                //printf("Red LED off MyLED3"); //debugging
                myLED3 = 0; //Turns blue LED off
            }
        }
        else 
        {
            //printf("\n clear line 1 \n"); Debugging
            BSP_LCD_DisplayStringAt(0, LINE(5), (uint8_t *)"                  ", CENTER_MODE);
            BSP_LCD_DisplayStringAt(0, LINE(6), (uint8_t *)"                  ", CENTER_MODE);
            myLED = 0;
        }

        //If temp is above below cold (28C)
        if(r1_temp < cold || r2_temp < cold)
        {
            if(r1_temp < cold)
            {
                //printf("\n test 3 \n"); //Debugging
                BSP_LCD_DisplayStringAt(0, LINE(5), (uint8_t *)"                  ", CENTER_MODE);
                BSP_LCD_DisplayStringAt(0, LINE(7), (uint8_t *)"Room 1 too cold", CENTER_MODE);
                myLED3 = 1; //Turns blue LED on
            }
            if(r2_temp < cold)
            {
                //printf("\n test 4 \n"); //Debugging
                BSP_LCD_DisplayStringAt(0, LINE(6), (uint8_t *)"                  ", CENTER_MODE);
                BSP_LCD_DisplayStringAt(0, LINE(8), (uint8_t *)"Room 2 too cold", CENTER_MODE);
                myLED3 = 1; //Turns blue LED on
            }
            if (r1_temp < cold && r2_temp < cold)
            {
                //printf("Red LED off MyLED"); //debugging
                myLED = 0; //Turns red LED off
            }
        }
        else 
        {   
            //printf("\n clear line 2 \n"); Debugging
            BSP_LCD_DisplayStringAt(0, LINE(7), (uint8_t *)"                  ", CENTER_MODE);
            BSP_LCD_DisplayStringAt(0, LINE(8), (uint8_t *)"                  ", CENTER_MODE);
            myLED3 = 0; //turn off blue LED
        }
        
    wait_us(wait);
    }
}