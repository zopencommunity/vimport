#ifndef __DIOINT__
  #define __DIOINT__ 1

  #include <dio.h>

  enum DSTATE {
    D_CLOSED=1,
    D_READ_BINARY=2,
    D_WRITE_BINARY=3,
    D_READWRITE_BINARY=4
  };

  struct DIFILE {
    char dataset_full_name[DS_FULL_MAX+1];
    char dataset_name[DS_MAX+1];
    char member_name[MEM_MAX+1];
    char ddname[DD_MAX+1];
    FILE* fp;
    enum DSTATE dstate;
    size_t read_buffer_size;
    size_t cur_read_offset;
  };

  void errmsg(struct DFILE* dfile, const char* format, ...);
#endif
