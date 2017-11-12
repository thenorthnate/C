// Included libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function declarations
int get_int();
int write_to_file(char fileName[], char data[], int offset, int whence);
void read_from_file(char fileName[], char *buffer, int bufferSize, int offset, int whence);

// main
int main(void) {
  int value;
  int success;
  char fn[] = "nate.n";
  char contents[] = {0x22, 0x10, 0xF3, 0xFF, 0x34, 0xFF, 0x34, 0xE1, 0x9E};

  int bufS = 10;

  char *cont;
  cont = (char *) malloc(sizeof(char) * bufS);
  for (int i = 0; i < sizeof(contents)/sizeof(contents[0]); i++){
    cont[i] = contents[i];
  }


  char *buf;
  buf = (char *) malloc(sizeof(char) * bufS);


  printf("Please enter a number: ");
  value = get_int();
  printf("You entered %d\n", value);

  printf("Writing to a file\n");
  success = write_to_file(fn, cont, 0, 0);
  printf("%i\n", success);

  read_from_file(fn, buf, bufS, 0, 0);
  printf("%s\n", buf);
  free(buf);
  free(cont);
}


// Functions
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

int write_to_file(char fileName[], char * data, int offset, int whence) {
  FILE *fp;
  fp = fopen(fileName, "wb");

  switch (whence) {
    case 0:
      // Start at the beginning of the file
      fseek(fp, offset, SEEK_SET);
      break;
    case 1:
      // Start from the current position in the file
      fseek(fp, offset, SEEK_CUR);
      break;
    case 2:
      // Start from the end of the file (offset should be negative)
      fseek(fp, offset, SEEK_END);
      break;
  }

  fputs(data, fp);
  fclose(fp);

  return 0;
}

void read_from_file(char fileName[], char *buffer, int bufferSize, int offset, int whence) {
  /* Opens and reads the file... no point in "case 1" unless the file was already
  opened before running this function. Maybe a different function can
  process an already opened file pointer? */

  //NOTE: Need to free the buffer!!!

  FILE *fp;

  fp = fopen(fileName, "rb");
  switch (whence) {
    case 0:
      fseek(fp, offset, SEEK_SET);
      break;
    case 1:
      fseek(fp, offset, SEEK_CUR);
      break;
    case 2:
      fseek(fp, offset, SEEK_END);
      break;
  }

  fgets(buffer, bufferSize, fp);
  fclose(fp);
}
