#include <avr/io.h>
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