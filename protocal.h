#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

#define IP_SIZE sizeof(IP_header)
#define ETHERNET_SIZE sizeof(Ethernet_header)


/******************Define headers here******************/

typedef struct __attribute__((__packed__)) 
{
    uint8_t icmp_type;
    uint8_t icmp_code;
    uint16_t icmp_checksum;
    uint16_t icmp_echo_ID;
    uint16_t icmp_echo_seq;
} ICMP_header;

typedef struct __attribute__((__packed__)) {
    uint8_t ip_len : 4;            
    uint8_t ip_ver : 4;           
    uint8_t ip_tos;                
    uint16_t ip_total_len;         
    uint16_t ip_ID;               
    uint16_t ip_offset;           
    uint8_t ip_TTL;                
    uint8_t ip_proto;              
    uint16_t ip_checksum;          
    uint32_t ip_src_addr;
    uint32_t ip_dst_addr;
} IP_header;

typedef struct __attribute__((__packed__)){ 
    uint8_t dst_mac[6];
    uint8_t src_mac[6];
    uint16_t ethernet_type;
} Ethernet_header;

typedef struct __attribute__((__packed__)) {
    Ethernet_header ethernet_header;
    ICMP_header icmp_header;
    IP_header ip_header;
} my_packet;

#endif
