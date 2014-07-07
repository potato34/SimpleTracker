#include <stdio.h>
#include <stdlib.h>

#define MODE_FILE_FORMAT "/sys/devices/virtual/misc/gpio/mode/gpio%d"
#define STATUS_FILE_FORMAT  "/sys/devices/virtual/misc/gpio/pin/gpio%d"

int setPinMode(int pin,int mode){
    char path[128];
    sprintf(path,MODE_FILE_FORMAT,pin);
    
    FILE *fd;
    fd = fopen(path,"w");
    
    fwrite(mode,sizeof(int),1,fd);
    fclose(fd);
    
    return  1;
} 

int setPinStatus(int pin,int status){
    char path[128];
    sprintf(path,STATUS_FILE_FORMAT,pin);
    
    FILE *fd;
    fd = fopen(path,"w");
    
    fwrite(status,sizeof(int),1,fd);
    fclose(fd);
    
    return  1;
} 

int main( int argc, const char** argv )
{

    setPinMode(13,1);
    setPinStatus(13,1);

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
//        if(mcount)
//        {
//        fflush(modefile1);
//	    fwrite("1",1,1,modefile1);
//	    fflush(modefile2);
//	    fwrite("1",1,1,modefile2);
//
//	    fflush(pinfile1);
//	    fwrite("0",1,1,pinfile1);
//	    fflush(pinfile2);
//	    fwrite("0",1,1,pinfile2);
//	    mcount=0;
//        }
//        else
//        {
//        fflush(modefile1);
//	    fwrite("1",1,1,modefile1);
//	    fflush(modefile2);
//	    fwrite("1",1,1,modefile2);
//
//	    fflush(pinfile1);
//	    fwrite("1",1,1,pinfile1);
//	    fflush(pinfile2);
//	    fwrite("1",1,1,pinfile2);
//
//	    mcount=1;
//        }
//

    }
    return 0;

}
