#ifndef STAGE_H
#define STAGE_H

typedef struct Stage {
   char command[512];
   int stageNum;
   char *input;
   char *output;
   int argc;
   char **argv;
   struct Stage *next;
} Stage;

Stage *createStages(char *arg[256], int numArgs,
   int numStages, char command[512]);

#endif
