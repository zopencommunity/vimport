#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "s99.h"
#include "dio.h"
#include "dioint.h"
#include "wrappers.h"

/* #define DEBUG 1 */
#define DD_SYSTEM "????????"
#define ERRNO_NONEXISTANT_FILE (67)

const struct s99_rbx s99rbxtemplate = {"S99RBX",S99RBXVR,{0,1,0,0,0,0,0},0,0,0};

static int dsdd_alloc(struct s99_common_text_unit* dsn, struct s99_common_text_unit* dd, struct s99_common_text_unit* disp)
{
  struct s99rb* __ptr32 parms;
  enum s99_verb verb = S99VRBAL;
  struct s99_flag1 s99flag1 = {0};
  struct s99_flag2 s99flag2 = {0};
  size_t num_text_units = 3;
  int rc;
  struct s99_rbx s99rbx = s99rbxtemplate;

  parms = s99_init(verb, s99flag1, s99flag2, &s99rbx, num_text_units, dsn, dd, disp );
  if (!parms) {
    fprintf(stderr, "Unable to initialize SVC99 (DYNALLOC) control blocks\n");
    return 16;
  }
  rc = S99(parms);
  if (rc) {
    fprintf(stderr, "SVC99 failed with rc:%d\n", rc);
    s99_fmt_dmp(stderr, parms);
    s99_prt_msg(stderr, parms, rc);
    return(rc);
  }

  struct s99_common_text_unit* ddout = (struct s99_common_text_unit*) parms->s99txtpp[1];
  dd->s99tulng = ddout->s99tulng;
  memcpy(dd->s99tupar, ddout->s99tupar, dd->s99tulng);

  s99_free(parms);
  return 0;
}

int ddfree(struct s99_common_text_unit* dd)
{
  struct s99rb* __ptr32 parms;
  enum s99_verb verb = S99VRBUN;
  struct s99_flag1 s99flag1 = {0};
  struct s99_flag2 s99flag2 = {0};
  size_t num_text_units = 1;
  int rc;
  struct s99_rbx s99rbx = s99rbxtemplate;

  parms = s99_init(verb, s99flag1, s99flag2, &s99rbx, num_text_units, dd );
  if (!parms) {
    fprintf(stderr, "Unable to initialize SVC99 (DYNFREE) control blocks\n");
    return 16;
  }
  rc = S99(parms);
  if (rc) {
    s99_fmt_dmp(stderr, parms);
    s99_prt_msg(stderr, parms, rc);
    return rc;
  }

  s99_free(parms);
  return 0;
}

int init_dsnam_text_unit(const char* dsname, struct s99_common_text_unit* dsn)
{
  size_t dsname_len = (dsname == NULL) ? 0 : strlen(dsname);
  if (dsname == NULL || dsname_len == 0 || dsname_len > DS_MAX) {
    fprintf(stderr, "Dataset Name <%.*s> is invalid\n", dsname_len, dsname);
    return 8;
  }

  dsn->s99tulng = dsname_len;
  memcpy(dsn->s99tupar, dsname, dsname_len);
  return 0;
}

static void strupper(char* str)
{
  for (int i=0; i<strlen(str); ++i) {
    if (islower(str[i])) {
      str[i] = toupper(str[i]);
    }
  }
}

static int has_member(struct DIFILE* difile)
{
  return difile->member_name[0] != '\0';
}
  
