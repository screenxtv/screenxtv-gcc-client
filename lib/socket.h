#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>

int socketconnect(const char*host,int port){
	char strport[6];
	sprintf(strport,"%d",port);
	struct addrinfo hints;
	struct addrinfo *res0=NULL;
	int sock=-1;
	for(int i=0;i<sizeof(hints);i++)i[(char*)&hints]=0;
	hints.ai_family=AF_INET;
	hints.ai_socktype=SOCK_STREAM;
	hints.ai_protocol=IPPROTO_TCP;
	if(getaddrinfo(host,strport,&hints,&res0)!=0){
		fprintf(stderr,"getaddrinfo() failed.\n");
		return sock;
	}
	for(struct addrinfo*res=res0;res!=NULL;res=res->ai_next){
		sock=socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if(sock<0)continue;
		if(connect(sock,res->ai_addr,res->ai_addrlen)!=0){
			close(sock);sock=-1;
			continue;
		}
		break;
	}
	freeaddrinfo(res0);
	return sock;
}

void*sockreadthreadfunc(void*);
class Socket{
public:
	int fd;
	Socket(const char*host,int port){
		fd=socketconnect(host,port);
		if(fd<0)errorexit("connect failed\n");
		pthread_t pt;
		pthread_create(&pt,NULL,&sockreadthreadfunc,this);
	}
	virtual void onreceive(const char*data,int len){}
	virtual void onclose(){}
	void closed(){fd=-1;onclose();}
	bool bwrite(const char*data,int len){
		if(fd<0)return false;
		int index=0;
		while(index<len){
			int w=write(fd,data+index,len-index);
			if(w<=0)return false;
			index+=w;
		}
		return true;
	}
	bool swrite(const char*str){return bwrite(str,strlen(str));}
	void disconnect(){close(fd);fd=-1;}
};
class KVSocket:public Socket{
public:
	pthread_mutex_t mutex;
	char sendbuf[2*65536];
	char key[65536],val[65536],*current;
	int currentlen;
	void(*datacallback)(KVSocket*,char*,char*);
	void(*closecallback)(KVSocket*);
	KVSocket(const char*host,int port):Socket(host,port){
		current=key;currentlen=0;
		datacallback=NULL;
		closecallback=NULL;
		pthread_mutex_init(&mutex,NULL);
	}
	void setDataCallback(void(*cb)(KVSocket*,char*,char*)){datacallback=cb;}
	void setCloseCallback(void(*cb)(KVSocket*)){closecallback=cb;}

	virtual void onclose(){if(closecallback)closecallback(this);}
	virtual void ondata(char*key,char*value){if(datacallback)datacallback(this,key,value);}
	bool send(const char*key,const char*val,int vallen=-1){
		pthread_mutex_lock(&mutex);
		int pos=0;
		int keylen=strlen(key);
		if(vallen<0)vallen=strlen(val);
		sendbuf[pos++]=keylen;
		for(int i=0;i<keylen;i++)sendbuf[pos++]=key[i];
		sendbuf[pos++]=vallen>>8;
		sendbuf[pos++]=vallen&0xff;
		for(int i=0;i<vallen;i++)sendbuf[pos++]=val[i];
		bool retval=bwrite(sendbuf,pos);
		pthread_mutex_unlock(&mutex);
		return retval;
	}
	void onreceive(const char*data,int len){
		for(int i=0;i<len;i++){
			char c=data[i];
			if(c=='\r')continue;
			if(c=='\n'){
				current[currentlen]='\0';
				if(current==key)current=val;
				else{current=key;
					ondata(key,val);
				}
				currentlen=0;
			}
			else current[currentlen++]=c;
		}
	}
};

void*sockreadthreadfunc(void*arg){
	Socket*socket=(Socket*)arg;
	int len,fd=socket->fd;
	char buf[8192];
	while((len=read(fd,buf,sizeof(buf)))>0)socket->onreceive(buf,len);
	socket->closed();
	return NULL;
}

