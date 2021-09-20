#include <iostream>
#include <cstring>
#include <fstream>
#include <dirent.h>
#include <stdio.h>
#include "Track_Defs.h"
using namespace std;
int C=0,fileC=0;
char files[32][32];

bool isSearched(char *file){
    for(int i=0;i<fileC;i++){
        if(!strcmp(file,files[i]))return true;
    }
    strcpy(files[fileC++],file);
    return false;
}

void Search_Files(char *path, char *file, int lvl){
    if(isSearched(file))return;
    char s[300];
    char c[4];
    //for(int i=0;i<lvl;i++)cout<<"  ";cout<<lvl+1<<"->";
    //cout<<path<<file<<endl;
    ifstream f;
    strcpy(s,path);strcat(s,file);
    f.open(s);
    if(!f.is_open()){/*cout<<file<<" does not exist!"<<endl;*/return;}
    while(!f.eof()){
        f.getline(s,300);
        if(strstr(s,"#include")!=NULL){
            char *a=strstr(s,"\"")+1;
            char *b=strstr(a,"\"");
            *b=0;
            char temp[300];
            strcpy(temp,a);
            Search_Files(path,temp,lvl+1);
            *b='\"';
        }
        else
            if(*s&&strstr(file,".c")==NULL)Proceed_Line(f,s);
    }
    f.close();
}

int main(){
    char path[300],s[200],t[300];
    const char* fileName="Path.txt";
    int c=0;
    ifstream p;p.open(fileName);
    p.getline(t,300);
    p.close();
    for(int i=0;i<strlen(t);i++){
        if(t[i]=='\\')path[c++]='\\';
        path[c++]=t[i];
    }
    path[c++]=0;
    strcpy(s,path);
    strcat(s,"\\data.txt");
    ofstream data(s);
    struct dirent *d;
    DIR *dr;
    dr = opendir(path);
    if(dr!=NULL)
    {
        for(d=readdir(dr); d!=NULL; d=readdir(dr))
        {
           if(d->d_name[d->d_namlen-1]!='c'||d->d_name[d->d_namlen-2]!='.')continue;
            strcat(path,"\\\\");
            Search_Files(path,d->d_name,0);
            Show_Res(data);
            data.close();
            break;
        }
        closedir(dr);
    }
        remove(fileName);
        return 0;
}
