#ifndef CHECKSUM_H
#define CHECKSUM_H

//******************Define checksum here******************/
//Reference: 
//1. Nicholas's Github
//2. http://web.cecs.pdx.edu/~jrb/tcpip/misc.src/checksum.c
//3. https://cse.usf.edu/~kchriste/tools/checksum.c

uint16_t checksum(uint16_t *data, int len)
{
	int i = len;
	uint32_t sum = 0;
	uint16_t oddbyte = 0;		
	uint16_t chcksum;		

	while (i > 1)  {
		sum += *data++;
		i -= 2;
	}
				
	if (i == 1) {
		*((u_char *) &oddbyte) = *(u_char *)data;   
		sum += oddbyte;
	}

	sum  = (sum >> 16) + (sum & 0xffff);		
	sum += (sum >> 16);				
	chcksum = ~sum;		
	
	return chcksum;
}

#endif
