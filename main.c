/*
 * main.c
 *
 *  Created on: Mar 17 2017
 *      Author: david
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <string.h>
#include "dsh.h"

#define MAX_PROC 1024

int main(int argc, char *argv[]) {

	// DO NOT REMOVE THE BLOCK BELOW (FORK BOMB PREVENTION) //
	struct rlimit limit;
	limit.rlim_cur = MAX_PROC;
	limit.rlim_max = MAX_PROC;
	setrlimit(RLIMIT_NPROC, &limit);
	// DO NOT REMOVE THE BLOCK ABOVE THIS LINE //


	char *cmdline = malloc(MAXBUF); // buffer to store user input from commmand line

	//my code here
		
	char cmd[MAXBUF];
	while(strcmp(cmd, "exit\n") != 0){
		//scanf("%[^\n]", cmd);
		char* EOFtest = fgets(cmd, MAXBUF, stdin);
		if(EOFtest == NULL){
			exit(0);
		}

		if(cmd[strlen(cmd)-1]=='\n'){//get rid of new line
      		cmd[strlen(cmd)-1]='\0';
    	}

		//trim potential space at start of cmd
		trimSpace(cmd);
		
		char **terms = split(cmd, " ");
		
		if(cmd[0]=='/'){
			mode1(terms);
		}
		else{	
			mode2(terms, cmd);
		}
		/*
		// print out all the tokens
		int i = 0;
		while (terms[i] != NULL) {
  			printf("%s\n", terms[i]);
  			i++;
		}
		*/
	}
	
	return 0;
}