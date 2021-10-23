#include <avr/io.h>
#include <util/delay.h>
#include <avr/power.h>
#include "light_automation.h"
#include "pin_defines.h"

#define AVERAGESKIP 255
uint8_t adcAverageList[255];
uint8_t index = 0;
uint8_t averageSkip = 0;
uint8_t adcAverage = 0;
void setup()
{
    //Clock
    clock_prescale_set(clock_div_1); //8MHz
    //PWM
    PWM_Config();
    //Pins
    DDRB = (1 << BLUE_LED) | (1 << RED_LED); //Set blue and red pins as output
    //ADC
    ADC_Config();
    //Setup adcAveraging
    for (uint8_t i = 0; i < 255; i++)
    {
        adcAverageList[i] = 0;
    }
}
void startupSequence()
{
    for (uint8_t ie = 0; ie < 5; ie++)
    {
        for (uint8_t i = 255; i > 0; i--)
        {
            RED_TIMER = i;
            BLUE_TIMER = i;
            _delay_ms(4);
        }
    }
}
void updateLights()
{
    if (averageSkip >= AVERAGESKIP)
    {
        averageSkip = 0;
        if (index >= 255)
        {
            index = 0;
        }
        else
        {
            adcAverageList[index] = 255 - getADC(X_Axis);
            uint16_t total = 0;
            for (uint8_t i = 0; i < 255; i++)
            {
                total += adcAverageList[i];
            }
            adcAverage = total / 255;
            index++;
        }
    }else{
        //_delay_us(500);
        averageSkip++;
    }
}
int main(void)
{
    setup();
    //startupSequence();
    while (1)
    {
        // Update Blue and Red LEDs
        //RED_TIMER = 255 - getADC(X_Axis);
        // BLUE_TIMER = 255 - getADC(Y_Axis);
        updateLights();
        RED_TIMER = adcAverage;
    }
    return 1;
}