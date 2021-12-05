#include <avr/io.h>
#include "settings.h"
#include "pin_defines.h"
// Values for
uint8_t adcAverageList[ADC_AVERAGE_AXES][ADC_AVERAGE_SIZE];
uint16_t index = 0;
uint16_t averageSkip = 0;
uint8_t adcAverage[ADC_AVERAGE_AXES] = {0, 0};

void lightsSetup()
{
    PWM_Config();
    ADC_Config();
    averageADC_Config();
}
void PWM_Config()
{
    TCCR0A = 0x00; //Normal mode
    //TCCR0A |= (1 << WGM00) | (1 << COM0A1) | (1 << COM0B1) | (1 << COM0A0) | (1 << COM0B0);
    // PWM Phase Correct
    TCCR0A |= (0 << WGM02) | (0 << WGM01) | (1 << WGM00) | (1 << COM0A1) | (1 << COM0B1) | (1 << COM0A0) | (1 << COM0B0);
    TCCR0B = 0x00;
    TCCR0B |= (1 << CS00);
    TCNT0 = 0;
}
void ADC_Config()
{
    // 0000 PB5
    // 0001 PB2
    // 0010 PB4
    // 0011 PB3

    ADMUX =
        (1 << ADLAR) | // left shift result
        (0 << REFS1) | // Sets ref. voltage to VCC, bit 1
        (0 << REFS0) | // Sets ref. voltage to VCC, bit 0

        (0 << MUX3) |
        (0 << MUX2) |
        (1 << MUX1) |
        (1 << MUX0);

    ADCSRA =
        (1 << ADEN) |  // Enable ADC
        (1 << ADPS2) | // set prescaler to 64, bit 2
        (1 << ADPS1) | // set prescaler to 64, bit 1
        (0 << ADPS0);  // set prescaler to 64, bit 0
}
void ADC_Update()
{

    ADCSRA |= (1 << ADSC); // start ADC measurement
    while (ADCSRA & (1 << ADSC))
        ; // wait till conversion complete
}
void ADC_Set_Input(uint8_t channel)
{
    ADMUX = (0xf0 & ADMUX) | channel;
}
uint8_t getADC(uint8_t pin)
{
    ADMUX = (0xf0 & ADMUX) | pin;
    ADC_Update();
    return ADCH;
}
/**
 * @brief Populates the average array with 0s
 * 
 */
void averageADC_Config()
{
    for (uint8_t i = 0; i < ADC_AVERAGE_AXES; i++)
    {
        for (uint32_t ie = 0; ie < ADC_AVERAGE_SIZE; ie++)
        {
            adcAverageList[i][ie] = 0;
        }
    }
}
/**
 * @brief Updates the average value from the adc 
 * 
 * 
 */
void updateAverageADC()
{
    if (averageSkip >= TICKS_BETWEEN_ADC_UPDATES)
    {
        averageSkip = 0;
        if (index >= ADC_AVERAGE_SIZE)
        {
            index = 0;
        }
        else
        {
            // Add new index
            uint8_t oY = getADC(Y_Axis);
            uint8_t oX = getADC(X_Axis);
            uint8_t X = ((getADC(X_Axis)-80) * 42);
            uint8_t Y = ((getADC(Y_Axis)-78) * 42);
            if(oY<78){
                Y = 0;
            }else if(oY>84){
                Y = 255;
            }

            if(oX<80){
                X = 0;
            }else if(oX>86){
                X = 255;
            }

            adcAverageList[0][index] = X;
            adcAverageList[1][index] = Y;

            // Calculate average
            for (uint8_t i = 0; i < ADC_AVERAGE_AXES; i++)
            {
                int32_t total = 0;
                for (uint16_t ie = 0; ie < ADC_AVERAGE_SIZE; ie++)
                {
                    total += adcAverageList[i][ie];
                }
                adcAverage[i] = (total / ADC_AVERAGE_SIZE);
            }
            index++;
        }
    }
    else
    {
        averageSkip++;
    }
}
/**
 * @brief Get the Average of ADC
 * 
 * @return uint8_t Averaged value 
 */
uint8_t * getAverageADC()
{
    updateAverageADC();
    adcAverage[0] *= ACD_AVERAGE_AMPLIFIER;
    adcAverage[1] *= ACD_AVERAGE_AMPLIFIER;
    return adcAverage;
}