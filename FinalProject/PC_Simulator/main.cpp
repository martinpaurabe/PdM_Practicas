//#include <iostream>
//#include <stdio.h>


//My Libraries
//#include "ThreadComPort.h"
#include "Unit_ComPort.h"


using namespace std;

int main()
{
  OpenCommPort(115200);
  // Write to serial port
 // unsigned char msg[] = { 'H', 'e', 'l', 'l', 'o', '\r' };
  

  // Allocate memory for read buffer, set size according to your needs
  char read_buf [256];

  // Normally you wouldn't do this memset() call, but since we will just receive
  // ASCII data for this example, we'll set everything to 0 so we can
  // call printf() easily.
  memset(&read_buf, '\0', sizeof(read_buf));
  int num_bytes=0;

  while(1)
  {
   // Read bytes. The behaviour of read() (e.g. does it block?,
    // how long does it block for?) depends on the configuration
    // settings above, specifically VMIN and VTIME
    num_bytes = BytesDisponibles(); 
  
       // n is the number of bytes read. n may be 0 if no bytes were received, and can also be -1 to signal an error.
    if (num_bytes < 0) {
      printf("Error reading: %s", strerror(errno));
      return 1;
    }

    // Here we assume we received ASCII data, but you might be sending raw bytes (in that case, don't try and
    // print it to the screen like this!)
    
    if(20< num_bytes)
    {
      ReadBytes(read_buf,9);
      printf("%s\r\n", read_buf);
    }
 
  //cout << "Hello world!" << endl;
  }  
  
  return 0; // success
};

