using namespace std;

int arr_mem, acc_bits;
struct Type;
struct Var;
struct Type{
    char name[64];
    int size=0;
    Var *t=NULL;
    short bitPart=0;
    int ct=0;
};
struct Var{
    char name[50];
    Type tp;
};
void show(ofstream &of, const Var v, const int lvl, int &mem, int &bit_part);
void show(ofstream &of, const Type t, const int lvl, int &mem, int &bit_part){
    int temp=mem;
    if(strstr(t.name,"union")!=NULL){
        for(int i=0;i<t.ct;i++){
                    if(strstr(t.t[i-1].tp.name,"bit")!=NULL){
                        bit_part+=t.t[i-1].tp.bitPart;
                        mem+=bit_part/8;
                        bit_part%=8;
                    }
                show(of,t.t[i],lvl,mem,bit_part);}
        return;
    }
    for(int i=0;i<t.ct;i++){
            if(!i)show(of,t.t[i],lvl,mem,bit_part);
            else {
                mem+=t.t[i-1].tp.size;
                    if(strstr(t.t[i-1].tp.name,"bit")!=NULL){
                        bit_part+=t.t[i-1].tp.bitPart;
                        mem+=bit_part/8;
                        bit_part%=8;
                        if(strstr(t.t[i].tp.name,"bit")==NULL&&bit_part%8)mem++;
                    }
                show(of,t.t[i],lvl,mem,bit_part);}
    }
    mem=temp;
}
void show(ofstream &of, const Var v, const int lvl,int &mem, int &bit_part){
    for(int i=0;i<lvl;i++){of<<"  ";}
    if(strstr(v.name,"[0]")!=NULL)mem=arr_mem;
    if(strstr(v.tp.name,"bit")==NULL&&bit_part)bit_part=0;
    of<<lvl+1<<';'<<v.name<<';'<<v.tp.name<<';'<<mem;
    if(bit_part||!strcmp(v.tp.name,"bit"))of<<'.'<<bit_part;
    of<<';';
    if(strchr(v.tp.name,'*'))
    of<<v.tp.size;
    else if(!strstr(v.tp.name,"bit") && !v.tp.t)of<<v.tp.size;
    else of<<'-';
    of<<endl;
    if(strstr(v.tp.name,"[")!=NULL)arr_mem=mem;
    show(of,v.tp,lvl+1,mem,bit_part);
}

 int types_Count=23;
 Type T[1000];
void initTypes(){
    strcpy(T[0].name,"bit");T[0].size=0;T[0].t=NULL;
    strcpy(T[1].name,"enum");T[1].size=1;T[1].t=NULL;
    strcpy(T[2].name,"void");T[2].size=0;T[2].t=NULL;
    strcpy(T[3].name,"unsigned short int");T[3].size=1;T[3].t=NULL;
    strcpy(T[4].name,"signed short int");T[4].size=1;T[4].t=NULL;
    strcpy(T[5].name,"unsigned long int");T[5].size=4;T[5].t=NULL;
    strcpy(T[6].name,"unsigned long");T[6].size=4;T[6].t=NULL;
    strcpy(T[7].name,"signed long int");T[7].size=4;T[7].t=NULL;
    strcpy(T[8].name,"signed long");T[8].size=4;T[8].t=NULL;
    strcpy(T[9].name,"long double");T[9].size=4;T[9].t=NULL;
    strcpy(T[10].name,"double");T[10].size=4;T[10].t=NULL;
    strcpy(T[11].name,"long int");T[11].size=4;T[11].t=NULL;
    strcpy(T[12].name,"long");T[12].size=4;T[12].t=NULL;
    strcpy(T[13].name,"unsigned short");T[13].size=1;T[13].t=NULL;
    strcpy(T[14].name,"unsigned char");T[14].size=1;T[14].t=NULL;
    strcpy(T[15].name,"signed char");T[15].size=1;T[15].t=NULL;
    strcpy(T[16].name,"char");T[16].size=1;T[16].t=NULL;
    strcpy(T[17].name,"unsigned int");T[17].size=2;T[17].t=NULL;
    strcpy(T[18].name,"signed int");T[18].size=2;T[18].t=NULL;
    strcpy(T[19].name,"unsigned");T[19].size=2;T[19].t=NULL;
    strcpy(T[20].name,"short");T[20].size=1;T[20].t=NULL;
    strcpy(T[21].name,"float");T[21].size=4;T[21].t=NULL;
    strcpy(T[22].name,"int");T[22].size=2;T[22].t=NULL;
}
Var V[1000];int N=0;

void MultipleComment(ifstream &f, char *s){
    if(strstr(s,"/*")!=NULL){while(strstr(s,"*/")==NULL)f.getline(s,300);f.getline(s,300);}
}

