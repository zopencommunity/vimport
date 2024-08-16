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
    /*
     * dataset_full_name: this is the fully-qualified name of the dataset AND
     * the dataset member, if present
     *
     * dataset_name: this is the fully-qualified name of the dataset
     *
     * member_name: this is the member name if specified. If no member name
     * is present, the member_name will be set to binary 0 (NULL string)
     *
     * hlq: the high level qualifier for the dataset, which may or may not
     * have been specified on the open_dataset call.
     *
     * mlqs: the mid-level qualifiers of the dataset. It is rare, but this
     * could be set to binary 0 (NULL string) if no mlqs existed.
     *
     * llq: the low-level qualifier of the dataset. This will always exist.
     *
     * ddname: the system-generated DDName after a successful call to open_dataset.
     *
     * fp: the active file pointer after a successful call to open_dataset.
     *
     * dstate: the current state of the file pointer.
     * 
     * read_buffer_size: the size of the buffer in DFILE (this is likely larger
     * than the number of bytes read into the buffer).
     */
    char dataset_full_name[DS_FULL_MAX+1];
    char dataset_name[DS_MAX+1];
    char member_name[MEM_MAX+1];
    char hlq[HLQ_MAX+1];
    char mlqs[MLQS_MAX+1];
    char llq[LLQ_MAX+1];
    char ddname[DD_MAX+1];
    FILE* fp;
    enum DSTATE dstate;
    size_t read_buffer_size;
    size_t cur_read_offset;
  };

  void errmsg(struct DFILE* dfile, const char* format, ...);
#endif
