#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdint.h>
#include "getchildren.h"
#include "readBlock.h"
#include "createheader.h"

int getchildrenhelper(char *filename, int outFD, int v);

void writeFile(char *fullpath, int outFD, int size) {
   int inFile;
   if ((inFile = open(fullpath, O_RDONLY)) == -1) {
      perror("Couldn't open file");
      return;
   }
   readBlocks(inFile, outFD, size);
}

int getchildren(int argc, char *argv[], int outFD, int v) {
  struct stat buff;
  int success;
  char *nulls = calloc(1024, sizeof(uint8_t));
  int err = 0;
  char *filename;  
  if (argc != 4) {
     write(outFD, nulls, 1024);
     free(nulls);
     return 1;
  }
  filename = argv[3];

  success = lstat(filename, &buff);
  if(success != 0) {
    perror(filename);
    write(outFD, nulls, 1024);
    free(nulls);
    return 1;
  }

  if(S_ISDIR(buff.st_mode)) {
    if (createheader(filename, outFD) == 0) {
      success = getchildrenhelper(filename, outFD, v);
      if(success != 0) {
        perror("Error getting file");
        err = 1;
      }
    }
    else {
      perror("");
      err = 1;
    }
  }

  if(S_ISREG(buff.st_mode)) {
 
    if (createheader(filename, outFD) == 0) {
       if (v)
          printf("%s\n", filename);
       writeFile(filename, outFD, (int) buff.st_size);
    } 
    else {
       perror("");
       err = 1;
    }
  }
  
  else if(S_ISLNK(buff.st_mode)) {
     
    if (createheader(filename, outFD) == 0) {
       if (v)
          printf("%s\n", filename);
    }
    else {
       perror("");
       err = 1;
    }
  }
  write(outFD, nulls, 1024);
  free(nulls);
  return err;
}

int getchildrenhelper(char *filename, int outFD, int v) {
  int success;
  DIR *file_dir;
  struct dirent *file_dirent;
  char fullpath[PATH_MAX + 1];
  char *nulls = calloc(1024, sizeof(uint8_t));
  int err = 0;
  if (v)
    printf("%s/\n", filename);

  file_dir = opendir(filename);
  if(file_dir == NULL) {
    free(nulls);
    perror("could not open directory");
    exit(EXIT_FAILURE);
  }  

  /* get all children in directory and print out */
  file_dirent = readdir(file_dir);
  if(file_dirent == NULL) {
    free(nulls);
    perror("could not read first file in directory");
    exit(EXIT_FAILURE);
  } 
  while(file_dirent != 0) {
    if(strcmp(file_dirent->d_name, ".") && strcmp(file_dirent->d_name, "..")) {
      struct stat file_buff;
      /* get stat for file */
      strcpy(fullpath, filename);
      strcat(fullpath, "/");
      strcat(fullpath, file_dirent->d_name);
      success = stat(fullpath, &file_buff);
      if(success == -1) {
        free(nulls);
        perror("stat");
        exit(EXIT_FAILURE);
      }
      /* regular file case */
      if(S_ISREG(file_buff.st_mode)) {
        
        if (createheader(fullpath, outFD) == 0) {
           if (v)
              printf("%s\n", fullpath);
           writeFile(fullpath, outFD, (int) file_buff.st_size);
        }
        else { 
           perror("");
           err = 1;
        }
      }
      /* directory file case */
      else if(S_ISDIR(file_buff.st_mode)) {
        if (createheader(fullpath, outFD) == 0) {
          err = getchildrenhelper(fullpath, outFD, v);
          if(err != 0) {
            return err;
          }
        }
        else {
           perror("");
           err = 1;
        }
      }
      else if(S_ISLNK(file_buff.st_mode)) {
         if (createheader(fullpath, outFD) == 0) {
            if (v)
               printf("%s\n", fullpath);
         }
         else {
            perror("");
            err = 1;
         }
      }
    }
    file_dirent = readdir(file_dir);
  }
  closedir(file_dir);
  free(nulls);
  return err;
}
