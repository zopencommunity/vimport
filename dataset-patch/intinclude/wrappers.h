/*
 * wrappers for C 31-bit or 64-bit functions
 */
#include <stdlib.h>

#if AMODE==31
  #pragma linkage(S99A,     OS)
  #pragma linkage(S99MSGA,  OS)

  struct s99rb;
  struct s99_em;

  int S99A(struct s99rb* __ptr32 rb);
  int S99MSGA(struct s99_em* __ptr32 em);

  #define MALLOC31(bytes) (malloc(bytes))
  #define FREE31(ptr)     (free(ptr))

#elif AMODE == 64
  extern int S99A;
  extern int S99MSGA;
  #pragma variable(S99A,     NORENT)
  #pragma variable(S99MSGA,  NORENT)
	#define S99A(ptr)         call31asm("S99A", &S99A, 1, ptr)
	#define S99MSGA(ptr)      call31asm("S99MSGA", &S99MSGA, 1, ptr)

  #define MALLOC31(bytes) (__malloc31(bytes))
  #define FREE31(ptr)     (free(ptr))
#else
	#error "AMODE must be 31 or 64"
#endif
