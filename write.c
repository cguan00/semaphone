#include "control.h"

int shmd, semd, fd;
struct sembuf sb;

// A main writing program
//
//     Will attempt to access the resources using the semaphore
//         Once in, it should display the last line added to the file (use shared memory to contain the size of the last line).
//         Prompt the use for the next line.
//         Once a new line is read, write that to the file, update the shared memory and then release the semaphore

// The main program should work as follows:
//
//     -Print a message that it will check if resources are available.
//     -When they are, display the last line added to the story (or none, if it is new).
//     -Prompt the user to enter the next line
//     -Update the story
//     -quit

int runProgram_write() {
  printf("Trying to get in\n");
  semd = semget(SEMKEY, 1, 0);
  if (semd < 0) {
    printf("Error: %s\n", strerror(errno));
    return -1;
  }
  semop(semd, &sb, 1);
  shmd = shmget(SHKEY, sizeof(char *), 0);
  if (shmd < 0) {
    printf("Error: %s\n", strerror(errno));
    return -1;
  }
  fd = open("semaphone.txt", O_WRONLY | O_APPEND);
  char *last = shmat(shmd, 0, 0);
  printf("Last addition: %s\n\n", last);
  char next[SIZE];
  printf("Your addition: ");
  fgets(next, SIZE, stdin);
  printf("\n");

  write(fd, next, strlen(next));
  close(fd);
  strcpy(last, next);
  shmdt(last);
  sb.sem_op = 1;
  semop(semd, &sb, 1);
  return 0;
}



int main() {
  sb.sem_num = 0;
  sb.sem_op = -1;
  runProgram_write();
  return 0;
}
