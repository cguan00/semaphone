#include "control.h"


// A control program
//
//     Take a command line argument to determine if it is creating or removing the resources or viewing the story.
//     If creating (command line argument: -c):
//         Should make the shared memory, semaphore and file (open the file with the truncate flag).
//         Set any values that are needed.
//
//     If removing (command line argument: -r):
//         Remove the shared memory, the semaphore and the story.
//         Display the full contents of the story.
//         This should wait until the semaphore is available.
//
//     If viewing (command line argument: -v):
//         Output the contents of the story file.
//         This mode does not need to interact with the semaphore




char flag[5];
int semd, shmd, fd;
union semun su;
struct sembuf sb;

// If creating (command line argument: -c):
  // Should make the shared memory, semaphore and file (open the file with the truncate flag).
  // Set any values that are needed.
int creating() {
  semd = semget(SEMKEY, 1, IPC_CREAT | 0644);
  if (semd < 0) {
    printf("Error: %s\n", strerror(errno));
    return -1;
  }
  printf("semaphore created\n");
  semctl(semd, 0, SETVAL, su);

  shmd = shmget(SHKEY, sizeof(char *), IPC_CREAT | 0644);
  if (shmd < 0) {
    printf("Error: %s\n", strerror(errno));
    return -1;
  }
  printf("shared memory created\n");

  fd = open("semaphone.txt", O_CREAT | O_TRUNC | O_RDWR, 0644);
  if (fd < 0) {
    printf("Error: %s\n", strerror(errno));
    return -1;
  }
  printf("file created\n");
  close(fd);

  return 0;
}


// If removing (command line argument: -r):
//     Remove the shared memory, the semaphore and the story.
//     Display the full contents of the story.
//     This should wait until the semaphore is available.
int removing() {
  semd = semget(SEMKEY, 1, 0);
  if (semd < 0) {
    printf("Error: %s\n", strerror(errno));
    return -1;
  }
  printf("trying to get in\n");
  semop(semd, &sb, 1);

  shmd = shmget(SHKEY, sizeof(char *), 0);
  if (shmd < 0) {
    printf("Error: %s\n", strerror(errno));
    return -1;
  }

  fd = open("semaphone.txt", O_RDONLY);
  if (fd < 0) {
    printf("Error: %s\n", strerror(errno));
    return -1;
  }
  char buff[SIZE];
  buff[0] = '\0';
  read(fd, buff, SIZE);
  if (strlen(buff) != 0) {
    *(strrchr(buff, '\n') + 1) = '\0';
  }
  printf("The story so far:\n");
  printf("%s\n", buff);
  close(fd);

  shmctl(shmd, IPC_RMID, 0);
  printf("shared memory removed\n");

  remove("semaphone.txt");
  printf("file removed\n");

  semctl(semd, IPC_RMID, 0);
  printf("semaphore removed\n");


  return 0;
}


// If viewing (command line argument: -v):
//     Output the contents of the story file.
//     This mode does not need to interact with the semaphore
int viewing() {
  fd = open("semaphone.txt", O_RDONLY);
  if (fd < 0) {
    printf("Error: %s\n", strerror(errno));
    return -1;
  }
  char buff[SIZE];
  buff[0] = '\0';
  read(fd, buff, SIZE);
  if (strlen(buff) != 0) {
    *(strrchr(buff, '\n') + 1) = '\0';
  }
  printf("The story so far:\n");
  printf("%s\n", buff);
  close(fd);

  return 0;
}

int main(int argc, char *argv[]) {
  su.val = 1;
  sb.sem_num = 0;
  sb.sem_op = -1;
  strcpy(flag, argv[1]);
  runProgram();
  return 0;
}


int runProgram() {
  if (strcmp(flag, "-c") == 0) {
    creating();
  } else if (strcmp(flag, "-v") == 0) {
    viewing();
  } else if (strcmp(flag, "-r") == 0) {
    removing();
  } else {
    printf("Invalid flag\n");
  }
  return 0;
}
