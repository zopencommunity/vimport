diff --git a/src/vim.h b/src/vim.h
index 9c1434cc6..748c3ae25 100644
--- a/src/vim.h
+++ b/src/vim.h
@@ -283,6 +283,7 @@
 // a concrete example, gcc-3.2 enforces exception specifications, and
 // glibc-2.2.5 has them in their system headers.
 #if !defined(__cplusplus) && defined(UNIX) \
+  && !defined(__MVS__) \
 	&& !defined(MACOS_X) // MACOS_X doesn't yet support osdef.h
 # include "auto/osdef.h"	// bring missing declarations in
 #endif
