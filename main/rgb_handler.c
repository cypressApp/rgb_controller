#include "context.h"

bool is_rgb_on = true;
long int rgb_value[3] = {255 , 255 , 255};

char *get_rgb_cmd  = "GET_RGB";

char *red_prefix   = "srp";
char *red_suffix   = "srs";
char *green_prefix = "sgp";
char *green_suffix = "sgs";
char *blue_prefix  = "sbp";
char *blue_suffix  = "sbs";

char *send_red_prefix   = "rrp";
char *send_red_suffix   = "rrs";
char *send_green_prefix = "rgp";
char *send_green_suffix = "rgs";
char *send_blue_prefix  = "rbp";
char *send_blue_suffix  = "rbs";

char *on_command   = "on" ;
char *off_command  = "off";

int red_prefix_offset   = 0;
int red_offset          = 0;
int red_suffix_offset   = 0;

int green_prefix_offset = 0;
int green_offset        = 0;
int green_suffix_offset = 0;

int blue_prefix_offset = 0;
int blue_offset        = 0;
int blue_suffix_offset = 0;

int red_prefix_len = 0;
int red_suffix_len = 0;

int green_prefix_len = 0;
int green_suffix_len = 0;

int blue_prefix_len = 0;
int blue_suffix_len = 0;

int rgb_command_len = 0;
int get_rgb_cmd_len = 0;

void set_power(bool value){
    
    is_rgb_on = value;

    if(is_rgb_on){
        for(int i = 0 ; i < 3 ; i++){
            ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparators[i], rgb_value[i]));
        }
    }else{
        for(int i = 0 ; i < 3 ; i++){
            ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparators[i], 0));
        }
    }
}

void set_rgb_value(int color_index , long int value){

    rgb_value[color_index] = value;
    if(is_rgb_on){
        ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparators[color_index], rgb_value[color_index]));
    }
    
}

void send_rgb_value(int sock){

    char *data = (char*) calloc(RGB_DATA_MAX_LENGTH , sizeof(char));

    int len = sprintf(data , "%s%ld%s%s%ld%s%s%ld%s" , 
                              send_red_prefix   , rgb_value[0] , send_red_suffix   , 
                              send_green_prefix , rgb_value[1] , send_green_suffix , 
                              send_blue_prefix  , rgb_value[2] , send_blue_suffix  );

    send(sock, data, len, 0);

}

void init_rgb_handler(){

    get_rgb_cmd_len = strlen(get_rgb_cmd);

    red_prefix_len = strlen(red_prefix);
    red_suffix_len = strlen(red_suffix);
    
    green_prefix_len = strlen(green_prefix);
    green_suffix_len = strlen(green_suffix);

    blue_prefix_len = strlen(blue_prefix);
    blue_suffix_len = strlen(blue_suffix);

    red_prefix_offset = 0;
    red_offset = red_prefix_offset + strlen(red_prefix);
    red_suffix_offset = red_offset + RGB_VALUE_DATA_LENGTH;

    green_prefix_offset = red_suffix_offset + strlen(red_suffix);
    green_offset = green_prefix_offset + strlen(green_prefix);
    green_suffix_offset = green_offset + RGB_VALUE_DATA_LENGTH;

    blue_prefix_offset = green_suffix_offset + strlen(green_suffix);
    blue_offset = blue_prefix_offset + strlen(blue_prefix); 
    blue_suffix_offset = blue_offset + RGB_VALUE_DATA_LENGTH; 

    rgb_command_len = red_prefix_len   + RGB_VALUE_DATA_LENGTH + red_suffix_len   + \
                      green_prefix_len + RGB_VALUE_DATA_LENGTH + green_suffix_len + \
                      blue_prefix_len  + RGB_VALUE_DATA_LENGTH + blue_suffix_len;

}

void rgb_task(void *pvParameters){

    init_rgb_handler();

    pwm_main_task(pvParameters);
}


bool is_rgb_command(char *rx_value , int rx_buffer_len){
    
    if(rgb_command_len != rx_buffer_len){
        return false;
    }

    if(memcmp(red_prefix , (rx_value + red_prefix_offset) , red_prefix_len) != 0){
        return false;
    }
    if(memcmp(red_suffix , (rx_value + red_suffix_offset) , red_suffix_len) != 0){
        return false;
    }
    
    if(memcmp(green_prefix , (rx_value + green_prefix_offset) , green_prefix_len) != 0){
        return false;
    }
    if(memcmp(green_suffix , (rx_value + green_suffix_offset) , green_suffix_len) != 0){
        return false;
    }

    if(memcmp(blue_prefix , (rx_value + blue_prefix_offset) , blue_prefix_len) != 0){
        return false;
    }
    if(memcmp(blue_suffix , (rx_value + blue_suffix_offset) , blue_suffix_len) != 0){
        return false;
    }

    return true;

}