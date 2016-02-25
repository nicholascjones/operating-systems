//copyit.c
//nicholascjones
//cse30341
//Project I: System Calls and Error Handling

//included libraries with associated system calls as comments
#include <stdio.h> //general inclusion
#include <sys/types.h> //open
#include <sys/stat.h> //open
#include <fcntl.h> //open
#include <unistd.h> //read
#include <string.h> //strerror
#include <errno.h> //errno
#include <stdlib.h> //exit
#include <signal.h> //signal

#define BUFSIZE 4096 //definition of default buffer size as such

//function declarations
inline void errExit(); //exit on error, inline as to not call stack
void success(int,char *, char *); //successful case, prints #bytes and file
void display_message(int); //prints waiting phrase when not successful
void noArguments(); //returns error if no arguments
void tooManyArguments(); //returns error if too many arguments
inline void properUse(); //prints "helpful" comment when not used correctly, inline to avoid too much stack call

//main function
int main(int argc, char *argv[])
{	
	//variable declarations
	int openv; //open value of return
	int creatv; //creat value of return
	int c1v, c2v; //close values 1 and 2
	int numBytes = 0; //number of bytes moved
	ssize_t readv; //value of read call
	ssize_t writev; //validity value of write call
	char * fn; //filename
	char * dest; //path destination
	char buffer[BUFSIZE]; //buffer for reading and writing, size defined above

	//setting and establishment of alarm for signaling
	signal(SIGALRM,display_message); //call signal from display function
	alarm(1); //set an alarm for one second

	//argument counter error checking
	switch(argc) { //switch case directs values
		case(0):
		case(1):
		case(2):
			 noArguments(); //if fewer than 2 arguments, fail
			 break;
		case(3): //if exactly 2 arguments, run successfully
			 break;
		default: //if more than 2 arguments, fail
			 tooManyArguments();
			 break;
	}
			 
	//assignment of arguments to variables
	fn = argv[1]; //filename = first argument
	dest = argv[2]; //dest = second argument
	
	//opening of file
	openv = open(fn,O_RDONLY); //opens filename, in readonly mode
	if (openv < 0) { //checks if file is valid
		printf("copyit: Unable to open %s: %s\n",fn,strerror(errno));
		errExit(); //error exit
	}
	//creation of new file
	creatv = creat(dest, S_IRWXU);
	if (creatv < 0) { //checks if can be done, reports and prints error else
		printf("copyit: Unable to create %s: %s\n",dest,strerror(errno));
		errExit();
	}
	
	//read/write loop
	readv = 1; //set as generic starting value
	while (readv != 0) { //while able to be read
		readv = read(openv, buffer, BUFSIZE);
		if (readv < 0) { //checks if read is valid
			if (errno == EINTR) {
				continue;
			} else {
				printf("copyit: Unable to read from %s: %s\n",fn,strerror(errno));
				errExit();
			}
		}
		writev = write(creatv,buffer,readv); //write and assigning check value
		if (writev < 0) {
			if (errno == EINTR) {
				continue;
			} else {
				printf("copyit: Unable to write to %s: %s\n",dest,strerror(errno));
				errExit();
			}
		}
		numBytes += readv;
	}

	c2v = close(creatv);
	if (c2v < 0) { //check to see if close is invalid
		printf("copyit: Unable to close %s: %s\n",dest,strerror(errno));
		errExit();
	}

	c1v = close(openv);
	if (c1v < 0) { //check to see if close is invalid
		printf("copyit: Unable to close %s: %s\n",fn,strerror(errno));
		errExit();
	}

	success(numBytes,fn,dest);
	
	return 0; //return with no error
}

//FUNCTIONS

inline void errExit() { //inline function to do exit
	exit(1);
}

inline void properUse() { //printed funtion message for proper use
	printf("usage: copyit <sourcefile> <targetfile>\n");
}

void noArguments() { //error function given when not enough arguments
	printf("copyit: Not enough arguments!\n");
	properUse(); //calls helpers for completion
	errExit();
}

void tooManyArguments() { //error when too many arguments
	printf("copyit: Too many arguments!\n");
	properUse(); //uses helper functions to give other messages adn exit
	errExit();
}
	
//prints message if still copying longer than 1s
void display_message(int s) { //used for signaling
	printf("copyit: still copying...\n"); //printed message
	alarm(1); //sets new alarm for one second
}
	
void success(int bc,char * f1, char * f2) { //successful case, prints #bytes and file
	printf("copyit: Copied %d bytes from %s to %s.\n",bc,f1,f2);
}
	
