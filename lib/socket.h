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
#include <string.h>
#include "macros.h"

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

class Socket{
public:
  int fd;
  unsigned char readbuf[LARGE_BUF_SIZE];
  int readbuf_len,readbuf_position;
  Socket(const char*host,int port){
    readbuf_len=readbuf_position=0;
    fd=socketconnect(host,port);
    if(fd<0)errorexit("connect failed\n");
  }
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
  void fdclose(){close(fd);fd=-1;}
  int getchar(){
    if(fd<0)return -1;
    if(readbuf_position<readbuf_len){
      return readbuf[readbuf_position++];
    }
    int len=read(fd,readbuf,sizeof(readbuf));
    if(len<0){fdclose();return -1;}
    readbuf_len=len;readbuf_position=0;
    return readbuf[readbuf_position++];
  }
};
class KVSocket:public Socket{
public:
  pthread_mutex_t mutex;
  char sendbuf[2*LARGE_BUF_SIZE];
  char key[LARGE_BUF_SIZE],val[LARGE_BUF_SIZE],*current;
  int currentlen;
  KVSocket(const char*host,int port):Socket(host,port){
    current=key;currentlen=0;
    pthread_mutex_init(&mutex,NULL);
  }
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
  int readline(char*buf,int max){
    for(int len=0;len<max;){
      int c=getchar();
      if(c<0||c=='\n'){buf[len]=0;return len;}
      if(c!='\r')buf[len++]=c;
    }
    return max;
  }
  bool read(char*key,char*value,int keymax,int valmax){
    int klen=readline(key,keymax);
    int vlen=readline(value,valmax);
    if(klen<0||vlen<0)return false;
    return true;
  }
};


