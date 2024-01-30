
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/mcpwm_prelude.h"
#include "driver/gpio.h"


extern mcpwm_cmpr_handle_t comparators[3];

void pwm_main_task(void *pvParameters);
