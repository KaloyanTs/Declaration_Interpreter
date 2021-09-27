#include <iostream>
#include <ctime>
#include <cstdlib>
#include <winnls.h>
using namespace std;

    time_t timet = time(0);
    struct tm * now = localtime( & timet );
    const char Months[12][10]={"January","February","March","April",
                        "May","June","July","August","September",
                        "October","November","December"};

char* getDateFormat(char *df){
    int ret;
    char p[16];
    ret = GetLocaleInfo(    LOCALE_USER_DEFAULT,  LOCALE_SSHORTDATE,  (LPTSTR)&p,   0   );
    GetLocaleInfo(LOCALE_USER_DEFAULT,   LOCALE_SSHORTDATE, (LPTSTR)&p, ret  );
    strcpy(df,p);
    return df;
}

char* getTimeFormat(char *df){
    int ret;
    char p[32];
    ret = GetLocaleInfo(    LOCALE_USER_DEFAULT,  LOCALE_STIMEFORMAT,  (LPTSTR)&p,   0   );
    GetLocaleInfo(LOCALE_USER_DEFAULT,   LOCALE_STIMEFORMAT, (LPTSTR)&p, ret  );
    strcpy(df,p);
    return df;
}

char *extractDay(char *df,char *d){
    *d=0;
    char *a=strstr(df,"d");
    if (a==NULL)return d;
    if(*(a+1)=='d'){
        d[0]=now->tm_mday/10+'0';
        d[1]=now->tm_mday%10+'0';
        d[2]=0;
        return d;
    }
    itoa(now->tm_mday,d,10);
    return d;
}

char *extractSec(char *df,char *s){
    *s=0;
    char *a=strstr(df,"s");
    if (a==NULL){a=strstr(df,"S");}
    if (a==NULL)return s;
    if(*(a+1)=='s'){
        s[0]=now->tm_sec/10+'0';
        s[1]=now->tm_sec%10+'0';
        s[2]=0;
        return s;
    }
    itoa(now->tm_sec,s,10);
    return s;
}

char *extractMin(char *df,char *m){
    *m=0;
    char *a=strstr(df,"m");
    if (a==NULL)return m;
    if(*(a+1)=='m'){
        m[0]=now->tm_min/10+'0';
        m[1]=now->tm_min%10+'0';
        m[2]=0;
        return m;
    }
    itoa(now->tm_min,m,10);
    return m;
}

char *extractHour(char *df,char *h){
    *h=0;
    char *a=strstr(df,"h");
    if (a==NULL){a=strstr(df,"H");}
    if (a==NULL)return h;
    int hh=now->tm_hour;
    if(strstr(df,"tt")!=NULL&&hh!=12)hh%=12;
    if (a==NULL)return h;
    if(*(a+1)=='h'||*(a+1)=='H'){
        h[0]=hh/10+'0';
        h[1]=hh%10+'0';
        h[2]=0;
        return h;
    }
    itoa(hh,h,10);
    return h;
}

char *extractMonth(char *df,char *m){
    *m=0;
    int mc;
    char *a=strstr(df,"M");
    if (a==NULL)return m;
    mc=1;
    while(*(++a)=='M'){mc++;}
    switch(mc){
    case 1:
        itoa(now->tm_mon+1,m,10);
        return m;
    case 2:
        m[0]=(now->tm_mon+1)/10+'0';
        m[1]=(now->tm_mon+1)%10+'0';
        m[2]=0;
        return m;
    default:
        strcpy(m,Months[now->tm_mon]);
        m[mc]=0;
        return m;
    }
}

char *extractYear(char *df,char *y){
    *y=0;
    int yc;
    char *a=strstr(df,"y");
    if (a==NULL)return y;
    yc=1;
    while(*(++a)=='y'){yc++;}
    switch(yc){
    case 2:
        itoa(now->tm_year%100,y,10);
        return y;
    case 3:
        itoa((900+now->tm_year)%1000,y,10);
        return y;
    default:
        itoa(1900+now->tm_year,y,10);
        return y;
    }
}

char* makeDate(char* curD){
    char df[16];
    int dC=0;
    getDateFormat(df);
    *curD=0;
    for(int i=0;i<strlen(df);i++){
        switch(df[i]){
        case 'd':
            char d[8];
            curD[dC]=0;
            strcat(curD,extractDay(df,d));
            dC+=strlen(d);
            while(df[i+1]=='d'){i++;};
            break;
        case 'M':
            char m[8];
            curD[dC]=0;
            strcat(curD,extractMonth(df,m));
            dC+=strlen(m);
            while(df[i+1]=='M'){i++;};
            break;
        case 'y':
            char y[8];
            curD[dC]=0;
            strcat(curD,extractYear(df,y));
            dC+=strlen(y);
            while(df[i+1]=='y'){i++;};
            break;
        default:
            curD[dC++]=df[i];
        }
    }
    curD[dC++]=0;
    return curD;
}

char* makeTime(char* curT){
    char df[16];
    int dC=0;
    getTimeFormat(df);
    *curT=0;
    for(int i=0;i<strlen(df);i++){
        switch(df[i]){
        case 's':
        case 'S':
            char s[8];
            curT[dC]=0;
            strcat(curT,extractSec(df,s));
            dC+=strlen(s);
            while(df[i+1]=='s'||df[i+1]=='S'){i++;};
            break;
        case 'm':
            char m[8];
            curT[dC]=0;
            strcat(curT,extractMin(df,m));
            dC+=strlen(m);
            while(df[i+1]=='m'){i++;};
            break;
        case 'h':
        case 'H':
            char h[8];
            curT[dC]=0;
            strcat(curT,extractHour(df,h));
            dC+=strlen(h);
            while(df[i+1]=='h'||df[i+1]=='H'){i++;};
            break;
        case 't':
            curT[dC]=0;
            if(now->tm_hour>11)strcat(curT,"PM");
            else strcat(curT,"AM");
            dC+=2;
            while(df[i+1]=='t'){i++;};
            break;
        default:
            curT[dC++]=df[i];
        }
    }
    curT[dC++]=0;
    return curT;
}
