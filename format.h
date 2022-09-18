#ifndef FORMAT_H
#define FORMAT_H

#include <stdint.h>
#include "protocal.h"

/*******************Formatting header (Adapted from Tanishq)*******************/

static void icmp_format_reply_header(ICMP_header *icmp_header, uint32_t size_of_icmp_packet) {

    icmp_header->icmp_type = 0;
    icmp_header->icmp_code = 0;
    icmp_header->icmp_checksum = 0;
    icmp_header->icmp_checksum = checksum((uint16_t *)(&icmp_header), size_of_icmp_packet);
}

static void ip_format_reply_header(IP_header *ip_header, uint32_t size_of_ip_header) {
    
    static uint16_t ID = 0;
    uint32_t new_dst_addr = ip_header->ip_src_addr;

    ip_header->ip_ID = ID++;
    ip_header->ip_src_addr = ip_header->ip_dst_addr;
    ip_header->ip_dst_addr = new_dst_addr;
    ip_header->ip_checksum = 0;
    ip_header->ip_checksum = checksum((uint16_t *)(&ip_header), size_of_ip_header);
}

static void ether_format_reply_header(Ethernet_header *ethernet_header) {
    
    uint8_t new_dst_mac[6];
    memcpy(new_dst_mac, ethernet_header->src_mac, 6);
    memcpy(ethernet_header->src_mac, ethernet_header->dst_mac, 6);
    memcpy(ethernet_header->dst_mac, new_dst_mac, 6);
}

static void format_reply(my_packet *frame, uint32_t size_of_frame_in_bytes) {
    ether_format_reply_header(&frame->ethernet_header);
    ip_format_reply_header(&frame->ip_header, IP_SIZE);
    icmp_format_reply_header(&frame->icmp_header, size_of_frame_in_bytes - ETHERNET_SIZE - IP_SIZE);
}

static void format_fragment(uint8_t *buffer_dst, uint8_t *buffer_src, uint16_t start, uint16_t len) {
    if (buffer_dst == NULL || buffer_src == NULL)
        return;
    memcpy(buffer_dst + start, buffer_src, len);
    printf("test completed.\n");
}

#endif
