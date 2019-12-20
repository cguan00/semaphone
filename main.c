#include <stdio.h>
#include <stdlib.h>

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

int main(){
  return 0;
}
