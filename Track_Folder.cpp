#include <iostream>
#include <cstring>
#include <fstream>
#include <dirent.h>
#include <stdio.h>
#include <ctime>
#include <iomanip>
#include "Track_Defs.h"
#include "Date_Time_Tools.h"
using namespace std;
#define VERSION "1.2"
#define AUTHOR "KaloyanTs"
#define ProjectNAME "Track_Folder"
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
    //                for(int i=0;i<lvl;i++)cout<<"  ";cout<<lvl+1<<"->";
    //                cout<<path<<file<<endl;
    ifstream f;
    strcpy(s,path);strcat(s,file);
    f.open(s);
    if(!f.is_open()){/*cout<<file<<" does not exist!"<<endl;*/return;}
    while(!f.eof()){
        readLine(f,s);
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

int main(int argc, char** argv){
    initTypes();
    char path[300],s[200],t[300];
    char date[24],time[24];
    const char* fileName="Path.txt";
    int c=0;
    ifstream p;p.open(fileName);
    readLine(p,t);
    p.close();
    for(int i=0;i<strlen(t);i++){
        if(t[i]=='\\')path[c++]='\\';
        path[c++]=t[i];
    }
    path[c++]=0;
    strcpy(s,path);
    strcat(s,"\\data.txt");
    ofstream data(s);
    data<<"// "<<ProjectNAME<<".exe V"<<VERSION<<" by "<<AUTHOR<<endl;
    makeDate(date);
    makeTime(time);
    data<<"// "<<date<<' '<<time<<endl;
    data <<"// "<<t;
    struct dirent *d;
    DIR *dr;
    dr = opendir(path);
    if(dr!=NULL)
    {
        for(d=readdir(dr); d!=NULL; d=readdir(dr))
        {
           if(d->d_name[d->d_namlen-1]!='c'||d->d_name[d->d_namlen-2]!='.')continue;
            Search_Files(path,d->d_name,0);
            data<<d->d_name<<endl;
            data<<"// Level; Name; Type; Address offset; Length"<<endl;
            Show_Res(data);
            data.close();
            break;
        }
        closedir(dr);
    }
        remove(fileName);
        return 0;
}
