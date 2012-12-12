#include "lib/conf.h"
#include "lib/socket.h"
#include "lib/macros.h"
#include "args.h"
#include "settings.h"
#include <fcntl.h>
#include <stdio.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#if defined(__GLIBC__)
#include <pty.h>
#elif defined(__FreeBSD__)||defined(__FreeBSD_kernel__)||defined(__DragonFly__)
#include <libutil.h>
#else
#include <util.h>
#endif

void exitWithMessage(const char*);
struct termios termios_saved;

int fd,pid;
KVSocket*sock;
Config*config=NULL;

void chldfunc(int n){
  exitWithMessage("broadcast end.");
}

void sendwinch(){
  winsize win;
  ioctl(STDOUT_FILENO,TIOCGWINSZ,&win);
  char buf[256];
  sprintf(buf,"{\"width\":%d,\"height\":%d}",win.ws_col,win.ws_row);
  sock->send("winch",buf);
  ioctl(fd,TIOCSWINSZ,&win);
}
void*winchthreadfunc(void*){
  sendwinch();
}
void winchfunc(int n){
  pthread_t ptt;
  pthread_create(&ptt,NULL,winchthreadfunc,NULL);
}
void*loop_fdread(void*){
	char buf[8192];int rd;
	while((rd=read(fd,buf,sizeof(buf)))>0){
		write(STDOUT_FILENO,buf,rd);
		sock->send("data",buf,rd);
	}
	fclose(stdin);
}
void*loop_fdwrite(void*){
  char buf[8192];
  int len;
  while((len=read(STDIN_FILENO,buf,sizeof(buf)))>0){
    write(fd,buf,len);
  }
}

int main(int argc, char *argv[]){
  if(getenv("SCREENXTV_BROADCASTING")){
    printf("cannot broadcast inside broadcasting screen\n");
    return 0;
  }
  setenv("SCREENXTV_BROADCASTING","1",1);

  winsize win;
  char buf[65536];
  tcgetattr(STDIN_FILENO,&termios_saved);
  OPTION option={0,0,0,0,0,0,0,0,0};
  char default_file[1024];
  sprintf(default_file,"%s/.screenxtv.yml",getenv("HOME"));
  const char*file=default_file;
  option=parseArgs(argv+1);
  if(option.help){showHelp();return 0;}
  if(option.file)file=option.file;


  config=new Config(file);
  if(option.reset)config->clear();
  if(option.url)config->put("url",option.url);
  if(option.title)config->put("title",option.title);
  if(option.color)config->put("color",option.color);
  if(option.url_read)config->put("url",NULL);
  if(option.title_read)config->put("title",NULL);
  if(option.color_read)config->put("color",NULL);
  for(int i=0;SCAN_SETTINGS[i].key;i++){
    ScanMsg sm=SCAN_SETTINGS[i];
    if(config->get(sm.key))continue;
    char*value=NULL;
    if(sm.msg){
      printf("%s\n> ",sm.msg);fflush(stdout);
      fgets(buf,sizeof(buf),stdin);
      char*value=trim(buf);
    }
    config->put(sm.key,value&&strlen(value)?value:sm.value);
  }
    
  
  char key[256],value[65536];
  while(true){
    config->save();
    ioctl(STDOUT_FILENO,TIOCGWINSZ,&win);
    sprintf(buf,"{\"width\":%d,\"height\":%d,\"slug\":\"%s#%s\",\"info\":{\"color\":\"%s\",\"title\":\"%s\"}}",
      win.ws_col,win.ws_row,config->get("url"),config->get("urlhash"),config->get("color"),config->get("title"));
    sock=new KVSocket("screenx.tv",8000);
    sock->send("init",buf);
    sock->read(key,value,sizeof(key),sizeof(value));
    if(streq(key,"slug")){
      char*hash=NULL;
      value[0]=value[strlen(value)-1]='\0';
      for(int i=1;value[i];i++){
        if(value[i]!='#')continue;
        value[i]='\0';
        hash=value+i+1;
        break;
      }
      config->put("url",value+1);
      config->put("urlhash",hash);
      break;
    }
    printf("url already in use. another url?\n> ");fflush(stdout);
    fgets(buf,sizeof(buf),stdin);
    char*val=trim(buf);
    if(strlen(val))config->put("url",val);
  }
  config->save();
  
  printf("Your URL is http://screenx.tv/%s\n\n",config->get("url"));
  printf("Press Enter to start broadcasting\n> ");fflush(stdout);
  fgets(buf,sizeof(buf),stdin);
  
  signal(SIGWINCH,winchfunc);
  signal(SIGCHLD,chldfunc);  
  sendwinch();
  struct termios tm=termios_saved;
  cfmakeraw(&tm);
  
  tcsetattr(STDIN_FILENO,TCSANOW,&tm);
  setenv("TERM","vt100",1);
  setenv("LANG","en_US.UTF-8",1);
  if(!(pid=forkpty(&fd,NULL,NULL,&win)))
    execlp("screen","screen","-x",config->get("screen"),"-R",NULL);
  pthread_t ptt;
  pthread_create(&ptt,NULL,loop_fdread,NULL);
  loop_fdwrite(NULL);
  exitWithMessage("broadcast end.");
}

void exitWithMessage(const char*msg){
  tcsetattr(STDIN_FILENO,TCSANOW,&termios_saved);
  tcsetattr(STDOUT_FILENO,TCSANOW,&termios_saved);
  tcsetattr(STDERR_FILENO,TCSANOW,&termios_saved);
  winsize win;
  ioctl(STDOUT_FILENO,TIOCGWINSZ,&win);
  printf("\e[?1l\e[>\e[1;%dr\e[%d;1H",win.ws_row,win.ws_row);
  printf("%s\n",msg);
  exit(0);
}
