#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int speedtest(){

	char* URLS[] =
	{
		 "http://zgb-speedtest-1.tele2.net",
		 "http://fra36-speedtest-1.tele2.net",
		 "http://bks4-speedtest-1.tele2.net",
		 "http://vln038-speedtest-1.tele2.net",
		 "http://ams-speedtest-1.tele2.net",
		 "http://bck-speedtest-1.tele2.net",
		 "http://hgd-speedtest-1.tele2.net"
	};

	char* SIZES[] = {
			"/1MB.zip",
			"/10MB.zip",
			"/100MB.zip",
			"/1GB.zip",
			"/10GB.zip"
	};

	for(int k = 0; k < 11; k++) {
		for (int i = 1; i < 2; i++) {
			for (int j = 1; j < 2; j++) {
				char *start = "wget -O /dev/null ";
				char *url = URLS[i];
				char *size = SIZES[j];
				char buf[128];
				strcat(buf, start);
				strcat(buf, url);
				strcat(buf, size);
				printf("%s\n", buf);
				system(buf);
				strcpy(buf, "\0");
			}
		}
	}
}