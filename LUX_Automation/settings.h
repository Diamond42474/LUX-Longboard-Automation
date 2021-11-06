// Color Settings
#define BASE_BLUE 5
#define BASE_RED 5

/**
 *  ADC averaging settings 
 */
// how many times the method is ran before average is updated
#define TICKS_BETWEEN_ADC_UPDATES 2048
// Multiplyer for ADC output (makes smaller values have bigger impace)
#define ACD_DIFFERENCE_AMPLIFIER 2

#define ACD_AVERAGE_AMPLIFIER 1
// Size of averaging array (smaller will let each value have greater effect)
#define ADC_AVERAGE_SIZE 32
