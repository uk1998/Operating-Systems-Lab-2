#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

void printDir()
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("%s", cwd);
}

void execute(char** tokens)
{
    // Forking a child
    pid_t pid = fork(); 
	
    if (pid == -1)
	{
        printf("Creation of a child process was unsuccessful...\n");
        return;
    }else if (pid == 0){
        if (execvp(tokens[0], tokens) < 0)
            printf("Could not execute command..\n");
        exit(0);
    } else {
        // waiting for child to terminate
        wait(NULL); 
        return;
    }
}

void tokenize(char* line, char** tokens)
{
    int i;
	int j=0;
    for (i = 0; i < MAX_INPUT_SIZE; i++)
	{
        tokens[i] = strsep(&line, " ");
        if (tokens[i] == NULL)
            break;
        if (strlen(tokens[i]) == 0)
            i--;
    }
}

int main(int argc, char* argv[]) {
	char line[MAX_INPUT_SIZE];
	char *tokens[MAX_NUM_TOKENS];
	char *username = getenv("USER");
	
	FILE* fp;
	if(argc == 2) {
		fp = fopen(argv[1],"r");
		if(fp < 0) {
			printf("File doesn't exists.");
			return -1;
		}
	}

	while(1) {			
		/* BEGIN: TAKING INPUT */
		bzero(line, sizeof(line));
		if(argc == 2)
		{ 	// batch mode
			if(fgets(line, sizeof(line), fp) == NULL)// file reading finished
				break;
			line[strlen(line) - 1] = '\0';
		} 
		else
		{ // interactive mode
			printf("%s:", username);
			printDir();
			printf("$ ");
			scanf("%[^\n]", line);
			getchar();
		}
		/* END: TAKING INPUT */
		tokenize(line, tokens);//separates command strings
		if(!strcmp("cd", tokens[0]))//handle cd here
			chdir(tokens[1]);
		else if(tokens[0]!=NULL)
			execute(tokens);
		else
			printf("Could not execute command..\n");
	}
	return 0;
}