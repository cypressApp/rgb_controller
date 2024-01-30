
#include "context.h"

mcpwm_cmpr_handle_t comparators[3];

void pwm_main_task(void *pvParameters)
{
    ESP_LOGI(TAG, "Create timers");
    mcpwm_timer_handle_t timers[3];
    mcpwm_timer_config_t timer_config = {
        .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,
        .group_id = 0,
        .resolution_hz = RGB_TIMER_RESOLUTION_HZ,
        .period_ticks = RGB_TIMER_PERIOD,
        .count_mode = MCPWM_TIMER_COUNT_MODE_UP,
    };
    for (int i = 0; i < 3; i++) {
        ESP_ERROR_CHECK(mcpwm_new_timer(&timer_config, &timers[i]));
    }

    ESP_LOGI(TAG, "Create operators");
    mcpwm_oper_handle_t operators[3];
    mcpwm_operator_config_t operator_config = {
        .group_id = 0, // operator should be in the same group of the above timers
    };
    for (int i = 0; i < 3; ++i) {
        ESP_ERROR_CHECK(mcpwm_new_operator(&operator_config, &operators[i]));
    }

    ESP_LOGI(TAG, "Connect timers and operators with each other");
    for (int i = 0; i < 3; i++) {
        ESP_ERROR_CHECK(mcpwm_operator_connect_timer(operators[i], timers[i]));
    }

    ESP_LOGI(TAG, "Create comparators");
    // mcpwm_cmpr_handle_t comparators[3];
    mcpwm_comparator_config_t compare_config = {
        .flags.update_cmp_on_tez = true,
    };
    for (int i = 0; i < 3; i++) {
        ESP_ERROR_CHECK(mcpwm_new_comparator(operators[i], &compare_config, &comparators[i]));
        // init compare for each comparator
        ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparators[i], rgb_value[i]));
    }

    ESP_LOGI(TAG, "Create generators");
    mcpwm_gen_handle_t generators[3];
    const int gen_gpios[3] = {RED_GPIO, GREEN_GPIO, BLUE_GPIO};
    mcpwm_generator_config_t gen_config = {};
    for (int i = 0; i < 3; i++) {
        gen_config.gen_gpio_num = gen_gpios[i];
        ESP_ERROR_CHECK(mcpwm_new_generator(operators[i], &gen_config, &generators[i]));
    }

    ESP_LOGI(TAG, "Set generator actions on timer and compare event");
    for (int i = 0; i < 3; i++) {
        ESP_ERROR_CHECK(mcpwm_generator_set_actions_on_timer_event(generators[i],
                        // when the timer value is zero, and is counting up, set output to high
                        MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH),
                        MCPWM_GEN_TIMER_EVENT_ACTION_END()));
        ESP_ERROR_CHECK(mcpwm_generator_set_actions_on_compare_event(generators[i],
                        // when compare event happens, and timer is counting up, set output to low
                        MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, comparators[i], MCPWM_GEN_ACTION_LOW),
                        MCPWM_GEN_COMPARE_EVENT_ACTION_END()));
    }

    ESP_LOGI(TAG, "Start timers one by one, so they are not synced");
    for (int i = 0; i < 3; i++) {
        ESP_ERROR_CHECK(mcpwm_timer_enable(timers[i]));
        ESP_ERROR_CHECK(mcpwm_timer_start_stop(timers[i], MCPWM_TIMER_START_NO_STOP));
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    vTaskDelay(pdMS_TO_TICKS(100));

    // // Manually added this "IDLE" phase, which can help us distinguish the wave forms before and after sync
    // ESP_LOGI(TAG, "Force the output level to low, timer still running");
    // for (int i = 0; i < 3; i++) {
    //     ESP_ERROR_CHECK(mcpwm_generator_set_force_level(generators[i], 0, true));
    // }

    // ESP_LOGI(TAG, "Setup sync strategy");
    // example_setup_sync_strategy(timers);

    // ESP_LOGI(TAG, "Now the output PWMs should in sync");
    // for (int i = 0; i < 3; ++i) {
    //     // remove the force level on the generator, so that we can see the PWM again
    //     ESP_ERROR_CHECK(mcpwm_generator_set_force_level(generators[i], -1, true));
    // }
    vTaskDelay(pdMS_TO_TICKS(100));

    vTaskDelete(NULL);
}