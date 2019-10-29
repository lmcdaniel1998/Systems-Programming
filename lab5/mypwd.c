#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>

struct FileEquality {
  mode_t permissions;
  ino_t serial_number;
  dev_t file_system;
};

int file_equal(struct FileEquality dir_1, struct FileEquality dir_2);

int main() {

  struct stat current_info, parent_info;
  struct FileEquality current, parent;
  DIR *parent_dir, *current_dir, *parent_dir_holder;
  struct dirent *parent_dir_info, *current_dir_info;
  char *full_path[PATH_MAX];
  int path_len = 0;
  int func_success;

  /* open directory program resides in */
  current_dir = opendir(".");
  /* cant read the directory */
  if(current_dir == NULL) {
    perror("cannot get current directory");
    exit(EXIT_FAILURE);
  }
  /* read first file (current dir) in directory */
  current_dir_info = readdir(current_dir);
  /* check if readdir succeeded */
  if(current_dir_info == NULL) {
    perror("could not read current directory");
    exit(EXIT_FAILURE);
  }
  /* get file information of current directory */
  if(stat(".", &current_info)) {
    perror("could not get file info");
    exit(EXIT_FAILURE);
  }
  current.permissions = current_info.st_mode;
  current.serial_number = current_info.st_ino;
  current.file_system = current_info.st_dev;
  
  /* open directory above where program resides in */
  parent_dir = opendir("..");
  /*cant read the directory */
  if(parent_dir == NULL) {
    perror("cannot get parent directory");
    exit(EXIT_FAILURE);
  }
  /* read second dir (parent dir) in directory */
  parent_dir_info = readdir(current_dir);
  if(parent_dir_info == NULL) {
    perror("could not read parent directory");
    exit(EXIT_FAILURE);
  }
  /* get file information of parent directory */
  if(stat("..", &parent_info)) {
    perror("could not get parent file info");
    exit(EXIT_FAILURE);
  }
  parent.permissions = parent_info.st_mode;
  parent.serial_number = parent_info.st_ino;
  parent.file_system = parent_info.st_dev;

  /* while currents parent is not current */
  while(file_equal(current, parent) != 0) {
    /* close current directory */
    func_success = closedir(current_dir);
    if(func_success == -1) {
      perror("directory not closed");
      exit(EXIT_FAILURE);
    }
    /* move up into parent directory */
    func_success = chdir("..");
    if(func_success == -1) {
      perror("could not change directory to parent");
      exit(EXIT_FAILURE);
    }

    parent_dir_holder = parent_dir;

    /* compare file attributes of all files in parent to childs attributes to find name */
    while(file_equal(current, parent) != 0) {
      /* read next file in directory */
      parent_dir_info = readdir(parent_dir_holder);
      if(parent_dir_info == NULL) {
        perror("couldn't find current dir in parent");
        exit(EXIT_FAILURE);
      }
      /* get file info of next file in directory */
      if(stat(parent_dir_info->d_name, &parent_info)) {
        perror("could not get next files info");
        exit(EXIT_FAILURE);
      }
      parent.permissions = parent_info.st_mode;
      parent.serial_number = parent_info.st_ino;
      parent.file_system = parent_info.st_dev;
    }
    /* reset read dir for parent directory */
    rewinddir(parent_dir);

    /* once loop is complete add previous directory name to path */
    char *dir_name;
    int b;
    b = 0;
    while((parent_dir_info->d_name)[b] != '\0') {
      b++;
    }
    dir_name = malloc(sizeof(char) * (b + 1));
    if(!dir_name) {
      perror("malloc failed");
      exit(EXIT_FAILURE);
    }
    int y;
    for(y = 0; (parent_dir_info->d_name)[y] != '\0'; ++y) {
      dir_name[y] = (parent_dir_info->d_name)[y];
    }
    dir_name[y] = '\0';
    full_path[path_len] = dir_name;
    path_len++;

    /* set curirent directory to parent */
    current_dir = parent_dir_holder;
    current_dir_info = readdir(current_dir);
    if(current_dir_info == NULL) {
      perror("couldn not find current directory");
      exit(EXIT_FAILURE);
    }
    if(stat(current_dir_info->d_name, &current_info)) {
      perror("could not get current stats");
      exit(EXIT_FAILURE);
    }
    current.permissions = current_info.st_mode;
    current.serial_number = current_info.st_ino;
    current.file_system = current_info.st_dev;

    /* set parent directory to current's parent */
    parent_dir_info = readdir(current_dir);
    if(parent_dir_info == NULL) {
      perror("could not get parent directory info");
      exit(EXIT_FAILURE);
    }
    parent_dir = opendir(parent_dir_info->d_name);
    if(parent_dir == NULL) {
      perror("could not open new parent directory");
      exit(EXIT_FAILURE);
    }
    if(stat(parent_dir_info->d_name, &parent_info)) {
      perror("could not get parent stats");
      exit(EXIT_FAILURE);
    }
    parent.permissions = parent_info.st_mode;
    parent.serial_number = parent_info.st_ino;
    parent.file_system = parent_info.st_dev;
  }

  int h;
  for(h = path_len - 1; h >= 0; h--) {
      printf("/%s", full_path[h]);
  }
  printf("\n");
  return 0;
}

/* function that return 0 if files are the same or 1 if they are different */
int file_equal(struct FileEquality dir_1, struct FileEquality dir_2) {
  
  /* check if both have same permissions */
  if(dir_1.permissions != dir_2.permissions) {
    return 1;
  }
  /* check if both have same file system */
  if(dir_1.file_system != dir_2.file_system) {
    return 1;
  }
  /* check if both have same serial number */
  if(dir_1.serial_number != dir_2.serial_number) {
    return 1;
  }
  return 0;
}
