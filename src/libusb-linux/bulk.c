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
static void LIBUSB_CALL cb_1(struct libusb_transfer *transfer)
{
	int i;
	if (transfer->status != LIBUSB_TRANSFER_COMPLETED) {
		fprintf(stderr, "img transfer status %d?\n", transfer->status);
		libusb_free_transfer(transfer);
		img_transfer[0] = NULL;
		return;
	}

	printf("cb_1 callback\n");
	for(i=0;i<1024;i++)
		printf("%c",rcvbuf[0][i]);
	printf("\r\n");
	libusb_submit_transfer(img_transfer[0]);
}
static void LIBUSB_CALL cb_2(struct libusb_transfer *transfer)
{
	int i;
	if (transfer->status != LIBUSB_TRANSFER_COMPLETED) {
		fprintf(stderr, "img transfer status %d?\n", transfer->status);
		libusb_free_transfer(transfer);
		img_transfer[1] = NULL;
		return;
	}

	printf("cb_2 callback\n");
	for(i=0;i<1024;i++)
		printf("%c",rcvbuf[1][i]);
	printf("\r\n");
	libusb_submit_transfer(img_transfer[1]);
}
static void LIBUSB_CALL cb_3(struct libusb_transfer *transfer)
{
	int i;
	if (transfer->status != LIBUSB_TRANSFER_COMPLETED) {
		fprintf(stderr, "img transfer status %d?\n", transfer->status);
		libusb_free_transfer(transfer);
		img_transfer[2] = NULL;
		return;
	}

	printf("cb_3 callback\n");
	for(i=0;i<1024;i++)
		printf("%c",rcvbuf[2][i]);
	printf("\r\n");
	libusb_submit_transfer(img_transfer[2]);
}
static void LIBUSB_CALL cb_4(struct libusb_transfer *transfer)
{
	int i;
	if (transfer->status != LIBUSB_TRANSFER_COMPLETED) {
		fprintf(stderr, "img transfer status %d?\n", transfer->status);
		libusb_free_transfer(transfer);
		img_transfer[3] = NULL;
		return;
	}

	printf("cb_4 callback\n");
	for(i=0;i<1024;i++)
		printf("%c",rcvbuf[3][i]);
	printf("\r\n");
	libusb_submit_transfer(img_transfer[3]);
}
static void LIBUSB_CALL cb_5(struct libusb_transfer *transfer)
{
	int i;
	if (transfer->status != LIBUSB_TRANSFER_COMPLETED) {
		fprintf(stderr, "img transfer status %d?\n", transfer->status);
		libusb_free_transfer(transfer);
		img_transfer[4] = NULL;
		return;
	}

	printf("cb_5 callback\n");
	for(i=0;i<1024;i++)
		printf("%c",rcvbuf[4][i]);
	printf("\r\n");
	libusb_submit_transfer(img_transfer[4]);
}
static void LIBUSB_CALL cb_6(struct libusb_transfer *transfer)
{
	int i;
	if (transfer->status != LIBUSB_TRANSFER_COMPLETED) {
		fprintf(stderr, "img transfer status %d?\n", transfer->status);
		libusb_free_transfer(transfer);
		img_transfer[5] = NULL;
		return;
	}

	printf("cb_6 callback\n");
	for(i=0;i<1024;i++)
		printf("%c",rcvbuf[5][i]);
	printf("\r\n");
	libusb_submit_transfer(img_transfer[5]);
}
static void LIBUSB_CALL cb_7(struct libusb_transfer *transfer)
{
	int i;
	if (transfer->status != LIBUSB_TRANSFER_COMPLETED) {
		fprintf(stderr, "img transfer status %d?\n", transfer->status);
		libusb_free_transfer(transfer);
		img_transfer[6] = NULL;
		return;
	}

	printf("cb_7 callback\n");
	for(i=0;i<1024;i++)
		printf("%c",rcvbuf[6][i]);
	printf("\r\n");
	libusb_submit_transfer(img_transfer[6]);
}
static int alloc_transfers(void)
{
	int i=0;
	for(i=0;i<7;i++)
	{
		img_transfer[i] = libusb_alloc_transfer(0);
		if (!img_transfer[i])
			return -ENOMEM;
	}
	libusb_fill_bulk_transfer(img_transfer[0], devh, EP_1, rcvbuf[0],
		sizeof(rcvbuf[0]), cb_1, NULL, 0);
	libusb_fill_bulk_transfer(img_transfer[1], devh, EP_2, rcvbuf[1],
		sizeof(rcvbuf[1]), cb_2, NULL, 0);
	libusb_fill_bulk_transfer(img_transfer[2], devh, EP_3, rcvbuf[2],
		sizeof(rcvbuf[2]), cb_3, NULL, 0);
	libusb_fill_bulk_transfer(img_transfer[3], devh, EP_4, rcvbuf[3],
		sizeof(rcvbuf[3]), cb_4, NULL, 0);
	libusb_fill_bulk_transfer(img_transfer[4], devh, EP_5, rcvbuf[4],
		sizeof(rcvbuf[4]), cb_5, NULL, 0);
	libusb_fill_bulk_transfer(img_transfer[5], devh, EP_6, rcvbuf[5],
		sizeof(rcvbuf[5]), cb_6, NULL, 0);
	libusb_fill_bulk_transfer(img_transfer[6], devh, EP_7, rcvbuf[6],
		sizeof(rcvbuf[6]), cb_7, NULL, 0);
	return 0;
}

static int init_capture(void)
{
	int r,i;
	for(i=0;i<7;i++)
	{
		r = libusb_submit_transfer(img_transfer[i]);
		if (r < 0)
			return r;
	}
	return 0;
}
static void sighandler(int signum)
{
	do_exit = 1;
}
int main(void)
{
	struct sigaction sigact;
	int r = 1;
	int i = 0;

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
		r = libusb_claim_interface(devh, i);
		if (r < 0) {
			fprintf(stderr, "usb_claim_interface error %d\n", r);
			goto out;
		}
	}
	printf("claimed interface\n");
	r = alloc_transfers();
	if (r < 0)
		goto out_deinit;

	r = init_capture();
	if (r < 0)
		goto out_deinit;
	sigact.sa_handler = sighandler;
	sigemptyset(&sigact.sa_mask);
	sigact.sa_flags = 0;
	sigaction(SIGINT, &sigact, NULL);
	sigaction(SIGTERM, &sigact, NULL);
	sigaction(SIGQUIT, &sigact, NULL);
	while (!do_exit) {
		r = libusb_handle_events(NULL);
		if (r < 0)
			goto out_deinit;
	}
	for(i=0;i<7;i++)
	{
		if (img_transfer[i]) {
			r = libusb_cancel_transfer(img_transfer[i]);
			if (r < 0)
				goto out_deinit;
		}
	}
out_deinit:
	for(i=0;i<7;i++)
	libusb_free_transfer(img_transfer[i]);
out_release:
	for(i=0;i<7;i++)
	libusb_release_interface(devh, i);
out:
	libusb_close(devh);
	libusb_exit(NULL);
}
