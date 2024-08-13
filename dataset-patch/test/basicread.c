#include "dio.h"
#include <stdio.h>

int main(int argc, char* argv[]) {
  int rc;
  struct DFILE* dfile;
  char ccsidstr[DCCSID_MAX];
  char ds[54+1+2+2];

  if (argc != 3) {
    fprintf(stderr, "Syntax: argv[0] <hlq> <relative-dataset>\n");
    return 4;
  }
  const char* hlq = argv[1];
  const char* relds = argv[2];

  if (strlen(hlq) > 8 || strlen(ds) > 45) {
    fprintf(stderr, "HLQ and/or relative dataset name too long: %s %s\n", hlq, ds);
    return 16;
  }
  sprintf(ds, "//'%s.%s'", hlq, relds);

  dfile = open_dataset(ds);
  if (!dfile) {
    perror("open failed for read");
    return 4;
  }
  printf("Dataset attributes for dataset %s: dsorg:%s recfm:%s lrecl:%d ccsid:%s\n",
    dfile->dataset_name, dsorgs(dfile->dsorg), recfms(dfile->recfm), dfile->reclen, dccsids(dfile->dccsid, ccsidstr));

  rc = read_dataset(dfile);
  if (rc) {
    perror("read of dataset failed");
    return 4;
  }

  printf("Read %d bytes from dataset %s\n", dfile->bufflen, relds);

  rc = close_dataset(dfile);
  if (rc) {
    perror("close of dataset failed");
    return 4;
  }
  return 0;
}