static int init_dataset_info(const char* dataset_name, struct DIFILE* difile)
{
  size_t dataset_name_len = strlen(dataset_name);
  size_t ds_full_len;
  const char* dataset_name_start;
  const char* dataset_name_end;

  if ((dataset_name_len > DS_MAX+MEM_MAX+2+2+2) || (dataset_name_len < 2+2)) {
    fprintf(stderr, "Dataset name %s is not a valid dataset name of format: //<dataset> or //'<dataset>'\n", dataset_name);
    return 1;
  }

  if (memcmp(dataset_name, "//", 2)) {
    fprintf(stderr, "Dataset name %s does not start with // and is not a valid dataset name\n", dataset_name);
    return 2;
  }
  if (!memcmp(dataset_name, "//'", 3)) {
    if (dataset_name[dataset_name_len-1] != '\'') {
      fprintf(stderr, "Dataset name %s does not have balanced single quotes\n", dataset_name);
      return 3;
    }
    dataset_name_start = &dataset_name[3];
    dataset_name_end = &dataset_name[dataset_name_len];
  } else {
    dataset_name_start = &dataset_name[2];
    dataset_name_end = &dataset_name[dataset_name_len-1];
    fprintf(stderr, "No support (yet) for open_dataset of relative dataset %s read/write - datasets must be fully qualified\n", dataset_name);
    return 4;
  }
  ds_full_len = dataset_name_end - dataset_name_start - 1;
  const char* open_paren=strchr(dataset_name, '(');
  const char* close_paren=strchr(dataset_name, ')');

  size_t dslen;
  if (open_paren && close_paren) {
    dslen = open_paren - dataset_name_start;
    size_t memlen = close_paren - open_paren - 1;
    if (memlen > MEM_MAX) {
      fprintf(stderr, "Member name of %s is more than %d characters\n", dataset_name, MEM_MAX);
      return 5;
    }
    /* dataset member - valid */
    memcpy(difile->member_name, open_paren+1, memlen);
    difile->member_name[memlen] = '\0';
  } else if (!open_paren && !close_paren) {
    /* dataset - valid */
    dslen = dataset_name_end - dataset_name_start - 1;
    difile->member_name[0] = '\0';
  } else {
    /* mis-matched parens - invalid */
    fprintf(stderr, "Dataset %s is not a valid dataset name or dataset member name\n", dataset_name);
  }
  memcpy(difile->dataset_full_name, dataset_name_start, ds_full_len);
  difile->dataset_full_name[ds_full_len] = '\0';
  memcpy(difile->dataset_name, dataset_name_start, dslen);
  difile->dataset_name[dslen] = '\0';

  strupper(difile->dataset_full_name);
  strupper(difile->dataset_name);
  strupper(difile->member_name);

#ifdef DEBUG
  printf("Original <%s> full <%s> name <%s> member <%s>\n", 
    dataset_name, difile->dataset_full_name, difile->dataset_name, difile->member_name);
#endif
  return 0;
}

const char* dsorgs(enum DSORG dsorg)
{
  switch(dsorg) {
    case D_PDS: return "PDS";
    case D_PDSE: return "PDSE";
    case D_SEQ: return "SEQ";
  }
  return "UNK";
}

const char* recfms(enum DRECFM drecfm)
{
  switch(drecfm) {
    case D_F: return "F";
    case D_V: return "V";
    case D_U: return "U";
    case D_VA: return "VA";
    case D_FA: return "FA";
  }
  return "UNK";
}

const char* dccsids(int dccsid, char* buf)
{
  switch(dccsid) {
    case DCCSID_NOTSET:
      strcpy(buf, "?");
      break;
    case DCCSID_BINARY:
      strcpy(buf, "B");
      break;
    default:
      sprintf(buf, "%u", dccsid);
      break;
  }
  return buf;
}

static const char* dstates(enum DSTATE dstate)
{
  switch(dstate) {
    case D_CLOSED: return "closed";
    case D_READ_BINARY: return "rb";
    case D_WRITE_BINARY: return "wb";
  }
  return "UNK";
}

struct DFILE* open_dataset(const char* dataset_name)
{

  struct DFILE* dfile = calloc(1, sizeof(struct DFILE));
  if (!dfile) {
    return NULL;
  }
  struct DIFILE* difile = calloc(1, sizeof(struct DIFILE));
  if (!difile) {
    free(dfile);
    return NULL;
  }
  if (init_dataset_info(dataset_name, difile)) {
    free(dfile);
    free(difile);
    return NULL;
  }

  dfile->internal = difile;

  struct s99_common_text_unit dsn = { DALDSNAM, 1, 0, 0 };
  struct s99_common_text_unit dd = { DALRTDDN, 1, sizeof(DD_SYSTEM)-1, DD_SYSTEM };
  struct s99_common_text_unit stats = { DALSTATS, 1, 1, {0x8} };

