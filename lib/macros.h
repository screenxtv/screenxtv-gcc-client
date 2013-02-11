#ifndef MACROS_H
#define MACROS_H
#include <string.h>
#define errorexit(...) (fprintf(stderr,__VA_ARGS__),exit(-1))
struct KeyValue{const char*key,*value;};
int strcpylen(char*dst,const char*src){strcpy(dst,src);return strlen(dst);}
char*strclone(const char*s){if(!s)return NULL;char*s2=new char[strlen(s)+1];strcpy(s2,s);return s2;}
int strpos(const char*str,char c){return strchr(str,c)-str;}
bool streq(const char*a,const char*b){return strcmp(a,b)==0;}
#define LARGE_BUF_SIZE 65536
#define SMALL_BUF_SIZE 256
#endif
