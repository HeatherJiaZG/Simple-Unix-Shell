// C Program to design a shell in Linux 
#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<sys/wait.h> 
#include<math.h>
#include<ctype.h>
//#include<readline/readline.h> 
//#include<readline/history.h> 

#define MAXCOM 1000 // max number of letters to be supported 
#define MAXLIST 1000 // max number of commands to be supported 
#define PATHSIZE 5000
// Clearing the shell using escape sequences 
//#define clear() printf("\033[H\033[J") 
char *hist_arr[MAXLIST];
int hist=0;

void add_history(char * str){	

	hist ++;
	hist_arr[hist-1] = (char *) malloc(100);//notice: necessary
	strcpy(hist_arr[hist-1], str);
	//hist_arr[hist-1] = str; //notice: should be strcpy
}

void my_history(char **parsed){
	
	if(parsed[1] == NULL){
		int h=0;
		printf("%s", hist_arr[h]);
		for(h = 1; h < hist; h++){
			printf("%s", hist_arr[h]);
		}
		return;
	}
	int ln = strlen(parsed[1]) - 1;	
	//parsed[1][ln] = '\0';

	int i, num = 0, k;
	for( i = 0; i <= ln; i++){
		if( !isdigit(parsed[1][i]) ){
			//printError();
			break;
		}
		// else{
		// 	num += pow(10, ln - i)*(parsed[1][i] - '0');
		// }
	}

	for(k = i - 1; k >= 0; k--){
		num = num*10 + (parsed[1][k] - '0');
	}
printf("num=%d\n",num);
	if(parsed[2]!=NULL){
		//printError();
	}
	int j = hist - num;
	printf("%s", hist_arr[j]);
	for(j = hist - num + 1; j < hist; j++){
		printf("%s", hist_arr[j]);
	}

}

// Function to take input 
int takeInput(char* str) 
{ 
	char* buffer; 
    size_t bufsize = 200;

    buffer = (char *)malloc(bufsize * sizeof(char));
    if( buffer == NULL)
    {
        perror("Unable to allocate buffer");
        exit(1);
    }

    getline(&buffer,&bufsize,stdin);
	if (strlen(buffer) != 0) { 
		strcpy(str, buffer); 
		add_history(str); 
		return 0; 
	} else { 
		return 1; 
	} 
} 

int findpath(char**parsed, char **path, char thispath[]){
	char path3[500];
    int i=0;
    while(path[i]){
        char path2[500];
		char temp[200];
			strcpy(path2,path[i]);
			strcat(path2, "/"); 
		strcpy(temp, strtok(parsed[0],"\t\n "));
			strcat(path2,temp);

		if(access(path2, X_OK)==0){
				strcpy(path3,path2);
				break;
			}
			i++;
    }
	if(!path[i]) {
        printf("No such command.");
		return 0;
	}

	strcpy(thispath,path3);
	return 1;
}

// Function where the system command is executed 
void execArgs(char **parsed, char thispath[]) 
{ 	
    char*path3=malloc(sizeof(char)*1000);
  	strcpy(path3,thispath); 
 	pid_t pid = fork(); 
	if (pid == 0) { 	
		execv(path3,parsed); 
        printf("Error in execArgs2");//printError();
	} else { 
		// waiting for child to terminate 
		int child=wait(NULL); 
	 
		return; 
	} 
} 

// Function where the piped system commands is executed 
void execArgsPiped(char **parsed, char** parsedpipe, char** path) 
{ 
	printf("\nI'm in execArgsPiped\n");
	// 0 is read end, 1 is write end 
	int pipefd[2]; 
	pid_t p1, p2; 

	if (pipe(pipefd) < 0) { 
		printf("\nPipe could not be initialized"); 
		return; 
	} 
	p1 = fork(); 
	if (p1 < 0) { 
		printf("\nCould not fork"); 
		return; 
	} 

	if (p1 == 0) { 
		// Child 1 executing.. 
		// It only needs to write at the write end 
		close(pipefd[0]); 
		dup2(pipefd[1], STDOUT_FILENO); 
		close(pipefd[1]); 

		if (execvp(parsed[0], parsed) < 0) { 
			printf("\nCould not execute command 1.."); 
			exit(0); 
		} 
	} else { 
		// Parent executing 
		p2 = fork(); 

		if (p2 < 0) { 
			printf("\nCould not fork"); 
			return; 
		} 

		// Child 2 executing.. 
		// It only needs to read at the read end 
		if (p2 == 0) { 
			close(pipefd[1]); 
			dup2(pipefd[0], STDIN_FILENO); 
			close(pipefd[0]); 
			if (execvp(parsedpipe[0], parsedpipe) < 0) { 
				printf("\nCould not execute command 2.."); 
				exit(0); 
			} 
		} else { 
			// parent executing, waiting for two children 
			wait(NULL); 
			wait(NULL); 
		} 
	} 
} 


