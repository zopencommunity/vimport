#include "dio.h"
#include <stdio.h>
#include <limits.h>

int main(int argc, char* argv[]) {
  int rc;
  struct DFILE* dfile;
  char ds[54+1+2+2];
  char dsorgbuff[DSORG_MAX];
  char recfmbuff[RECFM_MAX];
  char ccsidbuff[DCCSID_MAX];
  char mem[MEM_MAX+1];
  char hlqstr[MEM_MAX+1];
  char mlqs[DS_MAX+1];
  char llq[DS_MAX+1];
  char unix[_POSIX_PATH_MAX];

  if (argc != 2 && argc != 3) {
    fprintf(stderr, "Syntax: %s [<hlq>] <relative-dataset>\n", argv[0]);
    return 4;
  }
  const char* hlq;
  const char* relds;

  if (argc == 3) {
    relds = argv[2];
  } else {
    relds = argv[1];
  }
  if (strlen(ds) > 45) {
    fprintf(stderr, "relative dataset name too long: %s\n", relds);
    return 16;
  }
  if (argc == 3) {
    hlq = argv[1];
    if (strlen(hlq) > 8) {
      fprintf(stderr, "HLQ too long: %s\n", hlq);
      return 16;
    }
    sprintf(ds, "//'%s.%s'", hlq, relds);
  } else {
    sprintf(ds, "//%s", relds);
  }
  
  printf("ds:%s\n", ds);
  dfile = open_dataset(ds, stderr);
  if (!dfile || dfile->err) {
    if (dfile) {
      fprintf(stderr, "%s\n", dfile->msgbuff);
    }
    return 4;
  }
  printf("Dataset attributes for dataset %s: mem:%s hlq:%s mlqs:%s llq:%s dsorg:%s recfm:%s lrecl:%d ccsid:%s\n",
    relds, member_name(dfile, mem), high_level_qualifier(dfile, hlqstr), mid_level_qualifiers(dfile, mlqs), low_level_qualifier(dfile, llq), dsorgs(dfile->dsorg, dsorgbuff), recfms(dfile->recfm, recfmbuff), dfile->reclen, dccsids(dfile->dccsid, ccsidbuff));

  printf("Unix file: %s\n", map_to_unixfile(dfile, unix));

  rc = read_dataset(dfile);
  if (rc) {
    fprintf(stderr, "%s\n", dfile->msgbuff);
    return 4;
  }

  printf("Read %d bytes from dataset %s\n", dfile->bufflen, relds);

  rc = close_dataset(dfile);
  if (rc) {
    fprintf(stderr, "%s\n", dfile->msgbuff);
    return 4;
  }
  return 0;
}
