#include <stdio.h>

int main(int argc, char * argv[]) {
  if (argc == 1) {
    // program was run without any cmd line args
    printf("Run main program\n");
  } else if (argc == 2) {
    // a single cmd line arg was given
    printf("Command is: %s\n", argv[1]);
  } else {
    printf("You gave %i command line arguments\n", argc);
  }
  return 0;
}
