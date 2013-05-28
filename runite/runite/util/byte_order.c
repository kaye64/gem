#include <runite/util/byte_order.h>

#include <netinet/in.h>

uint16_t to_host_order_s(uint16_t s)
{
	return ntohs(s);
}
uint32_t to_host_order_tri(uint32_t t)
{
	return ntohl(t) >> 8;
}
uint32_t to_host_order_i(uint32_t i)
{
	return ntohl(i);
}

uint16_t to_net_order_s(uint16_t s)
{
	return htons(s);
}
uint32_t to_net_order_tri(uint32_t t)
{
	return htonl(t);
}
uint32_t to_net_order_i(uint32_t i)
{
	return htonl(i);
}
