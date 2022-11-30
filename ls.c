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

#define valid_options "al"

void print_full_details(char * file_name);

void print_dir(char * path = NULL) {
    if(path == NULL) {
        path = getcwd();
    }
    DIR * folder;
    if((folder = opendir(path)) == NULL) {
        perror("lsh: ");
        exit(EXIT_FAILURE);
    }
    errno = 0;
    struct dirent * file_name;
    while((file_name = readdir(folder)) != NULL && file_name->d_name[0] != '.') {
        printf("%s ", file_name->d_name);
    }
    if(errno != 0) {
        perror("lsh: ");
        exit(EXIT_FAILURE);
    }
}

void print_dir_hidden_files(char * path = NULL) {
    if(path == NULL) {
        path = getcwd();
    }
    DIR * folder;
    if((folder = opendir(path)) == NULL) {
        perror("lsh: ");
        exit(EXIT_FAILURE);
    }
    errno = 0;
    struct dirent * file_name;
    while((file_name = readdir(folder)) != NULL) {
        printf("%s ", file_name->d_name);
    }
    if(errno != 0) {
        perror("lsh: ");
        exit(EXIT_FAILURE);
    }
}

void print_all_details() {
    char * path = getcwd();
    DIR * folder;
    if((folder = opendir(path)) == NULL) {
        perror("lsh: ");
        exit(EXIT_FAILURE);
    }
    errno = 0;
    struct dirent * file_name;
    while((file_name = readdir(folder)) != NULL && file_name->d_name[0] != '.') {
        print_full_details(file_name->d_name);
    }
    if(errno != 0) {
        perror("lsh: ");
        exit(EXIT_FAILURE);
    }
}

void print_all_hidden_details() {
    char * path = getcwd();
    DIR * folder;
    if((folder = opendir(path)) == NULL) {
        perror("lsh: ");
        exit(EXIT_FAILURE);
    }
    errno = 0;
    struct dirent * file_name;
    while((file_name = readdir(folder)) != NULL) {
        print_full_details(file_name->d_name);
    }
    if(errno != 0) {
        perror("lsh: ");
        exit(EXIT_FAILURE);
    }
}


int ls(int argn, char ** args) {
    int c=0, flags=0;
    while((c = getopt(argn, args, valid_options)) != -1) {
        switch(c) {
            case 'a':
                flag += 1;
            case 'l':
                flag += 2;
            case '?':
                perror("lsh: ");
                exit(EXIT_SUCCESS);
            default:
                // print regularly.
        }
    }
    char * path = NULL;
    if(optind < argn) {
        path = args[optind];
    }
    if(flag == 0) {
        print_dir(path);
    }
    else if(flag == 1) {
        print_dir_hidden_files(path);
    }
    else if(flag == 2) {
        print_all_details();
    }
    else {
        print_all_hidden_details();
    }
    return 1;
}

void print_full_details(char * file_name) {
    struct stat fileStat;
    if(stat(file_name, &fileStat) < 0) {    
        perror("lsh: ")
        exit(EXIT_SUCCESS);
    }

    printf("Information for %s\n", argv[1]);
    printf("---------------------------\n");
    printf("File Size: \t\t%d bytes\n", fileStat.st_size);
    printf("Number of Links: \t%d\n", fileStat.st_nlink);
    printf("File inode: \t\t%d\n", fileStat.st_ino);

    printf("File Permissions: \t");
    printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
    printf("\n\n");

    printf("The file %s a symbolic link\n", (S_ISLNK(fileStat.st_mode)) ? "is" : "is not");
}