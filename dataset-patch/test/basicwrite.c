#include "dio.h"
#include <stdio.h>
#include <stdlib.h>

static int calc_size(char** data, int length_prefix, size_t reclen)
{
  int tot_size = 0;
  int i=0;
  if (length_prefix) {
    while (data[i] != NULL) {
      tot_size += sizeof(uint16_t);
      tot_size += strlen(data[i]);
      ++i;
    }
  } else {
    while (data[i] != NULL) {
      tot_size += reclen;
      ++i;
    }
  }
  return tot_size;
}

static void copy_data(char* buffer, char** data, int length_prefix, size_t maxreclen)
{
  int i=0;
  size_t offset = 0;
  uint16_t reclen;
  while (data[i] != NULL) {
    reclen = strlen(data[i]);
    if (length_prefix) {
      memcpy(&buffer[offset], &reclen, sizeof(reclen));
      offset += sizeof(reclen);
      memcpy(&buffer[offset], data[i], reclen);
      offset += reclen;
    } else {
      memcpy(&buffer[offset], data[i], reclen);
      memset(&buffer[offset+reclen], 0, maxreclen-reclen);
      offset += maxreclen;
    }
    ++i;
  }
}

int main(int argc, char* argv[]) {
  int rc;
  struct DFILE* dfile;
  char ccsidstr[DCCSID_MAX];
  char ds[54+1+2+2];

  /*
   * Make first character blank so that ASA datasets see a valid
   * character
   */
  char* data[] = { " Line 1", " Line Number 2", " Line # 3", NULL };

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
    relds, dsorgs(dfile->dsorg), recfms(dfile->recfm), dfile->reclen, dccsids(dfile->dccsid, ccsidstr));

  int length_prefix = has_length_prefix(dfile->recfm);

  dfile->bufflen = calc_size(data, length_prefix, dfile->reclen);
  dfile->buffer = malloc(dfile->bufflen);
  if (!dfile->buffer) {
    perror("unable to acquire storage");
    return 4;
  }

  copy_data(dfile->buffer, data, length_prefix, dfile->reclen);
  rc = write_dataset(dfile);
  if (rc) {
    perror("write of dataset failed");
    return 4;
  }

  printf("Wrote %d bytes to dataset %s\n", dfile->bufflen, relds);

  rc = close_dataset(dfile);
  if (rc) {
    perror("close of dataset failed");
    return 4;
  }
  return 0;
}
