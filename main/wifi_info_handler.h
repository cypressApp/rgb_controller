#include "esp_netif.h"

extern esp_netif_t * p_netif;

uint32_t get_device_ip_info_int();
uint32_t get_gateway_ip_info_int();
void     get_gateway_ip_info_array(int *array , uint32_t gateway_ip);
void     update_wifi_mode_ip(int tempIpSlice);
void     init_mac_address();
int      get_ip4(char *addr_str , int len);