  int rc = init_dsnam_text_unit(difile->dataset_name, &dsn);
  if (rc) {
    return NULL;
  }
  rc = dsdd_alloc(&dsn, &dd, &stats);
  if (rc) {
    return NULL;
  }

  memcpy(difile->ddname, dd.s99tupar, dd.s99tulng);
  difile->ddname[dd.s99tulng] = '\0';

  difile->dstate = D_CLOSED;

#ifdef DEBUG
  printf("allocated ddname:%s\n", difile->ddname);
#endif

  /*
   * Note - there is a timing window here and it is not efficient to 
   * open the dataset twice (once to get the dataset characteristics and once to read or write)
   * but this is 'good enough' for now since the C I/O services don't let us do better 
   */

  char copendd[DD_MAX+MEM_MAX+2+2+2+1+1];
  if (has_member(difile)) {
    sprintf(copendd, "//DD:%s(%s)", difile->ddname, difile->member_name);
  } else {
    sprintf(copendd, "//DD:%s", difile->ddname);
  }
  difile->fp = fopen(copendd, "rb");
  if (difile->fp) {
    difile->dstate = D_READ_BINARY;
  } else {
    if ((errno == ERRNO_NONEXISTANT_FILE) && has_member(difile)) {
      /*
       * This is a PDS or PDSE member, and the member does not exist yet.
       * We need to open the member in write to get the attributes of the actual
       * PDS(E) member, otherwise if we only specify the PDS(E), we will get
       * the attributes for working with the PDS(E) directly, which is wrong (unformatted)
       * This is 'ok' because if we don't have write access to the PDS(E) to create a 
       * member, we should know this now now rather than later.
       */
     
      difile->fp = fopen(copendd, "wb");
      if (!difile->fp) {
        return NULL;
      }
      difile->dstate = D_WRITE_BINARY;
    } else {
      fprintf(stderr, "Errno: %d Unable to open dataset %s to retrieve file information\n", errno, dataset_name);
      return NULL;
    }
  }

  fldata_t info;
  rc = __fldata(difile->fp, NULL, &info);
  if (rc) {
    fprintf(stderr, "Unable to obtain file information for %s\n", dataset_name);
    close_dataset(dfile);
    return NULL;
  }

  if (info.__recfmF) {
    if (info.__recfmASA) {
      dfile->recfm = D_FA;
    } else {
      dfile->recfm = D_F;
    }
  } else if (info.__recfmV) {
    if (info.__recfmASA) {
      dfile->recfm = D_VA;
    } else {
      dfile->recfm = D_V;
    }
  } else if (info.__recfmU) {
    dfile->recfm = D_U;
  } else {
    fprintf(stderr, "Dataset %s is not F, V, or U format. open_dataset not supported at this time\n", dataset_name);
    return NULL;
  }

  if (info.__dsorgPDSE) {
    dfile->dsorg = D_PDSE;
  } else if (info.__dsorgPO) {
    dfile->dsorg = D_PDS;
  } else if (info.__dsorgPS) {
    dfile->dsorg = D_SEQ;
  } else {
    fprintf(stderr, "Dataset %s is not PDS, PDSE, or SEQ organization. open_dataset not supported at this time\n", dataset_name);
    return NULL;
  }

  dfile->reclen = info.__maxreclen;
  dfile->dccsid = DCCSID_NOTSET; 

#ifdef DEBUG
  char ccsidstr[DCCSID_MAX];
  printf("Dataset attributes: dsorg:%s recfm:%s lrecl:%d dstate:%s ccsid:%s\n", 
    dsorgs(dfile->dsorg), recfms(dfile->recfm), dfile->reclen, dstates(difile->dstate), dccsids(dfile->dccsid, ccsidstr));
#endif

  return dfile;
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

  int rc = 0;
  struct DIFILE* difile = (struct DIFILE*) dfile->internal;

  rc = fclose(difile->fp);

  free(difile);
  free(dfile);

  return rc;
}