void Define_def(ifstream &f,char *s){
    while(strstr(s,"\\")!=NULL&&(*s!='\0'&&(s[0]!='/'||s[1]!='/'))){
        f.getline(s,300);
    }
    f.getline(s,300);
    if(strstr(s,"#")!=NULL){Define_def(f,s);return;}
    MultipleComment(f,s);
}

void Enum_def(ifstream &f,char *s,Var *v,int &n){
    char name[32];
    char * newt=strstr(s,"typedef");
    int c=0;
    bool mult=false;
    while(strstr(s,";")==NULL){
        f.getline(s,300);
    }
    char *a=strstr(s,"}")+1;
    while(*a==' '){a++;}
    while(*a!=';'&&*a!=' '){
            if(*a==','){
                *a=' ';
                mult=true;
                break;
            }
            name[c++]=*(a++);
            *(a-1)=' ';
    }
    if(!c)return;
    name[c++]=0;
    if(newt!=NULL){
        Type t;
        strcpy(t.name,name);
        t.size=1;
        T[types_Count++]=t;
    }
    else{
        Var vv;
        strcpy(vv.name,name);
        vv.tp=T[1];
        v[n++]=vv;

    }
    if(mult)Enum_def(f,s,v,n);
}
int Simple_Def(ifstream &f, char *s,Var *v,int &n);

void Skip_Functions(ifstream &f,char *s){
    if(strstr(s,"absolute")!=NULL){Simple_Def(f,s,V,N);return;}
    if(strstr(s,"[")!=NULL&&(strstr(s,"(")-strstr(s,"[")>0)){Simple_Def(f,s,V,N);return;}
    if(s[strlen(s)-1]==';'&&s[strlen(s)-2]==')')return;
    int state=0,l=strlen(s);
    bool flag=false;
    char *a,*b;
        a=strstr(s,"{");b=strstr(s,"}");
        if(a!=NULL){state++;flag=true;
            while(a!=&s[l-1]){
                a=strstr(a+1,"{");
                if(a==NULL||(strstr(s,"//")!=NULL?strstr(s,"//")-a<0:0))break;
                state++;flag=true;}}
        if(b!=NULL){state--;
            while(b!=&s[l-1]){
                b=strstr(b+1,"}");
                if(b==NULL||(strstr(s,"//")!=NULL?strstr(s,"//")-a<0:0))break;
                state--;}}
    while(state||!flag){
        do{
            f.getline(s,300);
        }while(*s=='\0');
        a=strstr(s,"{");
        b=strstr(s,"}");
        if(a!=NULL){state++;flag=true;
            while(a!=&s[l-1]){
                a=strstr(a+1,"{");
                if(a==NULL||(strstr(s,"//")!=NULL?strstr(s,"//")-a<0:0))break;
                state++;flag=true;}}
        if(b!=NULL){state--;
            while(b!=&s[l-1]){
                b=strstr(b+1,"}");
                if(b==NULL||(strstr(s,"//")!=NULL?strstr(s,"//")-a<0:0))break;
                state--;}}
    }
}

void Array_Def(Type t, char *name, Var *v, int arr_size);


int Simple_Def(ifstream &f, char *s,Var *v,int &n){
    Var vv;
    int c=0;
    char *a,*b;
    char ar_n[8];
    a=strstr(s," bit ");
    if(a!=NULL){
        strcpy(vv.tp.name,"bit");
        vv.tp.bitPart=1;
        a+=5;
    }
    else{
        for(int i=0;i<types_Count;i++){
            int l=strlen(T[i].name);
            T[i].name[l++]=' ';T[i].name[l--]=0;
            a=strstr(s,T[i].name);
            T[i].name[l]=0;
            if (a!=NULL){vv.tp=T[i];a+=strlen(T[i].name);break;}
            if(i==types_Count-1)return -1;
        }
        if(strstr(s,"*")!=NULL&&(strstr(s,"//")!=NULL?strstr(s,"//")-strstr(s,"*")>0:1)){
                strcat(vv.tp.name,"*");
                vv.tp.size=(strstr(s,"const")!=NULL?3:2);
                vv.tp.ct=0;
                vv.tp.t=NULL;
        }
    }
    while(*a==' '||*a=='*'){a++;}
    for(char *p=a;*p!=';'&&*p!=' '&&*p!=',';p++)vv.name[c++]=*p;
    vv.name[c]=0;
    a=strstr(s,"[");
    if(a!=NULL&&(strstr(s,"//")!=NULL?strstr(s,"//")-a>0:1)){
        int C=0;
        do{
           C*=10;
           C+=*(++a)-'0';
           if(*a<'0'||*a>'9'){C=2;break;}
        }while(*(a+1)!=']');
            itoa(C,ar_n,10);
            a=strstr(vv.name,"[");
            *a=0;

            Type temp=vv.tp;
            vv.tp.t=new Var[C];
            Array_Def(temp,vv.name,vv.tp.t,C);
            vv.tp.ct=C;
            strcat(vv.tp.name,"[");
            strcat(vv.tp.name,ar_n);
            strcat(vv.tp.name,"]");
            vv.tp.size*=C;
            v[n++]=vv;
    }else{
         v[n++]=vv;
    }
multiple:
    a=strstr(s,",");
    if(a==NULL||(strstr(s,"//")!=NULL?strstr(s,"//")-strstr(s,",")<0:0))return vv.tp.size;
    char nn[300];
    char *y=&s[strlen(s)-1];
    while(*y==' ')*(y--)=0;
    while(s[strlen(s)-1]==','){f.getline(nn,300);strcat(s,nn);}
    char next_dec[300];
    b=strstr(vv.tp.name,"[");
    if(b!=NULL)*b=0;
    strcpy(next_dec,vv.tp.name);
    strcat(next_dec,++a);
    return vv.tp.size+Simple_Def(f,next_dec,v,n);
}

