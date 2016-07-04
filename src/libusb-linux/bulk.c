#include <errno.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "libusb.h"
#define EP_1			(1 | LIBUSB_ENDPOINT_IN)
#define EP_2			(2 | LIBUSB_ENDPOINT_IN)
#define EP_3			(3 | LIBUSB_ENDPOINT_IN)
#define EP_4			(4 | LIBUSB_ENDPOINT_IN)
#define EP_5			(5 | LIBUSB_ENDPOINT_IN)
#define EP_6			(6 | LIBUSB_ENDPOINT_IN)
#define EP_7			(7 | LIBUSB_ENDPOINT_IN)
static struct libusb_device_handle *devh = NULL;
unsigned char *rcvbuf[7];
static struct libusb_transfer *img_transfer[7] = {NULL};
static int do_exit = 0;


static int find_device(void)
{
	devh = libusb_open_device_with_vid_pid(NULL, 0x0483, 0x5710);
	return devh ? 0 : -EIO;
}

static void sighandler(int signum)
{
	do_exit = 1;
}
static int save_to_file(int id,unsigned char *data,int len)
{
	FILE *fd;
	char filename[64];

	snprintf(filename, sizeof(filename), "EP_%d.dat", id);
	fd = fopen(filename, "a");
	if (!fd)
		return -1;

	(void) fwrite(data, 1, len, fd);
	fclose(fd);
	return 0;
}
void printf_bulk(int EP,int id)
{
	int size=0,i=0;
	int r = libusb_bulk_transfer(devh, EP, (unsigned char*)rcvbuf[id], 1024, &size, 5000);
	if (r < 0)
	{
		printf("libusb_bulk_transfer failed: %s\n", libusb_error_name(r));
		//return;
	}	
	else
	{
		//printf("EP_%x Got %d bytes\n",EP,size);
		//for(i=0;i<size;i++)
		//	printf("%c",rcvbuf[id][i]);
		//printf("\n");
		save_to_file(id,rcvbuf[id],size);
		libusb_bulk_transfer(devh, EP-0x80, (unsigned char*)rcvbuf[id], 1024, &size, 5000);
	}
}
int main(void)
{
	struct sigaction sigact;
	int r = 1;
	int i = 0;
	int size=0;

	r = libusb_init(NULL);
	if (r < 0) {
		fprintf(stderr, "failed to initialise libusb\n");
		exit(1);
	}

	r = find_device();
	if (r < 0) {
		fprintf(stderr, "Could not find/open device\n");
		goto out;
	}
	for(i=0;i<7;i++)
	{
		rcvbuf[i]=(unsigned char *)malloc(1024);
		r = libusb_claim_interface(devh, i);
		if (r < 0) {
			fprintf(stderr, "usb_claim_interface error %d\n", r);
			goto out;
		}
	}
	printf("claimed interface\n");
	sigact.sa_handler = sighandler;
	sigemptyset(&sigact.sa_mask);
	sigact.sa_flags = 0;
	sigaction(SIGINT, &sigact, NULL);
	sigaction(SIGTERM, &sigact, NULL);
	sigaction(SIGQUIT, &sigact, NULL);
	while (!do_exit) 
	{
		for(i=0;i<7;i++)
			printf_bulk((i+1)|LIBUSB_ENDPOINT_IN,i);
	}
out_release:
	for(i=0;i<7;i++)
		libusb_release_interface(devh, i);
out:
	libusb_close(devh);
	libusb_exit(NULL);
}
