//#include <iostream>
//#include <stdio.h>


//My Libraries
#include "Unit_Disp.h"


using namespace std;

int main()
{
 
   // Allocate memory for read buffer, set size according to your needs
  char read_buf [256];
  ThreadComPort_Init();
  // Normally you wouldn't do this memset() call, but since we will just receive
  // ASCII data for this example, we'll set everything to 0 so we can
  // call printf() easily.
  memset(&read_buf, '\0', sizeof(read_buf));
  int num_bytes=0;

  while(1)
  {
//    usleep(10000);
    ThreadComPort_Update();
  }  
  
  return 0; // success
};

