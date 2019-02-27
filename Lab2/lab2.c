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
#define SERVER_HOST "160.39.128.67"
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

  char shiftKey;
  int err, col,row,row1,col1,sendRes,m,n;
  m=0;
  n=0;
  row1 = 21;
  col1 = 1;
  char stringSend [128] = {' '};
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
  printf("m:%d,n:%d,col1:%d,row1:%d",m,n,col1,row1);
  
  printf("Here 113");
  	if (m>=n){
  	n = m;
  	}
    libusb_interrupt_transfer(keyboard, endpoint_address,
			      (unsigned char *) &packet, sizeof(packet),
			      &transferred, 0);
    if (transferred == sizeof(packet)) {
      sprintf(keystate, "%02x %02x %02x", packet.modifiers, packet.keycode[0],
	      packet.keycode[1]);
      printf("%s\n", keystate);
      fbputs(keystate, 6, 0);
      
      for (col = 0 ; col < 64 ; col++) {
    	//fbputchar('*', 0, col);
    		fbputchar('*', 20, col);
  		}
  		fbputs("Hello CSEE 4840 World!", 0, 0);
      
      //if (packet.keycode[0] == 0x2c) { /* space pressed? */
		

		
  		
  		
  		
  		
  		
  		//fbputchar(' ', row1, col1);

      //}
      shiftKey = packet.keycode[0];
      if ((packet.keycode[0])&& (packet.keycode[0] != 0x50) &&(packet.keycode[0] != 0x4f) ) { /* NUMBERS */
      //printf("WHY VTHE FUCK???") ;
      
      	if ((packet.keycode[0] > 0x1d )& (packet.keycode[0] < 0x39) & ( packet.keycode[0] != 0x2C) & ( packet.keycode[0] != 0x2A)&( packet.keycode[0] != 0x28))
      	
      	{
      	
      		printf("WE ARE HERE");
      		
      		if ((packet.keycode[0]>0x2c) & ~( packet.modifiers & USB_LSHIFT || packet.modifiers & USB_RSHIFT )){
      		
      		switch(shiftKey) {
						
						case 44: shiftKey = ' ';
										 break;
						case 45: shiftKey = '-';
										 break;
						case 46: shiftKey = '=';
										 break;
						case 47: shiftKey = '[';
										 break;
						case 48: shiftKey = ']';
										 break;
						case 49: shiftKey = '\\';
										 break;
						case 51: shiftKey = ';';
										 break;
						case 52: shiftKey = '\'';
										 break;
						case 53: shiftKey = '`';
										 break;
						case 54: shiftKey = ',';
										 break;
						case 55: shiftKey = '.';
										 break;
						case 56: shiftKey = '/';
										 break;
						case 0: break;
						default: shiftKey = '0';
										 break;
										 }
			char c  = shiftKey;
      		
      		stringSend[m] = c;
      		printf("HERE:%s",stringSend);
      		m = m +1;
      		
			fbputchar(c,row1, col1);
			col1 = col1 +1 ;
			if (col1 > 63){
			row1 = row1 +1 ;
			col1 = 1;
			}
			continue;
			}
      		
      		if ( packet.modifiers & USB_LSHIFT || packet.modifiers & USB_RSHIFT ) {

				printf("SHIFT IS PRESSED %x", packet.keycode[0]);
				
				

// some other char
switch(shiftKey) {
case 30: shiftKey = '!';  
break;
case 31: shiftKey = '@';
break;
case 32: shiftKey = '#';
break;
case 33: shiftKey = '$';
break;
case 34: shiftKey = '%';
break;
case 35: shiftKey = '^';
break;
case 36: shiftKey = '&';
break;
case 37: shiftKey = '*';
break;
case 38: shiftKey = '(';
break;
case 39: shiftKey = ')';
break;
case 44: shiftKey = ' ';
break;
case 45: shiftKey = '_';
break;
case 46: shiftKey = '+';
break;
case 47: shiftKey = '{';
break;
case 48: shiftKey = '}';
break;
case 49: shiftKey = '|';
break;
case 51: shiftKey = ':';
break;
case 52: shiftKey = '"';
break;
case 53: shiftKey = '~';
break;
case 54: shiftKey = '<';
break;
case 55: shiftKey = '>';
break;
case 56: shiftKey = '?';
break;
default: shiftKey= '0';
break;

				}
char c  = shiftKey;
      		
      		stringSend[m] = c;
      		printf("HERE:%s",stringSend);
      		m = m +1;
      		
			fbputchar(c,row1, col1);
			col1 = col1 +1 ;
			if (col1 > 63){
			row1 = row1 +1 ;
			col1 = 1;

				
				}
				}
	else{
			if ((packet.keycode[0] <0x2a) | (packet.keycode[0] >0x2d) ){
			/*
			fbputchar(' ',row1, col1);
			col1 = col1 +1 ;
			if (col1 > 63){
			row1 = row1 +1 ;
			col1 = 1;
			}*/
			
			char c  = packet.keycode[0] +19;
      			if (packet.keycode[0] == 0x27){
      				c = '0';
      			}
      		
      			stringSend[m] = c;
      			m = m +1;
      			
				fbputchar(c,row1, col1);
				col1 = col1 +1 ;
				if (col1 > 63){
				row1 = row1 +1 ;
				col1 = 1;
				}
			
			}
      		/*else{
      		
      			char c  = packet.keycode[0] +19;
      			if (packet.keycode[0] == 0x27){
      				c = '0';
      			}
      		
      			stringSend[m] = packet.keycode[0];
      			m = m +1;
      			
				fbputchar(c,row1, col1);
				col1 = col1 +1 ;
				f (col1 > 63){
				row1 = row1 +1 ;
				col1 = 1;
				}
				}*/
			
      	}
      	}
      	
      	//  ALPH
      	else{
      		// UPPER 
      					//shiftKey = packet.keycode[0];
			if ( packet.modifiers & USB_LSHIFT || packet.modifiers & USB_RSHIFT ) {

					printf("SHIFT IS PRESSED %x", packet.keycode[0]);
					/*Capital letters*/
					if (shiftKey >= 4 && shiftKey <= 29) {
						shiftKey += 61;
						}

					//printf("HERE");

					char c = shiftKey;
					stringSend[m] = c;
      				//printf("HERE:%s",stringSend);
      				m = m +1;
					fbputchar(c,row1, col1);
					col1 = col1 +1 ;
					if (col1 > 63){
						row1 = row1 +1 ;
						col1 = 1;
					}
			}
			//int m =n;
else {
      	
      		//LOWER 
      		char c  = packet.keycode[0] + 93;
      		stringSend[m] = c;
      		printf("HERE:%s",stringSend);
      		m = m +1;
      		
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
      	//fbputchar(' ',row1, col1);
      	//int  z = n;
      	//stringSend[z] = '\0';
      	//fbputs(stringSend,21,1);
      	//stringSend[z] = ' ';
      	//m = m+1;
      	
      	//col1 = col1 +1 ;
      	
      	//TRYING WITH 1 char
      	if ((col1 >n )|(col1==n)){
      	
      	//printf("WEN AR$E FINALLY EHERE");
      	int  z = n;
      	stringSend[z] = '\0';
      	
      	printf("stringSend before: %s%m", stringSend,m);
      	//stringSend[z] = ' ';
      	stringSend[n] = "0";
      	fbputchar(" ",row1,col1);
      	 z = n+1;
      	stringSend[z] = '\0';
      	m=m+1;
      	n=n+1;
      	col1 = col1+1;
      	printf("stringSend after: %s%m", stringSend,m);
      	//fbputchar(stringSend[col1-1],row1,col1);
      	
      	}
      	fbputchar(stringSend[col1-1],row1,col1);
      	
      	
      	
      	}
      	//LEFT STILL DOESNT WORK 
      	if (packet.keycode[0] == 0x50){
      	/*fbputchar(' ',row1, col1);
      	col1 = col1 - 2 ;
      	//int z1 =n;
      	//stringSend[z1] = '\0';
      	fbputs(stringSend,21,1);
      	//stringSend[z1] = ' ';
      	m=m-1;*/
      	//TRYING WITH 1 char
      	
      	fbputchar(stringSend[col1-1],row1,col1);
      	//printf("stringSend after: %s%m", stringSend,m);
      	m=m-1;
      	col1=col1-1;
      	if(col1<1){
      	col1 =1;
      	m=0;
      	
      	
      	}
      	
      	}
      	
      	// THIS IS NOW BACKSPACE KEY
      if (packet.keycode[0] == 0x2a){
      	fbputchar(' ',row1, col1);
      	//printf("Before %d/n",m);
      	
      	m = m-2;
      	
      	//printf("After %d/n",m);
      	col1 = col1 - 2;
      	if(col1 == 0){
      		row1 = row1 -1;
      		col1 = 62;
      	}
      	
      	if ((row1 < 21)){
      		row1=21;
      		col1 = 1;
      	}
      	
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
      	printf("This is m: %d",m);
      	//m=m-1;
      	//n =n-1;
      	stringSend[n] = '\0';
      	printf("%s",stringSend);
      	
      	sendRes = send(sockfd,stringSend, n,0);
      	
      	for (int i = 0; i<n;i++){
      	stringSend[i] = ' ';
      	}
      	//stringSend [128] = {' '};
      	m = 0;
      	n = 0;
      	
      	if (sendRes < 0){
      	printf("Error");
		}      	
		else{
			printf("sent");
			
			
			for (row1 = 21;row1<23;row1++){
				for (col1 = 0 ; col1 < 64 ; col1++) {
    			
    				//fbputchar('*', 0, col);
    				fbputchar(' ', row1, col1);
    				
    			}
  			}
  			
  			row1 = 21;
  			col1 = 1;
		
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
  char temp[65];
  int n;
  int j =0;
  /* Receive data */
  
  while ( (n = read(sockfd, &recvBuf, BUFFER_SIZE - 1)) > 0 ) {
    recvBuf[n] = '\0';
    printf("HERE IS THE BUFFER %s", recvBuf);
    char temp[65];
    if (n > 60){
    	
    	
    	for (int i=0; i<n ;i++){
    		printf("%d",i);
    		temp[j] = recvBuf[i];
    		j++;
    		
    		if (j> 60){
    		
    			temp[j] = '\0';
    			fbputs(temp, netRow, 0);
    			netRow = netRow +1;
    			netCol = 0;
    			j =0;
    		}
    	}
    	temp[j] = '\0';
    	fbputs(temp, netRow, 0);
    	j=0;
    	netRow = netRow +1;
    	netCol = 0;
    }
    else{
    fbputs(recvBuf, netRow, 0);
    netRow = netRow +1 ;
    netCol = 0;
    }
    
    if (netRow == 21){
    //Clear the table 
    for (netCol = 0 ; netCol < 64 ; netCol++) {
    	for (netRow = 0;netRow<21;netRow++){
    	//fbputchar('*', 0, col);
    		fbputchar(' ', netRow, netCol);
    		}
  		}
  		
    netRow = 1;
    netCol = 0;
    
    fbputs(recvBuf, netRow, 0);
    netRow = 2;
    }
    
    
  }
  

  return NULL;
}