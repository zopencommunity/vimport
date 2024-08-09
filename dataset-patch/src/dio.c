  enum DRECFM {
    D_V=1,
    D_F=2,
    D_U=3,
    D_VA=4,
    D_FA=5
  };

  enum DSORG {
    D_PDS=1,
    D_PDSE=2,
    D_SEQ=3
  };

  #define DCCSID_BINARY (-1)
  #define DCCSID_NOTSET (0)

  struct DFILE {
    const char* dataset_name;
    char* buffer;
    size_t bufflen;
    size_t reclen;
    enum DRECFM recfm;
    enum DSORG dsorg;
    int dccsid;
  };

#include <dio.h>
DFILE* open_dataset(const char* dataset_name)
{
  return NULL;
}
int read_dataset(struct DFILE* dfile)
{
  return 0;
}
int write_dataset(struct DFILE* dfile)
{
  return 0;
}

int close_dataset(struct DFILE* dfile)
{
  return 0;
}
