# SEHT-Macflood
Scorpion Ethical Hacking Tools - MAC flood. This tool fills the CAM table of a destination Network switch, forcing it into failopen mode. You may choose between using the ARP protocol, or using no protocol at all.
<br/><br/>
Usage:
<br/><br/>
<device> <interval> <three_octet_prefix> <broadcast_option>
<br/><br/>
Breakdown:
<br/>**<device>**: The device you would like to use for this attack  
<br/>**<interval>**: An interval in seconds to sleep for between packet injection or -1 for a continuous loop
<br/>**<ip_prefix>**: A three octet prefix if your will use the ARP protocol with this attack, set to 0 if you are not using option 2
<br/>**<broadcast_option>**: Broadcast option allows you to set options relating to the destination/source MAC and IP addresses:  
<br/>0 = Broadcast destinations only (send packets to broadcast addresses)
<br/>1 = Generate random MAC addresses for the source and destination including random IP addresses based on the prefix
<br/>2 = Include no upper layer protocol, in such case sends an ETH packet with the upper layer protocol set to 0
