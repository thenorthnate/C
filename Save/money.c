#include <stdio.h>
#include "money.h"

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











// fuctions

int get_int(void) {
  char line[100];
  int answer, sscanf_result, check;

  check = 0;

  while (check == 0){
    fgets(line, sizeof(line), stdin);
    sscanf_result = sscanf(line, "%d", &answer);

    if ((sscanf_result == 0) | (sscanf_result == EOF)){
      // Integer was not entered
      printf("You must enter a number: ");
    } else {
      check = 1;
    }
  }
  return answer;
}
