#include <dio.h>
#include <stdio.h>
#include <stdlib.h>

#include <stdio.h>
#include <stdlib.h>
#include "s99.h"
#include "dio.h"
#include "dioint.h"
#include "wrappers.h"

#define DD_SYSTEM "????????"
#define DS_MAX (44)

const struct s99_rbx s99rbxtemplate = {"S99RBX",S99RBXVR,{0,1,0,0,0,0,0},0,0,0};

static int pdsdd_alloc(struct s99_common_text_unit* dsn, struct s99_common_text_unit* dd, struct s99_common_text_unit* disp)
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

int S99(struct s99rb* __ptr32 s99rb)
{
  return S99A(s99rb);
}
int S99MSG(struct s99_em* __ptr32 s99em)
{
  return S99MSGA(s99em);
}

void dumpstg(FILE* stream, void* p, size_t len)
{
  char* buff = p;
  size_t i;
  for (i=0; i<len; ++i) {
    if ((i != 0) && (i % 16 == 0)) {
      fprintf(stream, "\n");
    }
    if (i % 4 == 0) {
      fprintf(stream, " ");
    }
    fprintf(stream, "%2.2X", buff[i]);
  }
}

struct DFILE* open_dataset(const char* dataset_name)
{
  struct DFILE* dfile = malloc(sizeof(struct DFILE));
  if (!dfile) {
    return NULL;
  }
  struct DIFILE* difile = malloc(sizeof(struct DIFILE));
  if (!difile) {
    return NULL;
  }
  dfile->internal = difile;

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
  free(difile);
  free(dfile);
  return rc;
}
