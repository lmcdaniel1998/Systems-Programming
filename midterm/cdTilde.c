#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
  struct stat *current_stat;
  DIR *current_dir;
  struct dirent *current_dirent;
  struct stat *parent_stat;
  DIR *parent_dir;
  struct dirent *parent_dirent;
  int success;
  char *users_name;

  if(argc >= 2) {
    users_name = argv[1];
  }

  if(argv[1][0] != "~") {
    return -1;
  }
  else {
   current_dir = opendir(".");
   current_dirent = readdir(current_dir);
   success = lstat(current_dirent->d_name, current_stat);
   
   parent_dirent = readdir(current_dir);
   success = lstat(current_dirent->d_name, parent_stat);

   while(current_stat->st_dev != parent_stat->st_dev && current_stat->st_ino != parent_stat->st_ino) {
     parent_dir = opendir("..");
     success = closedir(current_dir);
     current_dir = parent_dir;
     current_dirent = readdir(current_dir);
     success = lstat(current_dirent->d_name, current_stat);
     parent_dirent = readdir(current_dir);
     success = lstat(parent_dirent->d_name, parent_stat);
  }
  
  while(!strcmp(current_dirent->d_name, (users_name + 1))) {
    current_dirent = readdir(current_dir);
    if(current_dirent == NULL) {
      return -1;
    }
  }

  parent_dir = opendir(current_dirent->d_name);
  success = closedir(current_dir);

  return 0;
  
}
