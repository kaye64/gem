/**
 *  This file is part of Gem.
 *
 *  Gem is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Gem is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Gem.  If not, see <http://www.gnu.org/licenses/\>.
 */

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
