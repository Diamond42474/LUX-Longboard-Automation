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
            RED_LED = 255 - i;
            BLUE_LED = i;
            _delay_ms(4);
        }
    }
    RED_LED = 0;
    BLUE_LED = 255;
    _delay_ms(500);
    RED_LED = 255;
    BLUE_LED = 0;
    _delay_ms(500);
}
int main(void)
{
    setup();
    startupSequence();
    BLUE_LED = 255;
    while (1)
    {
        
        uint8_t *average = getAverageADC();
        uint8_t X = *(average + 0);
        uint8_t Y = *(average + 1);
        
        //RED_LED = 255 - ((getADC(X_Axis)-70) * 5);
        RED_LED = 255 - X;
        BLUE_LED = 255 - Y;

        if(Y<128){
            RED_LED = 0; // full brightness
            BLUE_LED = 255 - Y * 2;
        }else{
            BLUE_LED = 0;
            RED_LED = Y * 2;
        }
        /*
        if (X < 128)
        {
            // When board is breaking
            if (Y < 128)
            {
                // When board is turning LEFT
                RED_LED = 0; // 0 = MAX
                BLUE_LED = 255 - ((2 * X) * (Y / 128));
            }
            else
            {
                // When board is turning RIGHT
                RED_LED = 0; // 0 = MAX
                BLUE_LED = 255 - (2 * (Y - 128));
            }
        }
        else
        {
            // When board is accellerating
            if (Y < 128)
            {
                // When board is turning LEFT
                RED_LED = 255 - ((-2 * X + 512) * (Y / 128));
                BLUE_LED = 0; // 0 = MAX
            }
            else
            {
                // When board is turning RIGHT
                RED_LED = 255 - (2 * (Y - 128));
                BLUE_LED = 0; // 0 = MAX
            }
        }

        /*
        if(average>=0){
            // When board is accellerating
            BLUE_LED = 255 - ((uint8_t)(average));
            RED_LED = 255;
        }else{
            // When board is breaking
            RED_LED = 255 - ((uint8_t)abs(average));
            BLUE_LED = 255;
        }
        */
        //RED_LED = 255 - getADC(MOTOR_INPUT);
    }
    return 1;
}