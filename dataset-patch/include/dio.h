#ifndef __DIO_H__
  #define __DIO_H__ 1

  #include <stddef.h>
  #include <stdint.h>
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
    uint16_t reclen;
    enum DRECFM recfm;
    enum DSORG dsorg;
    int dccsid;
    void* internal;
  };

  /*
   * open_dataset: given a dataset name in the format:
   *  //'qual[.qual]*[(mem)]'
   * or:
   *  //qual[.qual]*[(mem)]
   *
   * return a DFILE structure. 
   * DFILE can be subsequently passed to the
   * read_dataset, write_dataset, close_dataset
   * services. 
   * Returns 0 if successful, non-zero otherwise
   * Will set errno if an I/O error occurred
   *
   * On successful return from open_dataset:
   * - dataset_name : will be the name of the dataset specified
   * - buffer : will be NULL
   * - reclen : will be the record length of the dataset. Note for 
   *   variable record formats, the length will include the 4 byte
   *   length prefix. Note for unformatted datasets, the length returned
   *   will be 0.
   * - recfm : will be the record format of the dataset
   * - dsorg : will be the dataset organization of the dataset
   * 
   */
  struct DFILE* open_dataset(const char* dataset_name);

  /*
   * read_dataset: read a dataset or dataset member into a buffer.
   * Given a DFILE structure returned from open_dataset,
   * read_dataset will read the specified dataset, or dataset member
   * in binary or text (as specified by the is_binary flag).
   * Returns 0 if successful, non-zero otherwise.
   * Will set errno if an I/O error occurred
   * If the return code is 0, the DFILE structure will update the following fields:
   * - buffer will point to a buffer containing the contents of the entire dataset 
   *   (or dataset member). Variable length records will have the length encoded
   *   in each record. No extra data such as newlines are added to the buffer.
   * - bufflen will be the length of the buffer.
   */
  int read_dataset(struct DFILE* dfile);

  /*
   * write_dataset: write a buffer to a dataset or dataset member.
   * Given a DFILE structure returned from open_dataset,
   * and a valid buffer and buffer length, write_dataset will write the
   * buffer to the dataset, or dataset member, replacing the previous contents,
   * in binary or text mode as specified by the is_binary flag.
   * Returns 0 if successful, non-zero otherwise.
   * Will set errno if an I/O error occurred
   * On entry to write_dataset:
   * - buffer will point to a buffer containing the contents of the entire dataset 
   *   (or dataset member) to be written. Variable length records will have the length encoded
   *   in each record. No extra data such as newlines are added to the buffer.
   * - bufflen will be the length of the buffer.
   * Records must not be longer than the logical record length of the dataset. 
   */
  int write_dataset(struct DFILE* dfile); 

  /*
   * close_dataset: close the dataset and free allocated storage in the DFILE structure.
   * Returns 0 if successful, non-zero otherwise.
   * Will set errno if an I/O error occurred
   */ 
  int close_dataset(struct DFILE* dfile);

  /*
   * dsorgs: return a string representing the dataset organization
   */
  const char* dsorgs(enum DSORG dsorg);

  /*
   * recfms: return a string representing the record format
   */
  const char* recfms(enum DRECFM drecfm);

  /*
   * dccsids: return a string representing the CCSID
   * Note: a buffer at least DCCSIDS_MAX should be passed in
   */
  #define DCCSID_MAX (11)
  const char* dccsids(int dccsid, char* buf);
#endif
