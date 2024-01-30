#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

#define ADC_MEASUREMENT_UNIT  ADC_UNIT_1   // ADC_UNIT_2 DOES NOT WORK WITH WIFI 
#define ADC_MEASUREMENT_CH    ADC_CHANNEL_6
     
#define NUMBER_OF_SAMPLES   64       

extern adc_oneshot_unit_handle_t adc_handle;
extern adc_cali_handle_t         adc_cali_handle;
extern bool                      do_calibration ;


extern int  new_measured_value;
extern int  last_measured_value;
extern bool adc_value_changed;

void init_analog_measurement_handler();
void start_analog_measuring(void *pvParameter);
void measure_analog_input  (adc_channel_t adc_channel , int *new_value , int *last_value , bool *value_changed , int number_of_sample);
bool adc_calibration_init  (adc_unit_t        unit, adc_atten_t atten, adc_cali_handle_t *out_handle);
void adc_calibration_deinit(adc_cali_handle_t handle);