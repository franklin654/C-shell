#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>
#include <errno.h>


#define LSH_RL_BUFFSIZE 1024
#define LSH_TOK_DELIMITER " \t\r\n\a\""
#define LSH_TOK_BUFFSIZE 64

int lsh_cd(char ** args);
int lsh_pwd(char ** args);
int lsh_echo(char ** args);
int lsh_exit(char ** args);

char *  shell_builtins[] = {"cd", "echo", "pwd", "exit"};

int (*shell_func[])  (char **) = {&lsh_cd, &lsh_echo, &lsh_pwd, &lsh_exit};

int lsh_builtins_size() {
    return sizeof(shell_builtins)/sizeof(char*);
}



char *lsh_read_line(void) {
    char *line = (char*)malloc(LSH_RL_BUFFSIZE*sizeof(char));
    size_t len = 0;
    ssize_t c;
    c = getline(&line, &len, stdin);
    if (c == -1) {
        if (feof(stdin)) {
            exit(EXIT_SUCCESS);
        }
        else {
            perror("readline");
            exit(EXIT_FAILURE);
        }
    }

    return line;
}

char **lsh_parser(char *line) {
    int buffsize = LSH_TOK_BUFFSIZE, position = 0;
    char ** arguments = (char**)malloc(LSH_TOK_BUFFSIZE*sizeof(char*));
    char * token;
    token = strtok(line, LSH_TOK_DELIMITER);
    while(token != NULL) {
        arguments[position] = token;
        position++;
        if (position >= buffsize) {
            buffsize += LSH_TOK_BUFFSIZE;
            arguments = (char**)realloc(arguments, buffsize*sizeof(char*));
            if(!arguments) {
                fprintf(stderr, "lsh allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, LSH_TOK_DELIMITER);
    }
    arguments[position] = NULL;
    
    return arguments;
}

int lsh_launch(char ** args) {
    pid_t pid, wait_pid;
    int status;
    pid = fork();
    if(pid == 0) {
       if(execvp(args[0], args) == -1){
           if(errno == 2) {
            printf("lsh: Invalid command please check your input\n");
           }
           else  {
            perror("lsh");
           }
       }
       exit(EXIT_FAILURE);
    }
    else if(pid < 0) {
        perror("lsh");
    }
    else {
        do{
        wait_pid = waitpid(pid, &status, WUNTRACED);
        }while(!WIFEXITED(status)&&!WIFSIGNALED(status));
    }

    return 1;
}

int lsh_execute(char ** args) {
    int i;
    if (args[0] == NULL) {
        printf("No command is entered\n");
        return 1;
    }
    for(i = 0;i<lsh_builtins_size();i++) {
        if(strcmp(shell_builtins[i], args[0])==0) {
            return (*shell_func[i])(args);
        }
    }

    return lsh_launch(args);
}

void lsh_loop() {
    char * command;
    char ** arguments;
    int status;
    do {
        printf("> ");
        command = lsh_read_line();
        arguments = lsh_parser(command);
        status = lsh_execute(arguments);
        free(command);
        free(arguments);
    }while(status);
}

main() {
    lsh_loop();

    return EXIT_SUCCESS;
}

lsh_cd(char **args) {
    if(args[1] == NULL) {
        if(chdir(getenv("HOME")) != 0) {
            perror("lsh");
        } 
    }
    else {
        if(chdir(args[1]) != 0) {
            perror("lsh");
        }
    }

    return 1;
}

lsh_echo(char **args) {
    int i=0;
    while(args[i+1] != NULL) {
        printf("%s ", args[i+1]);
        i++;
    }
    printf("\n");
    return 1;
}

lsh_pwd(char ** args) {
    char * buffer  = (char *)malloc(PATH_MAX*sizeof(char));
    if(getcwd(buffer, PATH_MAX) == NULL) {
        perror("lsh");
    }
    else {
        printf("%s\n", buffer);
    }
    return 1;
}

lsh_exit(char ** args) {
    return 0;
}