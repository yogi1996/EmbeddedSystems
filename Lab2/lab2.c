/*
 *
 * CSEE 4840 Lab 2 for 2019
 *
 * Name/UNI: Jino Haro (jmh2289), Tvisha Gangwani (trg2128)
 */
#include "fbputchar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "usbkeyboard.h"
#include <pthread.h>

/* Update SERVER_HOST to be the IP address of
 * the chat server you are connecting to
 */
/* micro36.ee.columbia.edu */
#define SERVER_HOST "129.236.238.242"
#define SERVER_PORT 42000

#define BUFFER_SIZE 128
int netRow=1;
int netCol=0;
/*
 * References:
 *
 * http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html
 * http://www.thegeekstuff.com/2011/12/c-socket-programming/
 * 
 */

int sockfd; /* Socket file descriptor */

struct libusb_device_handle *keyboard;
uint8_t endpoint_address;

pthread_t network_thread;
void *network_thread_f(void *);

int main()
{
  int err, col,row,row1,col1,sendRes,m;
  m=0;
  row1 = 21;
  col1 = 1;
  char stringSend [128];
  struct sockaddr_in serv_addr;

  struct usb_keyboard_packet packet;
  int transferred;
  char keystate[12];

  if ((err = fbopen()) != 0) {
    fprintf(stderr, "Error: Could not open framebuffer: %d\n", err);
    exit(1);
  }
	for (col = 0 ; col < 64 ; col++) {
	for (row =0; row <24;row++)
    
    fbputchar(' ', row, col);
  }
  /* Draw rows of asterisks across the top and bottom of the screen 
  for (col = 0 ; col < 64 ; col++) {
    fbputchar('*', 0, col);
    fbputchar('*', 23, col);
  }

	for (col = 0 ; col < 64 ; col++) {
    //fbputchar('*', 0, col);
    fbputchar('*', 12, col);
  }
  fbputs("Hello CSEE 4840 World!", 4, 10);*/

  /* Open the keyboard */
  if ( (keyboard = openkeyboard(&endpoint_address)) == NULL ) {
    fprintf(stderr, "Did not find a keyboard\n");
    exit(1);
  }
    
  /* Create a TCP communications socket */
  if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
    fprintf(stderr, "Error: Could not create socket\n");
    exit(1);
  }

  /* Get the server address */
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(SERVER_PORT);
  if ( inet_pton(AF_INET, SERVER_HOST, &serv_addr.sin_addr) <= 0) {
    fprintf(stderr, "Error: Could not convert host IP \"%s\"\n", SERVER_HOST);
    exit(1);
  }

  /* Connect the socket to the server */
  if ( connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    fprintf(stderr, "Error: connect() failed.  Is the server running?\n");
    exit(1);
  }

  /* Start the network thread */
  pthread_create(&network_thread, NULL, network_thread_f, NULL);

  /* Look for and handle keypresses */
  for (;;) {
    libusb_interrupt_transfer(keyboard, endpoint_address,
			      (unsigned char *) &packet, sizeof(packet),
			      &transferred, 0);
    if (transferred == sizeof(packet)) {
      sprintf(keystate, "%02x %02x %02x", packet.modifiers, packet.keycode[0],
	      packet.keycode[1]);
      printf("%s\n", keystate);
      fbputs(keystate, 6, 0);
      
      
      
      if (packet.keycode[0] == 0x2c) { /* space pressed? */
		

		for (col = 0 ; col < 64 ; col++) {
    	//fbputchar('*', 0, col);
    		fbputchar('*', 20, col);
  		}
  		
  		fbputs("Hello CSEE 4840 World!", 0, 0);

      }
      
      if (packet.keycode[0]) { /* NUMBERS */
      	if ((packet.keycode[0] > 0x1d )& (packet.keycode[0] < 0x28) )
      	
      	{
      		
      		
      		char c  = packet.keycode[0] +19;
      		if (packet.keycode[0] == 0x27){
      			c = '0';
      		}
      		
      		stringSend[m] = packet.keycode[0];
      		m = m +1;
      		
			fbputchar(c,row1, col1);
			col1 = col1 +1 ;
			if (col1 > 63){
			row1 = row1 +1 ;
			col1 = 1;
      	}
      	}
      	
      	//  ALPH
      	else{
      		// UPPER 
      					firstkey = packet.keycode[0];
if ( packet.modifiers & USB_LSHIFT || packet.modifiers & USB_RSHIFT ) {
/*Capital letters*/
if (firstkey >= 4 && firstkey <= 29) {
					firstkey += 61;
				}
else{
// DO WE ALSO HAVE TO DO KEYPAD?
switch(firstkey) {
case 30: firstkey = '!';  
break;
case 31: firstkey = '@';
break;
case 32: firstkey = '#';
break;
case 33: firstkey = '$';
break;
case 34: firstkey = '%';
break;
case 35: firstkey = '^';
break;
case 36: firstkey = '&';
break;
case 37: firstkey = '*';
break;
case 38: firstkey = '(';
break;
case 39: firstkey = ')';
break;
case 44: firstkey = ' ';
break;
case 45: firstkey = '_';
break;
case 46: firstkey = '+';
break;
case 47: firstkey = '{';
break;
case 48: firstkey = '}';
break;
case 49: firstkey = '|';
break;
case 51: firstkey = ':';
break;
case 52: firstkey = '"';
break;
case 53: firstkey = '~';
break;
case 54: firstkey = '<';
break;
case 55: firstkey = '>';
break;
case 56: firstkey = '?';
break;
default: is_ascii = 0;
break;
					}
				}
			}
else {
      	
      		//LOWER 
      		char c  = packet.keycode[0] + 93;
      		
			fbputchar(c,row1, col1);
			col1 = col1 +1 ;
			if (col1 > 63){
			row1 = row1 +1 ;
			col1 = 1;
			}
			}
			}
			
      }
      
      //MOVING THE CURSOR RIGHT
      fbputchar('_',row1,col1);
      if (packet.keycode[0] == 0x4f){
      	fbputchar(' ',row1, col1);
      	col1 = col1 +1 ;
      	
      	}
      	if (packet.keycode[0] == 0x50){
      	fbputchar(' ',row1, col1);
      	col1 = col1 - 2 ;
      	
      	}
      	// THIS IS NOW BACKSPACE KEY
      if (packet.keycode[0] == 0x2a){
      	fbputchar(' ',row1, col1);
      	col1 = col1 - 2;
      	
      	}
      
      /*	THESE WERE UP-DOWN KEYS 
      if (packet.keycode[0] == 0x52){
      	fbputchar(' ',row1, col1);
      	row1 = row1 -1 ;
      	
      	}
      if (packet.keycode[0] == 0x51){
      	fbputchar(' ',row1, col1);
      	row1 = row1 +1 ;
      	
      	}*/
      	
      	
      	// IF ENTER 
      	if (packet.keycode[0] == 0x28){
      	fbputchar('S',row1, col1);
      	
      	//stringSend[m] = '\0';
      	printf("%s",stringSend);
      	
      	sendRes = send(sockfd,"trg2128", strlen("trg2128"),0);
      	m =0;
      	if (sendRes < 0){
      	printf("Error");
		}      	
		else{
		printf("sent");
		}
      	
      	}
      	
      if (packet.keycode[0] == 0x29) { /* ESC pressed? */
	break;
      }
    }
  }

  /* Terminate the network thread */
  pthread_cancel(network_thread);

  /* Wait for the network thread to finish */
  pthread_join(network_thread, NULL);

  return 0;
}

void *network_thread_f(void *ignored)
{
  char recvBuf[BUFFER_SIZE];
  int n;
  /* Receive data */
  
  while ( (n = read(sockfd, &recvBuf, BUFFER_SIZE - 1)) > 0 ) {
    recvBuf[n] = '\0';
    printf("%s", recvBuf);
    fbputs(recvBuf, netRow, 0);
    netRow = netRow +1 ;
    
    
    
  }
  

  return NULL;
}