void Array_Def(Type t, char *name, Var *v, int arr_size){
    Var temp;
    char *a;
    char b[8];
    temp.tp=t;
    strcpy(temp.name,name);
    strcat(temp.name,"[");
    a=strstr(temp.name,"[")+1;
    for(int i=0;i<arr_size;i++){
        *a=0;
        itoa(i,b,10);
        strcat(temp.name,b);
        strcat(temp.name,"]");
        v[i]=temp;
    }
}

int Bit_Field(ifstream &f, char *s,Var *v, int &n){
    char *a=strstr(s,":");
    int C=0,cc=0;
    while(a!=NULL){
            a++;
            while(*a<'0'||*a>'9'){a++;}
            C=0;
            do{
                C*=10;
                C+=*a-'0';
            }while(*(++a)>='0'&&*a<='9');
            strcpy(v[n].tp.name,"bit");

            char *b=strstr(s,":");
            do{b--;}while(*b==' ');
            *(++b)=0;
            a=b;
            do{a--;}while(*a!=' ');a++;
            strcpy(v[n].name,a);
            *b=' ';
            cc+=C;
            v[n].tp.bitPart=C%8;
            v[n++].tp.size=C/8;
            do{
                f.getline(s,300);
                a=strstr(s,":");
            }while(*s=='\0'||(strstr(s,"//")!=NULL?(strstr(s,"//")-a)<0:0));
    }
    return cc;
}

Type Struct_OneLine(ifstream &f, char *s){
    Type temp;
    char r;
    temp.t=new Var[16];
    strcpy(temp.name,"struct");
    char *a=strstr(s,"{")+1;
    char *b=strstr(a,";");

    while(b<strstr(s,"}")){
        r=*(++b);
        *b=0;
        temp.size+=Simple_Def(f,a,temp.t,temp.ct);
        *b=r;
        a=b;
        b=strstr(a,";");
    }
    return temp;
}

void Struct_Def(ifstream &f,char *s,Var *v,int &n, bool isUnion){
    int c=0,cc=0;
    Var vv;Type temp;
    char ar_n[8];
    if(!isUnion)strcpy(vv.tp.name,"struct");
    else strcpy(vv.tp.name,"union");
    vv.tp.t=new Var[128];
    if(strstr(s,"}")!=NULL&&
       (strstr(s,"//")!=NULL?strstr(s,"//")-strstr(s,"}")>0:1))
       vv.tp=Struct_OneLine(f,s);
    else{
        f.getline(s,300);
        while(strstr(s,"}")==NULL||strstr(s,"enum")!=NULL){
            if(s[0]!='/'||s[1]!='/'&&*s!='\0'){
                if(strstr(s,":")!=NULL&&
                   (strstr(s,"//")!=NULL?strstr(s,"//")-strstr(s,":")>0:1)
                  ){
                      cc=Bit_Field(f,s,vv.tp.t,vv.tp.ct);
                      vv.tp.size+=(cc%8?cc/8+1:cc/8);
                   }
                while(strstr(s,"enum")!=NULL){Enum_def(f,s,vv.tp.t,vv.tp.ct);f.getline(s,300);}
                if(strstr(s,"union")!=NULL)Struct_Def(f,s,vv.tp.t,vv.tp.ct,true);
                int check=Simple_Def(f,s,vv.tp.t,vv.tp.ct);
                if(check==-1)check=0;
                if(!isUnion)vv.tp.size+=check;
                else vv.tp.size=check;
            }
            if(strstr(s,"}")==NULL)f.getline(s,300);
        }
    }
    temp=vv.tp;
    char *a=strstr(s,"}")+1;
    do{
        while(*a==' '){a++;}
        c=0;
        do{
            vv.name[c++]=*(a++);
        }while(*a!=';'&&*a!=' '&&*a!=',');
        vv.name[c]=0;
    char *aa=strstr(s,"[");
    if(aa!=NULL){
        int C=0;
        do{
           C*=10;
           C+=*(++aa)-'0';
        }while(*(aa+1)!=']');
                itoa(C,ar_n,10);
                aa=strstr(vv.name,"[");
                *aa=0;
                vv.tp.t=new Var[C];
                Array_Def(temp,vv.name,vv.tp.t,C);
                vv.tp.ct=C;
                strcpy(vv.tp.name,"struct");
                strcat(vv.tp.name,"[");
                strcat(vv.tp.name,ar_n);
                strcat(vv.tp.name,"]");
                vv.tp.size*=C;
                v[n++]=vv;

        }else{
            v[n++]=vv;
        }
    }while(*(a++)==',');
}

