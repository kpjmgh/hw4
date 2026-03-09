#define MAXBUF 256  // max number of characteres allowed on command line

// TODO: Any global variables go below

// TODO: Function declarations go below
char** createStringArray(int num, int capacity); //taken from assignment info
char** split(char *str, char *delim);
int mode1(char** terms);
int mode2(char **terms, char cmd[]);
void forkAndExec(char** terms);
void trimSpace(char cmd[]);
char* cdPath(char cmd[]);
