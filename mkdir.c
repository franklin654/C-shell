#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <limits.h>
#include <errno.h>
#include <dirent.h>

#define mkdir_options "vm:"

int create_dir(int argn, char ** args) {
    int c=0, flags=0;
    mode_t permission;
    while((c = getopt(argn, args, valid_options)) != -1) {
        switch(c) {
            case 'v':
                flag += 1;
            case 'm':
                flag += 2;
                permission = atoi(optarg); 

            case '?':
                perror("lsh: ");
                exit(EXIT_SUCCESS);
            default:
                // print regularly.
        }
    }

    if(optind >= argn) {
        printf("Missing argument after options\n");
        exit(EXIT_SUCCESS);
    }
    char * path = args[optind];
    switch(flags) {
        default:
            if( (mkdir(path)) == -1) {
                perror("lsh: ");
                exit(EXIT_FAILURE);
            };
        case 1:
            if(mkdir(path) == 0) {
                printf("Successfully created dir");
            }
            else {
                perror("lsh: ");
                exit(EXIT_FAILURE);
            }
        case 2:
            if( (mkdir(path), permission) == -1) {
                perror("lsh: ");
                exit(EXIT_FAILURE);
            }
        case 3:
            if( (mkdir(path), permission) == 0) {
                printf("Successfully created dir");
            }
            else {
                perror("lsh: ");
                exit(EXIT_FAILURE);
            }
    }
    return 1;

}