/*******************
Reference:
1. Tanishq's Github
2. Nicholas's Github
3. https://sites.google.com/site/sbobovyc/home/network/c-raw-sockets-1
******************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h> 
#include <time.h>   
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/if_packet.h> 
#include <net/ethernet.h>  
#include <net/if.h>        
#include <sys/ioctl.h>

#include "checksum.h"
#include "protocal.h"
#include "format.h"

#define BUFFER_SIZE 1000
#define IP__DF 0x4000              
#define IP__MF 0x2000             
#define FLAG_MASK 0xE000;
#define OFFSET_MASK ~FLAG_MASK;

//Check if the interface is included
int main(int argc, char *argv[])
{
    char buffer[BUFFER_SIZE];
    char new_buffer[BUFFER_SIZE];
    int buflen = 0;
    uint32_t total_size = 0;
    uint32_t new_total_size = 0;
    uint32_t header_offset; 
    uint16_t offset, flag; 

    if (argc != 2)
    {
        printf("Error: Add the interface\n");
        printf("Ex: ./my_ping end1320s0f1\n");
        exit(1);
    }
    
    printf("1. Completed: The interface is included\n");

//Create a raw socket

    int rawsock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_IP));
        if (rawsock < 0)
        {
            perror("Error in socket\n");
            exit(1);
        }

    printf("2. Completed: Create a raw socket %d\n", rawsock);
  
//Get the Interface Index

    struct sockaddr_ll sll, saddr;
    struct ifreq ifr;
    bzero(&sll, sizeof(sll));
    bzero(&ifr, sizeof(ifr));

    strncpy((char *)ifr.ifr_name, argv[1], sizeof(ifr.ifr_name));
    if((ioctl(rawsock, SIOCGIFINDEX, &ifr)) == -1) 
    {    
        perror("Error getting Interface index !\n");
        exit(1);
    }
    
    printf("3. Completed: Get the interface Index %s\n", ifr.ifr_name);
 
//Bind our raw socket to this interface

	sll.sll_family = AF_PACKET;
	sll.sll_ifindex = ifr.ifr_ifindex;
        sll.sll_protocol = htons(ETH_P_IP);

    if((bind(rawsock, (struct sockaddr *)&sll, sizeof(sll)))== -1)
    {
        perror("Error binding raw socket to interface\n");
        exit(1);
    }
    
   printf("4. Completed: Bind our raw socket to %s\n", ifr.ifr_name);

//Receiving the network packet

    while (1)
    {
        
        int saddr_len = sizeof(saddr);
        buflen = recvfrom(rawsock, buffer, BUFFER_SIZE, 0, (struct sockaddr  *)&saddr, &saddr_len);
	printf("The buffer size is %d\n", buflen);
        if (buflen < 0)
        {
            printf("Error in reading recvform function \n");
            exit(-1);
        }
        
	printf("5. Complete: Receiving the network packet\n");

// Reply (Adopted from Tanishq)
	bzero((void *)buffer, BUFFER_SIZE);

   	my_packet *packet = (my_packet* )buffer;

   	offset = ntohs(packet->ip_header.ip_offset); 
   	printf("1.%o\n", offset);
    
   	flag = offset & FLAG_MASK; 
   	printf("2.%o\n", flag);
   	 
   	offset = offset & OFFSET_MASK; 
   	printf("3.%o\n", offset);
   	 
   	if ((flag == IP__DF || flag == 0x0) && offset == 0) 
   	{ 
   	     printf("\n******Test*****\n");
   	     format_reply(packet, ETHERNET_SIZE + ntohs(packet->ip_header.ip_total_len));
   	     
             sendto(rawsock, packet, buflen, 0, (const struct sockaddr *)&saddr, sizeof(saddr));
        
             puts("Reply send.(Non-Fragment)\n");
    	}
	else
    	{
    	     memcpy(new_buffer, buffer, ETHERNET_SIZE);
    	
    	     memcpy(new_buffer + ETHERNET_SIZE, buffer + ETHERNET_SIZE, IP_SIZE);
    	
       	     header_offset = ETHERNET_SIZE + IP_SIZE;
    	
    	     new_total_size += ntohs(packet->ip_header.ip_total_len) - IP_SIZE;
    	
    	     format_fragment(new_buffer + header_offset, buffer + header_offset, offset * 8, ntohs(packet->ip_header.ip_total_len) - IP_SIZE);
    	     
    	}

    	if (flag == 0x0) 
    	{
    		total_size = (offset * 8) + ntohs(packet->ip_header.ip_total_len) - IP_SIZE;
    	}
    
    	if (total_size == new_total_size)
    	{
        	new_total_size += ETHERNET_SIZE + IP_SIZE;
           	packet = (my_packet *)new_buffer;
           	packet->ip_header.ip_total_len = htons(new_total_size - ETHERNET_SIZE);
           	packet->ip_header.ip_offset = 0x0;
           	format_reply(packet, new_total_size);
           	sendto(rawsock, packet, buflen, 0, (const struct sockaddr *)&saddr, sizeof(saddr));
           	puts("reply sent. (Fragment)");
           	bzero(new_buffer, BUFFER_SIZE);
           	total_size = 0;
           	new_total_size = 0;
       	}
    	
    	}
   
	printf("Complete: reply ICMP Echo\n");

//Closing the socket

    if(close(rawsock) == 0)
    {
        printf("Complete: Closing the socket");
    }
    else
    {
        printf("Error closing the socket! \n");
        exit(1);
    }
    
return 0;

}
