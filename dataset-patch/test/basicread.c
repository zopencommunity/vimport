#include "dio.h"
#include <stdio.h>

int main(int argc, char* argv[]) {
  int rc;
  struct DFILE* dfile;

  if (argc != 2) {
    fprintf(stderr, "Syntax: argv[0] <dataset>\n");
    return 4;
  }
  dfile = open_dataset(argv[1]);
  if (!dfile) {
    perror("open failed for read");
    return 4;
  }

  rc = read_dataset(dfile);
  if (rc) {
    perror("read of dataset failed");
    return 4;
  }

  printf("Read %d bytes from dataset %s\n", rc, argv[1]);

  rc = close_dataset(dfile);
  if (rc) {
    perror("close of dataset failed");
    return 4;
  }
  return 0;
}
