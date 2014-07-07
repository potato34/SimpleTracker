#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

#define MODE_FILE_FORMAT "/sys/devices/virtual/misc/gpio/mode/gpio%d"
#define PIN_FILE_FORMAT  "/sys/devices/virtual/misc/gpio/pin/gpio%d"


int main( int argc, const char** argv )
{
    char buffer[128];
    FILE *modefile1, *pinfile1,*modefile2, *pinfile2;
    int pin1,pin2;
    cout<<"test2"<<endl;
	//********************gpio init*********************
	pin1 = 2;
	pin2 = 3;
//********GPIO 1******
	sprintf(buffer,MODE_FILE_FORMAT,pin1);
    modefile1 = fopen(buffer,"w");
    cout<<"test1"<<endl;
    if(modefile1 == NULL)
    {
        fprintf(stderr,"Unable to open mode file: %s\n",buffer);
        exit(2);
    }

    sprintf(buffer,PIN_FILE_FORMAT,pin1);
    pinfile1 = fopen(buffer,"w");
    if(pinfile1 == NULL)
    {
        fprintf(stderr,"Unable to open pin file: %s\n",buffer);
        exit(2);
    }
//********GPIO 2******
    sprintf(buffer,MODE_FILE_FORMAT,pin2);
    modefile2 = fopen(buffer,"w");
    if(modefile2 == NULL)
    {
        fprintf(stderr,"Unable to open mode file: %s\n",buffer);
        exit(2);
    }

    sprintf(buffer,PIN_FILE_FORMAT,pin2);
    pinfile2 = fopen(buffer,"w");
    if(pinfile2 == NULL)
    {
        fprintf(stderr,"Unable to open pin file: %s\n",buffer);
        exit(2);
    }

    fflush(modefile1);
    fwrite("1",1,1,modefile1);
    fflush(modefile2);
    fwrite("1",1,1,modefile2);

    fflush(pinfile1);
    fwrite("1",1,1,pinfile1);
    fflush(pinfile2);
    fwrite("0",1,1,pinfile2);
    /*
    fclose(modefile1);
    fclose(modefile2);
    fclose(pinfile1);
    fclose(pinfile2);
*/
    int mcount=0;

    while(1)
    {
        /*
        sprintf(buffer,MODE_FILE_FORMAT,pin1);
	    modefile1 = fopen(buffer,"w");
	    cout<<"test1"<<endl;
	    if(modefile1 == NULL)
	    {
	        fprintf(stderr,"Unable to open mode file: %s\n",buffer);
	        exit(2);
	    }

	    sprintf(buffer,PIN_FILE_FORMAT,pin1);
	    pinfile1 = fopen(buffer,"w");
	    if(pinfile1 == NULL)
	    {
	        fprintf(stderr,"Unable to open pin file: %s\n",buffer);
	        exit(2);
	    }
	//********GPIO 2******
	    sprintf(buffer,MODE_FILE_FORMAT,pin2);
	    modefile2 = fopen(buffer,"w");
	    if(modefile2 == NULL)
	    {
	        fprintf(stderr,"Unable to open mode file: %s\n",buffer);
	        exit(2);
	    }

	    sprintf(buffer,PIN_FILE_FORMAT,pin2);
	    pinfile2 = fopen(buffer,"w");
	    if(pinfile2 == NULL)
	    {
	        fprintf(stderr,"Unable to open pin file: %s\n",buffer);
	        exit(2);
	    }
        */

        //sleep(1);
        if(mcount)
        {
        fflush(modefile1);
	    fwrite("1",1,1,modefile1);
	    fflush(modefile2);
	    fwrite("1",1,1,modefile2);

	    fflush(pinfile1);
	    fwrite("0",1,1,pinfile1);
	    fflush(pinfile2);
	    fwrite("0",1,1,pinfile2);
	    mcount=0;
        }
        else
        {
        fflush(modefile1);
	    fwrite("1",1,1,modefile1);
	    fflush(modefile2);
	    fwrite("1",1,1,modefile2);

	    fflush(pinfile1);
	    fwrite("1",1,1,pinfile1);
	    fflush(pinfile2);
	    fwrite("1",1,1,pinfile2);

	    mcount=1;
        }


    }
        fclose(modefile1);
	    fclose(modefile2);
	    fclose(pinfile1);
        fclose(pinfile2);
    return 0;

}
