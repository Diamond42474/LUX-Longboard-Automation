#include <avr/io.h>
#include <util/delay.h>
#include <avr/power.h>
#include "light_automation.h"
#include "pin_defines.h"

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
}
void startupSequence()
{
    OCR0B = 250;
    BLUE_TIMER = 250;
    _delay_ms(1000);
    RED_TIMER = 10;
    BLUE_TIMER = 10;
    _delay_ms(1000);
    for (uint8_t ie = 0; ie < 5; ie++)
    {
        for (uint8_t i = 0; i < 255; i++)
        {
            RED_TIMER = i;
            BLUE_TIMER = i;
            _delay_ms(4);
        }
    }
}
int main(void)
{
    setup();
    startupSequence();
    while (1)
    {
        // Update Blue and Red LEDs
        RED_TIMER = getADC(X_Axis);
        BLUE_TIMER = getADC(Y_Axis);
    }
    return 1;
}