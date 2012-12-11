#include "macros.h"
#include <stdio.h>


struct OPTION{
  const char*url,*title,*color,*file;
  bool help,reset,url_read,title_read,color_read;
};

void printOptions(OPTION*op){
  printf("url:%s\ntitle:%s\ncolor:%s\nfile:%s\n",
	 op->url,
	 op->title,
	 op->color,
	 op->file);
  printf("help:%d\nreset:%d\n-u:%d\n-t:%d\n-c:%d\n",
	 op->help,op->reset,op->url_read,op->title_read,op->color_read);
	 
}

void optput(OPTION*op,const char*key,const char*val){
  if(streq(key,"-u"))op->url=val;
  else if(streq(key,"-t"))op->title=val;
  else if(streq(key,"-c"))op->color=val;
  else if(streq(key,"-f"))op->file=val;
}
void optflag(OPTION*op,const char*key){
  if(streq(key,"-h")||streq(key,"--help"))op->help=true;
  else if(streq(key,"-reset"))op->reset=true;
  else if(streq(key,"-u"))op->url_read=true;
  else if(streq(key,"-t"))op->title_read=true;
  else if(streq(key,"-c"))op->color_read=true;
}
void showHelp(){
  printf("some help message");
}
OPTION parseArgs(char**argv){
  char*prev=NULL;
  OPTION op={NULL,NULL,NULL,NULL,false,false};
  for(;*argv;argv++){
    char*s=*argv;
    if(s[0]=='-'){
      if(prev)optflag(&op,prev);
      prev=s;
    }else{
      if(prev)optput(&op,prev,s);
      prev=NULL;
    }
  }
  if(prev)optflag(&op,prev);
  return op;
}

