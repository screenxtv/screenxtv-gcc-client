//g++ main.cc -lpthread -lutil
#include "lib/conf.h"
#include "lib/socket.h"
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


int fd,pid;
winsize win;

KVSocket*sock;
Config*config=NULL;
void*loop_chat(void*data){
	char*file=(char*)data;
	sock->send("chat",file);
	FILE*fp=fopen(file,"w+");
	if(!fp)return NULL;
	rewind(fp);
	char buf[1024];
	while(true){
		usleep(1000);
		rewind(fp);
		int len=fread(buf,1,sizeof(buf),fp);
		if(!len)continue;
		buf[len]='\0';
		if(!sock->send("chat",trim(buf)))break;
		fclose(fp);
		fp=fopen(file,"w+");
	}
	fclose(fp);
	return NULL;
}

void chldfunc(int n){wait(NULL);}
void winchfunc(int n){
	ioctl(STDOUT_FILENO,TIOCGWINSZ,&win);
	char buf[256];
	sprintf(buf,"{\"width\":%d,\"height\":%d}",win.ws_col,win.ws_row);
	sock->send("winch",buf);
	ioctl(fd,TIOCSWINSZ,&win);
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
void onclose(KVSocket*sock){
	fclose(stdin);
}
void ondata(KVSocket*sock,char*key,char*value){
	if(strcmp(key,"slug")==0){
		int len=strlen(value);
		value[0]=value[len-1]='\0';
		config->put("slug",value+1);
		config->save();
	}
	if(strcmp(key,"listener")==0){
		
	}if(strcmp(key,"chat")==0){
		
	}if(strcmp(key,"init")==0){

	}if(strcmp(key,"error")==0){
		fclose(stdin);
	}
}
int main(int argc, char *argv[]){
	char buf[65536];
	config=new Config("screenxtv.conf",DEFAULT_SETTINGS);
	if(!config->load()){
		for(int i=0;SCAN_SETTINGS[i].key;i++){
			KeyValue kv=SCAN_SETTINGS[i];
			printf("%s\n>",kv.value);fflush(stdout);
			scanf("%s",buf);
			config->put(kv.key,buf);
		}
		config->save();
	}
	ioctl(STDOUT_FILENO,TIOCGWINSZ,&win);
	signal(SIGWINCH,winchfunc);
	signal(SIGCHLD,chldfunc);

	sock=new KVSocket(config->get("host"),8000);
	sock->setCloseCallback(onclose);
	sock->setDataCallback(ondata);

	sprintf(buf,"{\"width\":%d,\"height\":%d,\"slug\":\"%s\",\"info\":{\"color\":\"%s\",\"title\":\"%s\",\"description\":\"%s\"}}",
		win.ws_col,win.ws_row,config->get("slug"),config->get("color"),config->get("title"),config->get("description"));
	sock->send("init",buf);


	struct termios tm,tm2;
	tcgetattr(STDIN_FILENO,&tm);
	tm2=tm;
	cfmakeraw(&tm2);

	tcsetattr(STDIN_FILENO,TCSANOW,&tm2);
	setenv("TERM","vt100",1);
	setenv("LANG","ja_JP.UTF-8",1);
	if(!(pid=forkpty(&fd,NULL,NULL,&win)))
		execlp("screen","screen","-x",argc>1?argv[1]:"screenx","-R",NULL);
	pthread_t ptt;
	//pthread_create(&ptt,NULL,loop_chat,(void*)"/Users/tomoya/.Trash/chat_in");
	pthread_create(&ptt,NULL,loop_fdread,NULL);
	loop_fdwrite(NULL);
	tcsetattr(STDIN_FILENO,TCSANOW,&tm);
	tcsetattr(STDOUT_FILENO,TCSANOW,&tm);
	tcsetattr(STDERR_FILENO,TCSANOW,&tm);
}
