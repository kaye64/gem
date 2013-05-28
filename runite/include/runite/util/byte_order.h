#ifndef _BYTEORDER_H_
#define _BYTEORDER_H_

#include <stdint.h>

uint16_t to_host_order_s(uint16_t s);
uint32_t to_host_order_tri(uint32_t t);
uint32_t to_host_order_i(uint32_t i);

uint16_t to_net_order_s(uint16_t s);
uint32_t to_net_order_tri(uint32_t t);
uint32_t to_net_order_i(uint32_t i);

#endif /* _BYTEORDER_H_ */
