/**********************************************************************
 *               Maps raw touch events to mouse clicks                *
 *               Author: Jin Kim
 *               Email: jin@ecoation.com
 *
 **********************************************************************/

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <fcntl.h>
#include <limits.h>
#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define RAW_WIDTH 4000.0
#define RAW_HEIGHT 4000.0

#define SCREEN_WIDTH 1024.0
#define SCREEN_HEIGHT 600.0

#define RAW_W_TO_SCREEN_W (SCREEN_WIDTH / RAW_WIDTH)
#define RAW_H_TO_SCREEN_H (SCREEN_HEIGHT / RAW_HEIGHT)

/*Note: If you couldn't build this source code successfully, it may caused by the old kernel issue.
  Please enable below define depending on your kernel version */

//#define KERNEL_2_6_35_DOWNWARD	/* As kernel <= 2.6.35 */
//#define KERNEL_2_6_32_DOWNWARD	/* As kernel <= 2.6.32 */
//#define KERNEL_2_6_29_DOWNWARD	/* As kernel <= 2.6.29 */

typedef struct tagevent
{
	struct timeval time;
	unsigned short type;
	unsigned short code;
	int value;
} input_event;

void SendClick(Display *display, int button, Bool down) 
{
	if(display == NULL)
	{
		fprintf(stderr, "Could not open display 0!!!\n");
		exit(EXIT_FAILURE);
	}
	XTestFakeButtonEvent(display, button, down, CurrentTime);
	XFlush(display);
}

void ParseEvent( input_event ev )
{
	static unsigned short X, Y, State;
	static unsigned short X2, Y2, State2;
	static unsigned char bIsSecond;
	static unsigned char buttonPressed;
	Display *display = XOpenDisplay(NULL);
	Window root = DefaultRootWindow(display);
	if( ev.type == EV_ABS ) {
		switch( ev.code ) {
#ifndef KERNEL_2_6_35_DOWNWARD
		case ABS_MT_SLOT:
			break;
#endif
#ifndef KERNEL_2_6_32_DOWNWARD
		case ABS_MT_PRESSURE: 
			break;
#endif
#ifndef KERNEL_2_6_29_DOWNWARD
		case ABS_MT_TRACKING_ID:
			break;
		case ABS_MT_POSITION_X: 
			break;
		case ABS_MT_POSITION_Y: 
			break;
		case ABS_MT_TOUCH_MAJOR:
			break;
		case ABS_MT_WIDTH_MAJOR:
			break;
#endif
		case ABS_X: 
			X = ev.value * RAW_W_TO_SCREEN_W;
			XWarpPointer(display, None, root, 0, 0, 0, 0, X, Y);
			break;
		case ABS_Y: 
			Y = ev.value * RAW_H_TO_SCREEN_H;
			XWarpPointer(display, None, root, 0, 0, 0, 0, X, Y);
			break;
		case ABS_RX: 
			break;
		case ABS_RY: 
			break;
		case ABS_PRESSURE: 
			break;
		default:
			break;
		}
	}
	else if( ev.type == EV_KEY ){
		switch( ev.code ){
		case BTN_TOUCH:
			SendClick(display, Button1, (ev.value == 1 ? True : False));
			break;
		case BTN_LEFT:
			break;
		case BTN_EXTRA:
			break;
		default:
			break;
		}

	}
	else if( ev.type == EV_SYN ){
		switch( ev.code ){
		case SYN_REPORT:
			break;
#ifndef KERNEL_2_6_29_DOWNWARD
		case SYN_MT_REPORT:
			break;
#endif
		default:
			break;
		}	
	}
	else if( ev.type == EV_MSC ){
	}
	XFlush(display);
	XCloseDisplay(display);
}

int main( int argc, char **argv )
{
	int fd;
	char strPort[64] = { 0 };
	fd_set readfds;

	if( argc == 1 || argc > 2 ) {
		printf(" \nUsage:\n \ttouch_event_clicker [Port]\n");
		printf("Example:\n \ttouch_event_clicker /dev/input/event1\n\n");
		return 0;
	}

	sprintf( strPort, "%s", argv[1] );
	printf("Event Port = %s ", strPort);
	fd = open(strPort, O_RDONLY );
	if( fd >= 0 ) {	
		printf(" open: ok\n" );
		FD_ZERO( &readfds );
		FD_SET( fd , &readfds );
		while( 1 ) {
			if( select( fd+1, &readfds, NULL, NULL, NULL ) > 0 ) {
				input_event ev;
				if( sizeof( input_event ) == read( fd, &ev, sizeof( input_event ) ) ) {
					ParseEvent( ev );
				} else {
					printf(" Nothing read \n" );
				}
			}
		}	
	}
	return 0;
}

