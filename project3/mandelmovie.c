//nicholascjones
//cse30341
//mandelmovie.c



#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// macro definitions of variables and strings
#define MANDEL "./mandel"
#define X -1.3689
#define Y .005
#define M 1000
#define BMP ".bmp"
#define MAN "mandel"
//i is predeclared to be zero globally

//i = 0;

//takes command line argument of number of processes
int main( int argc, char *argv[] ) {

	//variable declarations

	int maxProcesses;

	double s = 2.000; //minimum value 
	double b = 0.00001; //maximum value
	double a = s; //temporarily set to start
	int m = 1000; //max number of iterations
	;

	//setting number of processes
	if (argc == 1) {
		exit(1); //exits if no argument
	} else {
		maxProcesses = atoi(argv[1]);

	}

	
	pid_t pid;

	int currentProcesses = 0; //number of processes currently

	int currentImage = 0; //current image being processed


	while (currentImage < 50) { //while less than 50 images

		if (currentProcesses < maxProcesses) { //if not max processes running
			pid = fork(); //fork process
			if (pid < 0) { //if error
				exit(1);
			} else if (pid == 0) { //looking at child if forked
				//string declarations and allocations
				static char fn[256];
				static char sstr[256];
				static char mstr[256];
				sprintf(fn, "mandel%d.bmp", currentImage);
				sprintf(sstr, "%f", s);
				sprintf(mstr, "%d", m);
				//turns into valid command with args array
				static char *input[] = {MANDEL,"-x","-1.3689","-y",".005","-s",sstr,"-o",fn,"-m",mstr,NULL};
				//execute mandel function
				execvp(input[0],input);
			} else { //looking at parent
				currentProcesses++; //increment current image and number of processes
				currentImage++;
				s *= exp(log(b/a)/49); //mathematically zoom in further
			
			}
		} else { //if there are maxProcesses currently running, then wait for one to end
			int status;
			pid_t wp;
			wp = wait(&status);
			currentProcesses--; //decrements number of processes currently running
		}
			
	}

	return 0; //returns and prints
}
