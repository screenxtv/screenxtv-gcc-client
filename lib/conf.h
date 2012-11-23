#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define errorexit(...) (fprintf(stderr,__VA_ARGS__),exit(-1))

struct KeyValue{const char*key,*value;};

int strcpylen(char*dst,const char*src){strcpy(dst,src);return strlen(dst);}
char*strclone(const char*s){char*s2=new char[strlen(s)+1];strcpy(s2,s);return s2;}
int strpos(const char*str,char c){return strchr(str,c)-str;}
char*trim(char*str){
	#define target(c) (c==' '||c=='\t'||c=='\r'||c=='\n')
	while(target(*str))str++;
	for(char*end=str+strlen(str)-1;end!=str&&target(*end);end--)*end='\0';
	#undef target
	return str;
}
char***confMake(){
	char***list=new char**[65536];
	list[0]=NULL;
	return list;
}

char*confGet(char***list,const char*key){
	for(char**kv;kv=*list;list++){
		if(strcmp(kv[0],key)==0)return kv[1];
	}
	return NULL;
}

void confPut(char***list,const char*key,const char*value){
	for(char**kv;kv=*list;list++){
		if(strcmp(kv[0],key)==0){
			delete[]kv[1];
			kv[1]=strclone(value);
			return;
		}
	}
	char**kv=*list=new char*[2];
	kv[0]=strclone(key);
	kv[1]=strclone(value);
	list[1]=NULL;
}


void confLoad(char***list,KeyValue*defaultsettings){
	for(int i=0;defaultsettings[i].key;i++){
			KeyValue kv=defaultsettings[i];
			confPut(list,strclone(kv.key),strclone(kv.value));
	}
}
bool confLoad(char***list,const char*file){
	FILE*fp=fopen(file,"r");
	if(!fp)return false;
	char buf[65536];
	char key[65536];
	int keylen=0;
	char value[65536]={0};
	int valuelen=0;
	#define iswhitespace(c) ((c)==' '||(c)=='\t'||(c)=='\r'||(c)=='\n')
	#define addvalue(s) (valuelen+=strcpylen(value+valuelen,(s)),value[valuelen++]='\n',value[valuelen]='\0')
	#define addlist() (confPut(list,key,trim(value)),valuelen=0)
	int line=0;
	while(fgets(buf,sizeof(buf),fp)){
		line++;
		if(buf[0])
		if(iswhitespace(*buf)){
			addvalue(trim(buf));
		}else{
			if(keylen)addlist();
			int colon=strpos(buf,':');
			if(colon<0)errorexit("parse error colon not found (%s):%d\n",file,line);
			buf[colon]='\0';
			keylen=strcpylen(key,trim(buf));
			addvalue(trim(buf+colon+1));
		}
	}
	if(keylen)addlist();
	#undef isalphabet
	#undef iswhitespace
	#undef addvalue
	#undef addlist
	fclose(fp);
	return true;
}

void confSave(char***list,const char*file){
	char data[65536];
	int datalen=0;
	#define pushC(c) (data[datalen++]=(c))
	#define pushS(s) (datalen+=strcpylen(data+datalen,s))
	for(char**kv;kv=*list;list++){
		char*key=kv[0],*value=kv[1];
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
	FILE*fp=fopen(file,"w");
	fwrite(data,1,datalen,fp);
	fclose(fp);
	#undef pushC
	#undef pushS
}


void confDelete(char***list){
	for(int i=0;list[i];i++){
		delete[]list[i][0];
		delete[]list[i][1];
	}
	delete[]list;
}


class Config{
public:
	char*filename;
	char***data;
	Config(const char*fname,KeyValue*defconf){
		filename=strclone(fname);
		data=confMake();
		confLoad(data,defconf);
	}
	bool load(){return confLoad(data,filename);}
	void save(){confSave(data,filename);}
	char*get(const char*key){return confGet(data,key);}
	void put(const char*key,const char*val){confPut(data,key,val);}
};
