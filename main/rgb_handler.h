#include "stdbool.h"

#define RGB_TIMER_RESOLUTION_HZ 1000000  // 1MHz, 1us per tick
#define RGB_TIMER_PERIOD        255     // 1000 ticks, 1ms
#define RED_GPIO                26       // RED
#define GREEN_GPIO              25       // GREEN
#define BLUE_GPIO               32       // BLUE 
#define RGB_VALUE_DATA_LENGTH   3
#define RGB_DATA_MAX_LENGTH     1024

extern char *get_rgb_cmd;

extern bool is_rgb_on;
extern long int rgb_value[3];

extern char *red_prefix  ;
extern char *red_suffix  ;

extern char *green_prefix;
extern char *green_suffix;

extern char *blue_prefix ;
extern char *blue_suffix ;

extern char *on_command ;
extern char *off_command;

extern int red_offset  ;
extern int green_offset;
extern int blue_offset ;

extern int get_rgb_cmd_len;

void set_power(bool value);
void set_rgb_value(int color_index , long int value);
void get_status();
void rgb_task(void *pvParameters);
bool is_rgb_command(char *rx_value , int rx_buffer_len);
void send_rgb_value(int sock);