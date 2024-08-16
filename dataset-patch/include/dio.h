#ifndef __DIO_H__
  #define __DIO_H__ 1

  #include <stddef.h>
  #include <stdint.h>
  #include <limits.h>
  #include <stdio.h>

  /*
   * Services that take a string or return a string are ASCII/EBCDIC aware.
   * If the program is running in ASCII mode, input strings are expected in
   * ASCII and returned strings will be in ASCII.
   * If the program is running in EBCDIC mode, input strings are expected in
   * EBCDIC and returned strings will be in EBCDIC.
   * 
   * Note that read_dataset and write_dataset are functions that work with
   * binary data, and therefore no conversion is performed on the buffers.
   */
  enum DRECFM { 
    D_V=1, 
    D_F=2, 
    D_U=3, 
    D_VA=4, 
    D_FA=5,
    DRECFM_FORCE_INT=INT_MAX
  };

  enum DSORG { 
    D_PDS=1, 
    D_PDSE=2, 
    D_SEQ=3,
    DSORG_FORCE_INT=INT_MAX
  };

  #define DCCSID_BINARY (-1)
  #define DCCSID_NOTSET (0)

  /*
   * Error message return code and last error message 
   */
  enum DIOERR {
    DIOERR_NOERROR=0,
    DIOERR_SVC99INIT_FAILURE=1,
    DIOERR_SVC99INIT_ALLOC_FAILURE=2,
    DIOERR_SVC99INIT_FREE_FAILURE=3,
    DIOERR_SVC99_ALLOC_FAILURE=4,
    DIOERR_INVALID_DATASET_NAME=5,
    DIOERR_LE_DATASET_NAME_TOO_LONG_OR_TOO_SHORT=6,
    DIOERR_INVALID_LE_DATASET_NAME=7,
    DIOERR_LE_DATASET_NAME_QUOTE_MISMATCH=8,
    DIOERR_RELATIVE_DATASET_NAME_NOT_IMPLEMENTED_YET=9,
    DIOERR_MEMBER_NAME_TOO_LONG=10,
    DIOERR_LE_DATASET_NAME_PAREN_MISMATCH=11,
    DIOERR_FCLOSE_FAILED_ON_READ=12,
    DIOERR_OPENDD_FOR_READ_FAILED=13,
    DIOERR_READ_BUFFER_ALLOC_FAILED=14,
    DIOERR_LARGE_READ_BUFFER_NOT_IMPLEMENTED_YET=15,
    DIOERR_INVALID_BUFFER_PASSED_TO_WRITE=16,
    DIOERR_FCLOSE_FAILED_ON_WRITE=17,
    DIOERR_OPENDD_FOR_WRITE_FAILED=18,
    DIOERR_FCLOSE_FAILED_ON_CLOSE=19,
    DIOERR_MALLOC_FAILED=20,
    DIOERR_FLDATA_FAILED=21,
    DIOERR_UNSUPPORTED_RECFM=22,
    DIOERR_UNSUPPORTED_DSORG=23,
    DIOERR_FOPEN_FOR_READ_FAILED=24,
    DIOERR_FREAD_FAILED=25,
    DIOERR_FWRITE_FAILED=26,
    DIOERR_FORCE_INT=INT_MAX
  };

  struct DFILE {
    char* buffer; 
    size_t bufflen; 
    uint16_t reclen;
    enum DRECFM recfm;
    enum DSORG dsorg;
    int dccsid;
    enum DIOERR err;
    char* msgbuff;
    size_t msgbufflen;
    int readonly:1; 
    int is_binary;
    FILE* logstream;
    void* internal;
  };
    
  /* CONSTANTS */
  #define DD_MAX (8)
  #define HLQ_MAX (8)
  #define LLQ_MAX HLQ_MAX
  #define DS_MAX (44)
  #define MEM_MAX (8)
  #define DS_FULL_MAX (DS_MAX+MEM_MAX+2)
  #define EXTENSION_MAX 16

  /*
   * open_dataset: given a dataset name in the format:
   *  //'qual[.qual]*[(mem)]'
   * or:
   *  //qual[.qual]*[(mem)]
   *
   * and a stream to log errors to (logstream).
   * If no logging is required, logstream should be NULL.
   * return a DFILE structure. 
   * DFILE can be subsequently passed to the
   * read_dataset, write_dataset, close_dataset
   * services. 
   * Returns 0 if successful, non-zero otherwise
   * Will set errno if an I/O error occurred 
   * Will update msgbuff with error text for the failure
   * Detailed error messages will be written to logstream
   *
   * On successful return from open_dataset:
   * - dataset_name : will be the name of the dataset specified
   * - buffer : will be NULL
   * - bufflen : will be 0
   * - reclen : will be the record length of the dataset. Note for 
   *   variable record formats, the length will include the 4 byte
   *   length prefix. Note for unformatted datasets, the length returned
   *   will be 0.
   * - recfm : will be the record format of the dataset
   * - dsorg : will be the dataset organization of the dataset
   * - readonly: will be set to 1 if the dataset or dataset member can only be opened read-only
   * - logstream : stream to write detailed errors to
   * 
   */
  struct DFILE* open_dataset(const char* dataset_name, FILE* logstream);

  /*
   * read_dataset: read a dataset or dataset member into a buffer.
   * Given a DFILE structure returned from open_dataset,
   * read_dataset will read the specified dataset, or dataset member
   * in binary or text (as specified by the is_binary flag).
   * Returns 0 if successful, non-zero otherwise.
   * Will set errno if an I/O error occurred
   * Will update msgbuff with error text for the failure
   * Detailed error messages will be written to logstream
   * If the return code is 0, the DFILE structure will update the following fields:
   * - buffer will point to a buffer containing the contents of the entire dataset 
   *   (or dataset member). Variable length records will have the length encoded
   *   in each record. No extra data such as newlines are added to the buffer.
   * - bufflen will be the length of the buffer.
   */
  enum DIOERR read_dataset(struct DFILE* dfile);

  /*
   * write_dataset: write a buffer to a dataset or dataset member.
   * Given a DFILE structure returned from open_dataset,
   * and a valid buffer and buffer length, write_dataset will write the
   * buffer to the dataset, or dataset member, replacing the previous contents,
   * in binary or text mode as specified by the is_binary flag.
   * Returns 0 if successful, non-zero otherwise.
   * Will set errno if an I/O error occurred
   * Will update msgbuff with error text for the failure
   * Detailed error messages will be written to logstream
   * On entry to write_dataset:
   * - buffer will point to a buffer containing the contents of the entire dataset 
   *   (or dataset member) to be written. Variable length records will have the length encoded
   *   in each record. No extra data such as newlines are added to the buffer.
   * - bufflen will be the length of the buffer.
   * Records must not be longer than the logical record length of the dataset. 
   */
  enum DIOERR write_dataset(struct DFILE* dfile); 

  /*
   * close_dataset: close the dataset and free the DFILE structure.
   * NOTE: The buffer is NOT freed by close_dataset. 
   * The caller should free the buffer when appropriate.
   * Returns 0 if successful, non-zero otherwise.
   * Will update msgbuff with error text for the failure
   * Detailed error messages will be written to logstream
   * Will set errno if an I/O error occurred
   */ 
  enum DIOERR close_dataset(struct DFILE* dfile);

  /*
   * dsorgs: return a string representing the dataset organization
   * Note: a buffer at least DSORG_MAX should be passed in
   */
  #define DSORG_MAX (5)
  const char* dsorgs(enum DSORG dsorg, char* buff);

  /*
   * recfms: return a string representing the record format
   * Note: a buffer at least DRECFM_MAX should be passed in
   */
  #define RECFM_MAX (3)
  const char* recfms(enum DRECFM drecfm, char* buff);

  /*
   * dccsids: return a string representing the CCSID
   * Note: a buffer at least DCCSIDS_MAX should be passed in
   */
  #define DCCSID_MAX (11)
  const char* dccsids(int dccsid, char* buff);

  /*
   * has_length_prefix: returns 1 if the record format requires length prefixes,
   *                    0 otherwise.
   */
  int has_length_prefix(enum DRECFM recfm);

  /*
   * low level qualifier: return low level qualifier of the dataset
   * Note: a buffer at least LLQ_MAX should be passed in
   */
  const char* low_level_qualifier(struct DFILE* dfile, char* buff);

  
  /*
   * member_name: return the member name if one exists
   * Note: a buffer at least MEM_MAX+1 should be passed in
   */
  const char* member_name(struct DFILE* dfile, char* member_copy);

  /*
   * high level qualifier: return the hlq if one exists
   * Note: a buffer at least HLQ_MAX+1 should be passed in
   */
  const char* high_level_qualifier(struct DFILE* dfile, char* hlq_copy);
  
  /*
   * high_to_mid_level_qualifier: return high to mid-level qualifier
   * Note: a buffer at least DS_MAX+1 should be passed in
   */
  const char* high_to_mid_level_qualifier(struct DFILE* dfile, char* hmql_copy);

  /*
   * map_to_unixfile: map the dataset to a unix filename
   * Note: a buffer at least PATH_MAX should be passed in
   */
  const char* map_to_unixfile(struct DFILE* dfile, char* unixfile);

  /*
   * is_dataset: return true if a filename is a dataset or false otherwise
   */
  static int is_dataset(const char *filename) {
    if (filename == NULL || strlen(filename) < 2) {
      return 0;
    }

    if (filename[0] == '/' && filename[1] == '/') {
      return 1;
    }

    return 0;
  }

  /*
   * is_binary: checks if a string conains binary content
   */
  int is_binary(const char *str, int length);

#endif
