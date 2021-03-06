#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include "macros.h"



char*trim(char*str){
  #define target(c) (c==' '||c=='\t'||c=='\r'||c=='\n')
  while(target(*str))str++;
  for(char*end=str+strlen(str)-1;end!=str&&target(*end);end--)*end='\0';
  #undef target
  return str;
}

class Config{
 public:
  char*filename;
  KeyValue map[LARGE_BUF_SIZE];
  int length;
  void put(const char*key,const char*val){
    for(int i=0;i<length;i++){
      if(streq(key,map[i].key)){
	if(map[i].value)delete[]map[i].value;
	map[i].value=strclone(val);
	return;
      }
    }
    map[length].key=strclone(key);
    map[length].value=strclone(val);
    length++;
  }
  const char*get(const char*key){
    for(int i=0;i<length;i++){
      if(streq(key,map[i].key))return map[i].value;
    }
    return NULL;
  }
  void load();
  void save();
  void clear(){
    for(int i=0;i<length;i++){
      if(map[i].key)delete[]map[i].key;
      if(map[i].value)delete[]map[i].value;
    }
    length=0;
  }
  Config(const char*file){
    filename=strclone(file);
    length=0;
    load();
  }
};
void Config::load(){
  FILE*fp=fopen(filename,"r");
  if(!fp)return;
  char buf[LARGE_BUF_SIZE];
  char key[LARGE_BUF_SIZE];
  int keylen=0;
  char value[LARGE_BUF_SIZE]={0};
  int valuelen=0;
  #define iswhitespace(c) ((c)==' '||(c)=='\t'||(c)=='\r'||(c)=='\n')
  #define addvalue(s) (valuelen+=strcpylen(value+valuelen,(s)),value[valuelen++]='\n',value[valuelen]='\0')
  #define addlist() (put(key,trim(value)),valuelen=0)
  int line=0;
  while(fgets(buf,sizeof(buf),fp)){
    line++;
    if(buf[0])
      if(buf[0]=='-')continue;
      if(iswhitespace(*buf)){
	addvalue(trim(buf));
      }else{
	if(keylen)addlist();
	int colon=strpos(buf,':');
	if(colon<0)errorexit("parse error colon not found (%s):%d\n",filename,line);
	buf[colon]='\0';
	keylen=strcpylen(key,trim(buf));
	addvalue(trim(buf+colon+1));
      }
  }
  if(keylen)addlist();
  fclose(fp);
  #undef isalphabet
  #undef iswhitespace
  #undef addvalue
  #undef addlist
}

void Config::save(){
  char data[LARGE_BUF_SIZE];
  int datalen=0;
  #define pushC(c) (data[datalen++]=(c))
  #define pushS(s) (datalen+=strcpylen(data+datalen,s))
  for(int i=0;i<length;i++){
    KeyValue kv=map[i];
    const char*key=kv.key,*value=kv.value;
    if(!value)continue;
    pushS(key);pushC(':');
    if(strpos(value,'\n')<0){
      pushC(' ');pushS(value);
    }else{
      pushS("\n  ");
      for(char c;c=*value;value++){
	if(c=='\n')pushS("\n  ");
	else pushC(c);
      }
    }
    pushC('\n');
  }
  FILE*fp=fopen(filename,"w");
  fwrite(data,1,datalen,fp);
  fclose(fp);
  #undef pushC
  #undef pushS
}
  
// Credit: http://www.gnu.org/software/libc/manual/html_node/getpass.html
size_t my_getpass (char **lineptr, size_t *n, FILE *stream) {
    struct termios old, new_t;
    int nread;

    /* Turn echoing off and fail if we can't. */
    if (tcgetattr (fileno (stream), &old) != 0)
     return -1;
    new_t = old;
    new_t.c_lflag &= ~ECHO;
    if (tcsetattr (fileno (stream), TCSAFLUSH, &new_t) != 0)
     return -1;

    /* Read the password. */
    nread = getline (lineptr, n, stream);

    /* Restore terminal. */
    (void) tcsetattr (fileno (stream), TCSAFLUSH, &old);

    return nread;
}