void New_Type(ifstream &f,char *s){
    Type temp;
    temp.t=new Var[64];
    int c=0,sc=0,cc=0;
    if(strstr(s,"}")!=NULL&&
       (strstr(s,"//")!=NULL?strstr(s,"//")-strstr(s,"}")>0:1))
       temp=Struct_OneLine(f,s);
    else{
        do{
            f.getline(s,300);
            if(s[0]=='/'&&s[1]=='/'||*s=='\0'){continue;}
            if(strstr(s,"/*")!=NULL){while(strstr(s,"*/")==NULL)f.getline(s,300);f.getline(s,300);}
            if(strstr(s,":")!=NULL&&
               (strstr(s,"//")!=NULL?strstr(s,"//")-strstr(s,":")>0:1)
              ){
                      cc=Bit_Field(f,s,temp.t,temp.ct);
                      temp.size+=(cc%8?cc/8+1:cc/8);
               }
            if(strstr(s,"enum")!=NULL)
                {   int ll=temp.ct;
                    Enum_def(f,s,temp.t,temp.ct);
                    f.getline(s,300);
                    for(int i=ll;ll<temp.ct;ll++)temp.size+=temp.t[i].tp.size;
                }
            if(strstr(s,"union")!=NULL)Struct_Def(f,s,temp.t,temp.ct,true);
            if(strstr(s,",")!=NULL)
                {int check=Simple_Def(f,s,temp.t,temp.ct);
                 if(check==-1)check=0;
                 temp.size+=check;}
            else{
                if(Simple_Def(f,s,temp.t,temp.ct)==-1)continue;
                    temp.size+=temp.t[temp.ct-1].tp.size;
            }

        }while(strstr(s,"}")==NULL);
    }
    char *a=strstr(s,"}")+1;
    do{
        temp.name[sc++]=*(a++);
    }while(*a!=';'&&*a!=' ');
    temp.name[sc]=0;
    T[types_Count++]=temp;
}

void Proceed_Line(ifstream &f,char *s){
            if(s[0]=='/'&&s[1]=='/')return;
            MultipleComment(f,s);
            if(strstr(s,"#")!=NULL){Define_def(f,s);}
            if(strstr(s,"enum")!=NULL){Enum_def(f,s,V,N);return;}
            if(strstr(s,"const")!=NULL){
                if(strstr(s,";")!=NULL)return;
                do{f.getline(s,300);}
                while(strstr(s,"}")==NULL||strstr(s,";")==NULL);
            }
            if(strstr(s,"(")!=NULL){
                    if(!(strstr(s,"//")!=NULL&&((strstr(s,"(")-strstr(s,"//"))>0)))
                    {Skip_Functions(f,s);return;}
            }
            if(strstr(s,"typedef")!=NULL){New_Type(f,s);return;}
            if(strstr(s,"struct")!=NULL){Struct_Def(f,s,V,N,false);return;}
            if(strstr(s,"union")!=NULL){Struct_Def(f,s,V,N,true);return;}
            if(Simple_Def(f,s,V,N)!=-1)return;

}

void Show_Res(ofstream &of){
    int m=0,bp=0;
    for(int i=0;i<N;i++){
        m=0;bp=0;
        show(of,V[i],0,m,bp);
    }
}

void deallocVar(Var& v)
{

    if(v.tp.t)delete[] v.tp.t;
}

void deaalocMemory()
{
    for(unsigned i=0;i<N;++i)
        deallocVar(V[i]);
}
