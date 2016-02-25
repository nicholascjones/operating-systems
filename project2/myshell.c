//myshell.c
//nicholascjones (njones7)
//CSE 30341 Project II: Processes and the Command Line Shell

//library inclusions & system calls requiring them
#include <stdio.h>
#include <unistd.h> //fork
#include <sys/types.h> //wait
#include <sys/wait.h> //wait
#include <stdlib.h> //exit
#include <string.h> //strtok
#include <errno.h> //errno library

//macro definitions
#define LNSIZ 4096 //4096 characters in a line buffer for fgets()



//function declarations
void startF(char * [],int); //starts a program wiht arguments
void runF(char * [],int); //start and wait combined, runs a process to finish and prints exit status
void waitF(); //shell waits until a process exits
void killF(pid_t); //kill takes pid as command
void stopF(pid_t);
void continueF(pid_t);
inline void exitF(); //quit/exit function
	

//main function
//no arguments, run as void

int main() {

	//variable declarations
	char * line; //buffer for fgets, temporarily set to null
	int a; //atoi conversion value
	printf("myshell> "); //prints prompt initially
	fflush(stdout); //forces output for shell prompt purpose

	while (1) { //runs unless program is killed, handled in loop.

		//variable declarations in-program
		char * words[100];
		int nwords = 0;
		int flag = 0; //flag to ensure length isn't too long
		//retrieving user input
		line=calloc(LNSIZ, sizeof(char)); //allocates memory appropriately
		fgets(line,LNSIZ,stdin); //take line with fgets

	//	printf("%d\n",strlen(line));

		if (strlen(line) > LNSIZ) {
			printf("myshell: too many input characters\n");
			continue;
		}

		
		words[0] = strtok(line," \t\n"); //take first word from buffer
		if (words[0] == NULL) continue;
//		printf("%s\n",words[nwords]);
		nwords++; //increment nwords to 1

		while ((words[nwords] = strtok(0," \t\n")) != NULL) {
		//	printf("%s\n",words[nwords]);
			nwords++;
			if (nwords > 100) {
				printf("myshell: too many arguments\n");
				flag = 1; //set break flag
				break;
			}
//			printf("%d\n",nwords);
		}

		if (flag == 1) continue; //continues if too many args

		//input case switch
		if (strcmp(words[0],"start")==0) { //start
		//	printf("%s",words[0]);
			startF(words,nwords);
		} else if (strcmp(words[0],"wait")==0) { //wait
			waitF();		
//			printf("%s",words[0]);
		} else if (strcmp(words[0],"run")==0) { //run
	//		printf("%s",words[0]);
			runF(words,nwords);
		} else if (strcmp(words[0],"kill")==0) { //kill
	//		printf("%s",words[0]);
			a = atoi(words[1]);
			killF(a);
		} else if (strcmp(words[0],"stop")==0) { //stop 
	//		printf("%s",words[0]);
			a = atoi(words[1]);
			stopF(a);
		} else if (strcmp(words[0],"continue")==0) { //continue
	//		printf("%s",words[0]);
			a = atoi(words[1]);
			continueF(a);
		} else if ( ( (strcmp(words[0],"exit")) && (strcmp(words[0],"quit"))) == 0) { //quit case
			exitF();
		} else { //unknown case
			printf("myshell: unknown command: %s\n",words[0]);	
		}

		printf("myshell> "); //prints prompt online
		fflush(stdout); //forces output for shell prompt purpose

	}

	return 0; //end main function if it gets here
}


//function implementations

//kill command
void killF(pid_t pid) {
//	char * pstr; //pid string
//	pstr = atoi((int)pid);
	int val; //value to check for errors	
	val = kill(pid, SIGKILL); //send kill signal
	if (val == 0) { //if no error in kill
		printf("myshell: process %d killed\n",(int)pid);
		return;
	} else { //if there was an error
		printf("myshell: error: process %d could not be killed\n",(int)pid);
		return;
	}
}

