#include <avr/io.h>
#include <util/delay.h>
#include <avr/power.h>
#include "light_automation.h"
#include "pin_defines.h"
#include "settings.h"
void setup()
{
    // Clock
    clock_prescale_set(clock_div_1); //8MHz
    // PWM
    PWM_Config();
    // Pins
    DDRB = (1 << BLUE_PIN) | (1 << RED_PIN); //Set blue and red pins as output
    // Lights
    lightsSetup();
}
void startupSequence()
{
    for (uint8_t ie = 0; ie < 1; ie++)
    {
        for (uint8_t i = 255; i > 0; i--)
        {
            RED_LED = i;
            BLUE_LED = i;
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
        int8_t average = getAverageADC();

        if(average>=0){
            // When board is accellerating
            BLUE_LED = 255 - ((uint8_t)(average));
            RED_LED = 255;
        }else{
            // When board is breaking
            RED_LED = 255 - ((uint8_t)abs(average));
            BLUE_LED = 255;
        }
        //RED_LED = 255 - getADC(MOTOR_INPUT);
    }
    return 1;
}