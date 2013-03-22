/* preconfigured options for the program */

/* (default) program options */
#define SERVPORT 27183
#define QUEUELEN 32

/* SQL options */
#define SQLSERV "localhost"
#define SQLUSER "root"
#define SQLPASS ""
#define SQLDATB "ctest"
#define SQLPORT 0

/* buffer length is the same across all files */
#define BUFFLEN 255

//#define debugout 1
#undef debugout
#ifdef debugout
#define DEBUGOUT(...) fprintf(stdout, __VA_ARGS__)
#else
#define DEBUGOUT(...)
#endif
