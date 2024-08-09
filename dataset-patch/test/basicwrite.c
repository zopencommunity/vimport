#include "dio.h"
#include <stdio.h>

int main(int argc, char* argv[]) {
  int rc;
  struct DFILE* dfile;
  char buffer[] = "Hello world";

  if (argc != 2) {
    fprintf(stderr, "Syntax: argv[0] <dataset>\n");
    return 4;
  }
  dfile = open_dataset(argv[1]);
  if (!dfile) {
    perror("open failed for write");
    return 4;
  }
  dfile->buffer = buffer;
  dfile->bufflen = (sizeof(buffer)-1);

  rc = write_dataset(dfile);
  if (rc) {
    perror("write of dataset failed");
    return 4;
  }

  printf("Wrote %d bytes to dataset %s\n", rc, argv[1]);

  rc = close_dataset(dfile);
  if (rc) {
    perror("close of dataset failed");
    return 4;
  }
  return 0;
}
