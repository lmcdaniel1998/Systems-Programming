#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <utime.h>
#include <math.h>
#include "archiveListing.h"
#include "binaryHeaders.h"

#define BLOCKSIZE 512

#define NAMEOFF 0
#define MODEOFF 100
#define UIDOFF 108
#define GIDOFF 116
#define SIZEOFF 124
#define MTIMEOFF 136
#define CHKSUMOFF 148
#define TYPEFLAGOFF 156
#define LINKNAMEOFF 157
#define MAGICOFF 257
#define VERSIONOFF 263
#define UNAMEOFF 265
#define GNAMEOFF 297
#define PREFIXOFF 345

int createDir(char *path);

void buildPath(char *header, char name[257]) {
   int prefixLen = 0;
   name[0] = '\0';
   if ((prefixLen = strlen(&header[PREFIXOFF])) != 0) {
   
      name[0] = '\0';
      strcpy(name, &header[PREFIXOFF]);
      strcat(name, "/");
   }
   strcat(name, header);
}

void passFile(int tarFD) {
   char header[BLOCKSIZE];
   int size;
   int numBlocks = 0;
   if (read(tarFD, header, BLOCKSIZE) == -1) {
      perror("Can't read tar file");
      exit(EXIT_FAILURE);
   }
   size = octTOint(&header[SIZEOFF]);
   if (size == 0)
      return;
   else {
      numBlocks = 1 + (size / BLOCKSIZE);
      while (numBlocks) {
         read(tarFD, header, BLOCKSIZE);
         numBlocks--;
      }
   }
}

int checkStrict(char *header) {
   if (&header[MAGICOFF + 5] != '\0')
      return 1;
   if ((header[VERSIONOFF] != '0') || (header[VERSIONOFF + 1] != '0'))
      return 1;
   return 0;
}

int extractFile(int tarFD, int v, int s) {

   char header[BLOCKSIZE];
   char body[BLOCKSIZE];
   int size;
   int numBlocks = 0;
   char type;
   char *pointlessPointer;
   long int permissions;
   int newFD, extra;
   char name[257];
   char name2[257];
   struct stat st = {0};
   time_t newMTime;
   struct utimbuf new_times;
   uint32_t gid;
   char *symName;

   if (read(tarFD, header, BLOCKSIZE) == -1) {
      perror("Can't read tar file");
      exit(EXIT_FAILURE);
   }
   buildPath(header, name);
   strcpy(name2, name);
   size = octTOint(&header[SIZEOFF]);
   type = header[TYPEFLAGOFF];
   permissions = strtol(&header[MODEOFF], &pointlessPointer, 8);
   umask(~permissions);
   
   /* strict flag  */
   if (s) {
      if (checkStrict(header)) {
         if (size) {
            numBlocks = 1 + (size / BLOCKSIZE);
            while (numBlocks) {
               read(tarFD, body, BLOCKSIZE);
            }
         }
         return 1;
      }
   }

   if (v)
      printf("%s\n", name);
   
   createDir(name2);
   
   if (type == '5') {
      
      if (stat(name, &st) == -1)
         mkdir(name, 0777);
      
   }
   /* Get file body if regular */
   else if (type == '0' || type == '\0') {
      
      numBlocks = 1 + (size / BLOCKSIZE);
      extra = (size % BLOCKSIZE);
      umask(~permissions);
      if ((newFD = open(name, O_WRONLY | O_CREAT, permissions)) != -1) {
         
         while (numBlocks) {

            read(tarFD, body, BLOCKSIZE);
            
            if (numBlocks == 1)
               write(newFD, body, extra);
            else
               write(newFD, body, BLOCKSIZE);

            numBlocks --;
         }
      } 
      else {
         fprintf(stderr, "%s :", name);
         perror("");
         while (numBlocks) {
            read(tarFD, body, BLOCKSIZE);
            numBlocks --;
         }
         return 1;
      }
   }
   
   if (type == '2') {
   
      symName = calloc(101, sizeof(uint8_t));
      memcpy(symName, &header[LINKNAMEOFF], 100);
      umask(~permissions);
      if ((newFD = open(name, O_WRONLY | O_CREAT, permissions)) != -1) {
      
         if (symlinkat(name, newFD, symName) == -1) {
            perror(name);
            free(symName);
            return 1;
         }
         else {
            free(symName);
         }
      }
      else {
         perror(name);
         free(symName);
         return 1;
      }

   }
   
   /* restore group ID */
   gid = extract_special_int(&header[GIDOFF], 8);
   if(chown(name, getuid(), gid) == -1) {
     perror("");
     return 1;
   }

   /* restore modified time */
   newMTime = (time_t) octTOint(&header[MTIMEOFF]);
   lstat(name, &st);
   
   new_times.actime = st.st_atime;
   new_times.modtime = newMTime;

   if (utime(name, &new_times) < 0) {
      perror(name);
      return 1;
   }
   return 0;
}

int createDir(char *path) {
   
   char *tempDir;
   char curDir[257];
   struct stat st = {0};

   getcwd(curDir, 257);
   
   tempDir = strtok(path, "/");
   if (tempDir) {
      
      if (stat(tempDir, &st) == -1) {
         umask(~0700);
         mkdir(tempDir, 0777);
      }
      if (chdir(tempDir) == -1)
         return 1;
      tempDir = strtok(NULL, "/");
   }
   chdir(curDir);
   return 0;
}

int extractMain(int argc, char *argv[], int v, int s) {  
   int tarFD; 
   int numPaths = argc - 3;
   char temp[BLOCKSIZE];
   char name[257];
   int i, pass;
   int len;
   int err = 0;
   if ((tarFD = open(argv[2], O_RDONLY)) == -1) {
      perror("Invalid Tar file");
      exit(EXIT_FAILURE);
   }
   
   if (numPaths == 0) {  
      while (checkIfEnd(tarFD)) {
         err = extractFile(tarFD, v, s);
      }
      return err;
   }
   else {

      /* If user specifies files to extract */
      while (checkIfEnd(tarFD)) {
         /* if pass = 1, user has extracted file and the FD
          * is at the start of next header */
         pass = 0;
         read(tarFD, temp, BLOCKSIZE);
         buildPath(temp, name);
         if (lseek(tarFD, -512, SEEK_CUR) == -1) {
            perror("Lseek failure");
            exit(EXIT_FAILURE);
         }
 
         for (i = 3; i < argc; i++) {
            len = strlen(argv[i]); 
            if ((strncmp(argv[i], name, len) == 0) && name[len] == '/') {
               if (createDir(name) == 0) {
                  err = extractFile(tarFD, v, s);
                  pass = 1;
               }
               break;
            }
         }
         if (pass == 0)
            passFile(tarFD);
      }
      return err;
   }
}