void my_path(char **parsed, char ** path){
    int i=0;
	if(!parsed[i+1]){
		int j=0;
		for(;j<PATHSIZE;j++){
			path[j]=NULL;
		}		
	}
    while(parsed[i+1]){
        path[i] = parsed[i+1];
		if(path[i][strlen(path[i]) - 1] == '/'){
			path[i][strlen(path[i]) - 1]='\0';
		}
        i++;
    }

}

// Function to execute builtin commands 
int ownCmdHandler(char **parsed,char** path) 
{ 
	int NoOfOwnCmds = 4, i, switchOwnArg = 0; 
	char* ListOfOwnCmds[NoOfOwnCmds]; 
 

	ListOfOwnCmds[0] = "exit"; 
	ListOfOwnCmds[1] = "cd"; 
	ListOfOwnCmds[2] = "history"; 
	ListOfOwnCmds[3] = "path"; 

	for (i = 0; i < NoOfOwnCmds; i++) { 
		if (strcmp(parsed[0], ListOfOwnCmds[i]) == 0) { 
			switchOwnArg = i + 1; 
			break; 
		} 
	} 
    
	switch (switchOwnArg) { 
	case 1: 
		exit(0); 
	case 2: 
		if(parsed[2]!=NULL || chdir(parsed[1])==-1){
			//printError();
		}
		return 1; 
	case 3: 
		my_history(parsed); 
		return 1; 
	case 4: 
        my_path(parsed, path);
		return 1; 
	default: 
		break; 
	}     
	return 0; // not 4 of my calls
} 

// function for finding pipe 
int parsePipe(char* str, char** strpiped) 
{ 
	int i; 
	for (i = 0; i < 2; i++) { 
		strpiped[i] = strsep(&str, "|"); 
		if (strpiped[i] == NULL) 
			break; 
	} 

	if (strpiped[1] == NULL) 
		return 0; // returns zero if no pipe is found. 
	else { 
		return 1; 
	} 
} 

// function for parsing command words 
void parseSpace(char* str, char **parsed) 
{ 

	int i=0;
	parsed[i] = strtok(str," \t\n");

	while (parsed[i] != NULL)
	{
			parsed[++i] = strtok(NULL, " \t\n");
	}
} 

int processString(char* str, char **parsed, char** parsedpipe, char** path) 
{ 

	char* strpiped[2]; 
	int piped  = parsePipe(str, strpiped); 
/*later
	if (piped) { 
		parseSpace(strpiped[0], parsed); 
		parseSpace(strpiped[1], parsedpipe); 

	} else { 
*/
		parseSpace(str, parsed); 
	//later}

	if (ownCmdHandler(parsed, path)) 
		return 0; 
	else
		return 1 + piped; //not 4 of my calls
} 

int main() 
{ 
	char inputString[MAXCOM], *parsedArgs[MAXLIST]; 
	char *parsedArgsPiped[MAXLIST];
	int execFlag = 0; 
	
    char **path=malloc(sizeof(char*)*PATHSIZE); //left: size big enough?
    path[0]="/bin";

	while (1) { 
		char thispath[1000];
		printf("wish> ");
		
		if (takeInput(inputString)) 
			continue; 

		// process 
		execFlag = processString(inputString, 
		parsedArgs, parsedArgsPiped, path); 

		// execflag returns zero if there is no command 
		// or it is a builtin command, 
		// 1 if it is a simple command 
		// 2 if it is including a pipe.
       

		
		strcmp(thispath, thispath);
		// execute 
		if (execFlag == 1) {
			if(findpath(parsedArgs, path, thispath)==0){
				continue;
			}
			execArgs(parsedArgs,thispath); 
		}
		
			
/*
		if (execFlag == 2){
			if(findpath(parsedArgs, path, thispath)==0){
				continue;
			}
			execArgsPiped(parsedArgs, parsedArgsPiped,thispath); 
		}
		
*/     
	} 
	return 0; 
} 