void stopF(pid_t pid) {
	int val; //value to check for errors
	val = kill(pid, SIGSTOP); //send stop signal
	if (val == 0) { //if no error
		printf("myshell: process %d stopped\n",(int)pid);
		return;
	} else { //if there was an error
		printf("myshell: error: process %d could not be stopped\n",(int)pid);
		return;
	}
}

void continueF(pid_t pid) {
	int val; //value to check for errors
	val = kill(pid, SIGCONT); //send continue signal
	if (val == 0) { //if no errors
		printf("myshell: process %d continued\n",(int)pid);
		return;
	} else { //if there was an error
		printf("myshell: error: process %d could not be continued\n",(int)pid);
		return;
	}
}

//exit function
inline void exitF(void) {
	printf("\n"); //prints newline for convenience sake
	exit(0);
}


//start function
void startF(char ** words,int nwords) { //starts a program wiht arguments
	pid_t p; //pid to be used
	int execSt; //exec status

	//test to see if aruments are valid
	if (nwords < 2) { //if didn't pass second argument
		printf("myshell: no arguments passed to start\n");
		return;
	}

	p = fork(); //pid = forked value

	if (p < 0) { //if error in fork
		printf("myshell: unable to fork\n");
		return;
	} else if (p == 0) {
	//if no child process
		execSt = execvp(words[1],&words[1]); //execute process
		if (execSt < 0) { //if doesn't execute successfully
			printf("myshell: unable to execute process: %s\n",strerror(errno)); //print error message
			exit(1);
		}
	} else { //starts process with no issue
		printf("myshell: process %d started\n",p);
		return;
	}
}



void runF(char ** words, int nwords) { //start and wait combined, runs a process to finish and prints exit status...almost essentially copy/pasted from both start and then wait

	pid_t p; //pid to be used
	int execSt; //exec status
	//char ** w2 = malloc(nwords*sizeof(*words)); //subarray of arguments to be passed to exec

	//test to see if aruments are valid
	if (nwords < 2) { //if didn't pass second argument
		printf("myshell: no arguments passed to start\n");
		//free(w2);
		return;
	}
	
//	printf("yeah\n");
	p = fork(); //pid = forked value
//	printf("%d\n",p);
	if (p < 0) { //if error
		printf("myshell: unable to fork\n");
	//	free(w2);
//		return;
	} else if (p == 0) {
	//if no child process
		execSt = execvp(words[1],&words[1]); //execute process
		if (execSt < 0) { //if doesn't execute successfully
			printf("myshell: unable to execute process: %s\n",strerror(errno)); //print error message
			exit(1);
		}
	} else {
		//do nothing
	}

	//initialization of run variables
	int s; //status value
	int pwait; //return value of pid given from wait function
	pwait = wait(&s); //wait, return value from current status

	printf("myshell: "); //regardless, prints myshell: as line intro

	//print and determination logic
	if (pwait >= 0) { 	//if the process exited
		
		//determine how process exited
		if (s == 0) { //if exited normally
			printf("process %d exited normally with status 0\n",pwait);
		} else { //if did not exit normally
			printf("process %d exited normally with signal %d: %s\n",pwait,s,strsignal(s));
		}		
		
	} else { //if weren't any processes to wait for
		printf("no processes left\n");
	}		

	return;

}

//wait function
void waitF() { //shell waits until a process exits

	//initialization
	int s; //status value
	int p; //return value of pid given from wait function
	p = wait(&s); //wait, return value from current statu

	printf("myshell: "); //regardless, prints myshell: as line intro

	//print and determination logic
	if (p >= 0) { 	//if the process exited
		//determine how process exited
		if (s == 0) { //if exited normally
			printf("process %d exited normally with status 0\n",p);
		} else { //if did not exit normally
			printf("process %d exited normally with signal %d: %s\n",p,s,strsignal(s));
		}		
		
	} else { //if weren't any processes to wait for
		printf("no processes left\n");
	}		

	return;

}
