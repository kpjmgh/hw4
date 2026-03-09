/*
 * dsh.c
 *
 *  Created on: Aug 2, 2013
 *      Author: chiu
 */
#include "dsh.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <errno.h>
#include <err.h>
#include <sys/stat.h>
#include <string.h>


// TODO: Your function definitions below (declarations in dsh.h)


char** split(char *str, char *delim){
    int capacity = MAXBUF;
    int numDelim = 0;
    //count the amount of instances of delimeters in str and call in instCount
    for(int i = 0; i<strlen(str); i++){
      if(delim[0] == str[i]){
        numDelim++;
      }
    }
    int numTokens=numDelim+1;
    char** tokens = createStringArray(numTokens,capacity); 

    char copy[strlen(str)+1];
    strcpy(copy, str);

    strcpy(tokens[0], strtok(copy, delim));
    for(int i = 1; i<numTokens; i++) {
        strcpy(tokens[i], strtok(NULL, delim));
    }
    tokens[numTokens] = NULL;// does null need to be replaced with '\0'?
    return tokens;
}
char** createStringArray(int num, int capacity){ //taken from assignment info
    
    // this creates num pointers to strings
    char **array = malloc(num * sizeof(char*));

    // this loops through each array element and instantiates
    // an array of chars of length CAPACITY
    for (int i = 0; i < num; i++) {
      array[i] = malloc(capacity * sizeof(char));
    }

    // now I can assign strings to individual array elements
    for (int i = 0; i < num; i++) {
      strcpy(array[i], "hello world"); //obviously replace hello world later
    }

    return array;
}

int mode1(char** terms){
  //does path to file exist?
  if(access(terms[0], F_OK | X_OK) == 0){//yes
    forkAndExec(terms);
    return 0;
  }
  else{
    printf("File does not exist or is not executable.");//command not found error
    return 1;
  }
}

int mode2(char **terms, char cmd[]){
  //is command "build-in"? (Is first token pwd, cd, )
  if(strcmp(terms[0], "pwd")==0){
    //run appropriate internal function (pwd)
    char currentDir[MAXBUF];
    printf("%s", getcwd(currentDir, MAXBUF));
    return 0;//exit mode2
  }
  else if(strcmp(terms[0], "cd")==0){
    int successCheck;
    if(sizeof(terms)/sizeof(terms[0]) ==1){//path not given
      successCheck = chdir(getenv("HOME"));
    }
    else{
      char path= cdPath(cmd);//isolates path from command.
      successCheck = chdir(path);//I'm a bit confused about how chdir works with relative vs absolute paths
    }

    if(successCheck){//failed
      printf("Command not found.");
      return 1;
    }
    return 0;
  }
  else{
    char currentDir[MAXBUF];
    getcwd(currentDir, MAXBUF);
    if(makeAndExecPath(currentDir, terms)){
      return;
    }
    char pathEnv[strlen(getenv("PATH"))+1];
    strcpy(pathEnv,getenv("PATH"));
    //use strtok to split pathEnv by ":"
    char* tokens = strtok(pathEnv, ":");
    while(tokens != NULL){
      tokens = strtok(NULL, ":");
    }
    //for each token, call makeAndExecPath
    //if it returns true, return. Otherwise, try next token
    for(int i = 0; i<strlen(tokens);i++){//would you use strlen here?
      int found = makeAndExecPath(cmd,tokens[i]); //I don't feel confident with this line.
      printf("%s/%s",cmd,tokens[i]);//for testing
      if(found){
        return 0;
      }
    }

    printf("Command not found.");
    return 1;
    /*
    else{
      split the PATH environment var by ":"
      more paths?
      int exitLoop = 0
      while(0(more paths (yes))){
        concatenate next path with command
        does path to file exist?
        if(yes){
          forkAndExec(cmd);
          exitLoop =1;
        }
        more paths?
        if(yes){
          continue while loop
        }
        else{
          exitLoop = 1;
          command not found error
          exit mode2
        }
      }
    }
    */
  }
  
}

void forkAndExec(char** terms){
  char* cmd = terms[0];
  pid_t id = fork();
  if(id == 0){//child
    if(cmd[strlen(cmd)-1]=='&'){
      cmd[strlen(cmd)-1]= '\0';
    }
    execv(cmd,terms);
  }
  else{//parent
    if(cmd[strlen(cmd)-1]=='&'){//if command ends with "&"
      //run in background
      return;
    }
    else{
      //run in forground
      wait(NULL);//what argument do I pass?
      return;
    }
  }
}

void trimSpace(char cmd[]){
  while(cmd[0]==' '){
			for(int i=0; i<strlen(cmd)-1; i++){
				cmd[i]=cmd[i+1];
			}
      cmd[strlen(cmd)-1]='\0';
		}

    while(cmd[strlen(cmd)-1]==' '){
      cmd[strlen(cmd)-1]='\0';
    }
}

int makeAndExecPath(char path[], char** terms){
  char* fullPath[strlen(path)+1+strlen(terms[0])+1];
  sprintf(fullPath, "%s/%s", path, terms[0]);
  if(access(fullPath, F_OK | X_OK) == 0){//yes
    terms[0] = fullPath;
    forkAndExec(terms);
    return 0;
  }
  else{
    return 1;
  }
}

char* cdPath(char cmd[]){
  char* path = cmd;
  for(int i =0; i<2; i++){
			for(int i=0; i<strlen(path)-1; i++){
				path[i]=path[i+1];
			}
      path[strlen(path)-1]='\0';
		}
  trimSpace(path); 
  return path;
}