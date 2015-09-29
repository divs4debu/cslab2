#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <cmath>

using namespace std;

typedef struct set set;

struct set {
    char name[100];
    int binary;
    set* next;
    set* down;
    int length;

}*head ,*cur,*nul,*uni;



void chkSet(set*);
int  getLength(string);
void chkSubElements(set*);
void findUnivarsalSet();
void chkDuplicate(set*);
set* search(string);
set* getIndexElement(set*,int);
void setLength();
int  getBinary (string x,string y);
void setBinary();
int  un (string,string);
int  sub (string,string);
int  inter(string,string);
void printInputSets();
string printElements (int,set*);
string powerset(set*);
string powPrint(set*,int);






int main() {

    
    ifstream inp;
    inp.open("mesh.txt");
    string line;
    char name[100];
    memset(name,0,sizeof(name));
    head=(set*)malloc(sizeof(set));
    cur=head;
    set *temp=head;

    while(getline(inp,line)) {
        

        int j=0;
        for(int i=line.find(' ')+1; i<line.find("=")-1; i++,j++) {
            name[j]=line[i];
        }
        

        strcpy(cur->name,name);
        memset(name,0,sizeof(name));
        j=0;
        int k=0;
        

        for(int i=line.find('{')+1; i<line.find('}'); i++) {

            
            if(line[i]!=' ') {
                name[k]=line[i];
                k++;
            } else if(name[0]!='\0'){
                set *empty=(set*)malloc(sizeof(set));
                cur->next=empty;
                cur=cur->next;
                strcpy(cur->name,name);
                k=0;
                memset(name,0,sizeof(name));
            }
        }
        

        cur=temp;
        set *empty=(set*)malloc(sizeof(set));
        cur->down=empty;
        temp=temp->down;
        cur=cur->down;


    }


    set* fr=head;
    while(fr) {
        chkSet(fr);
        fr=fr->down;
    }
    setLength();
    findUnivarsalSet();
    fr=head;
     while(fr) {
        chkSubElements(fr);
        fr=fr->down;
    }
    setBinary();
   
    

    cout<<"\nSET PRESENT IN THE FILE\n"<<"______________________________________________________________________"<<endl;;
    printInputSets();
    cout<<"\n"<<uni->name<<"  it is found to be universal set in the input file"<<endl;
    cout<<"______________________________________________________________________"<<endl;

    ofstream out;
    out.open("output.txt");
    bool flag=true;
    string input,res,se1,se2;
    char x='\0';
    memset(name,0,sizeof(name));
    cout<<"\nENTER INSTRUCTIONS\nUSE :\nx(U)y FOR UNION OF x AND y\nx(-)y FOR SUBSTRACTION OF SET y FROM SET x\nx(i)y FOR INTERSECTION OF SET x AND SET y\nx(p) FOR POWERSET OF SET x\n"<<endl;
    while(flag) {
        int j=0;


        cin>>input;
    
        input[input.length()]='\0';
        if(input=="q") {
            flag=false;
            break;
        }
        if(input.find("(U)")==-1&&input.find("(u)")==-1&&input.find("(p)")==-1&&input.find("(P)")==-1&&input.find("(i)")==-1&&input.find("(I)")==-1&&input.find("(-)")==-1){
            cout<<"INVALID MOVE"<<endl;
            continue;
        }


        for(int i=0; i<input.find("("); i++,j++) {
            name[j]=input[i];
            name[j+1]='\0';
        }
        se1=name;
        memset(name,0,sizeof(name));
        x=input[input.find('(')+1];

        j=0;
        for(int i=input.find(")")+1; input[i]!='\0'; i++,j++)
            name[j]=input[i];
        
        se2=name;
 
        if((search(se1)&&search(se2))||((x=='p'||((x=='P')||x=='p')&&search(se1)))) {

            

            if(x=='U'||x=='u') {

                res=res+se1+"U"+se2+"   "+"{"+printElements(un(se1,se2),uni)+"}"+"\n";
                cout<<"{"<<printElements(un(se1,se2),uni)<<"}"<<endl;

            } else if(x=='-') {

                res=res+se1+"-"+se2+"   "+"{"+printElements(sub(se1,se2),uni)+"}"+"\n";

                cout<<"{"<<printElements(sub(se1,se2),uni)<<"}"<<endl;
            } else if(x=='i'||x=='I') {

                res=res+se1+"∩"+se2+"   "+"{"+printElements(inter(se1,se2),uni)+"}"+"\n";
                cout<<"{"<<printElements(inter(se1,se2),uni)<<"}"<<endl;
            }
            else if(x=='p'||x=='P'){
                cout<<search(se1)->name;
                cout<<powerset(search(se1))<<endl;
                res=res+se1+"powerset ";
                res+=powerset(search(se1));
            } else
                cout<<'"'<<x<<'"'<<" IS AN INVALID INSTRUCTION PLEASE TRY AGAIN "<<endl;


        } else
            cout<<"SET CANNOT BE FOUND "<<endl;



    }


    out<<res;
    out.close();
    ifstream out2;
   
    out2.open("output.txt");
    cout<<"\nOUTPUT FILE HAS BEEN GENERATED ITS SHOWS THE FOLLOWING \n"<<"______________________________________________________________________"<<endl;
    while(getline(out2,line)) {
        cout<<line<<endl;
    }
    cout<<"______________________________________________________________________"<<endl;

    out2.close();

}




void chkSet(set*tmp){
    string s1;
    set* se1=tmp;
        int i=getLength(tmp->name);
        if(i>32) {
            s1=s1+" "+tmp->name+" set has overflowing bits"+"\tPRESS ENTER TO CONTINUE";
            cout<<s1;
            getchar();              //continue or exit
        }
        chkDuplicate(tmp);
}

/*#chkSubElements checks if there is any element in the set out of the universal set
  # check each element in a set and find out the match in the universal set .
  #
  #@param structure set pointer */


void chkSubElements(set* tmp) {
    string s1=tmp->name;
    string s2,s3;
    set* se1;
    bool notSub=false;
    se1=uni;								
    while(tmp->next) {
        tmp=tmp->next;
        s2=tmp->name;
        while(se1->next) {
            se1=se1->next;
            s3=se1->name;
            if(s2==s3) {
                notSub=false;
                se1=uni->next;
                break;
            }else
                notSub=true;
        }
        if(notSub) {
            cout<<s1<<" is not the sub set of "<<uni->name<<"."<<" '"<<tmp->name<<"' this element is not found in universal set "<<uni->name; 
            getchar();              //here more methods that can be used to continue or exit;
            break;
        }
    }

}

/*chkDupicate checks if there is any element reoccuring twice in a same set.
  
  #set a pointer on first element of the set and then another pointer pointing 
  to next element to previous pointer then move second pointer until it found the 
  match if no same element found moves the first pointer to next element and check
  for every element in the set 

  #@param structure set pointer */


void chkDuplicate(set* tmp){
    set* se1;
    set* se2;
    string s1,s2;
    se2=tmp;
    
    while(tmp->next){
        tmp=tmp->next;
        se1=tmp->next;
        s1=tmp->name;
        while(se1){
            s2=se1->name;
            if(s1==s2){
                cout<<se2->name<<"  has "<<se1->name<<" twice in the same set"<<endl;       //you can add more specs here according to your needs
                break;
            }else
                se1=se1->next;
        }
        
    }
}




set* search(string x) {
    cur=head;
    string s;
    while(cur) {
        s=cur->name;
        if(s==x) {
            return cur;
        } else
            cur=cur->down;
    }
    return nul;
}




set* getIndexElement(set* tmp,int i) {

    for(i; tmp&&i; i--) {
        tmp=tmp->next;
    }
    if(!tmp) {
        cout<<"ELEMENT OUT OF RANGE";
    }
    return tmp;
}



int getLength(string x) {

    set* set1=search(x);
    if(!set1)
        return 0;
    set1=set1->next;
    int i=0;
    while(set1) {
        set1=set1->next;
        i++;
    }

    return (i);
}



void setLength() {

    set* se1=head;
    while(se1) {
        se1->length=getLength(se1->name);
        se1=se1->down;
    }
}



int getBinary (string x,string y) { 
    

    int bin=0;
    string s1,s2;
    int i=getLength(x);
    int j=getLength(y);
    set* tmp=uni;
    tmp=tmp->next;
    set* set1=search(x);
    set1=set1->next;
    for(int n=0; n<j; n++) {
        s1=getIndexElement(tmp,n)->name;
        for(int k=0; k<i; k++) {
            s2=getIndexElement(set1,k)->name;
            if(s1==s2)
                bin |= (1<<n);
        }
    }
    return bin;
}




void setBinary() {

    cur=head;
    while(cur) {
        cur->binary=getBinary(cur->name,uni->name);
        cur=cur->down;
    }
}

/*printElements prints the elements of a particular set with respect to some parameter.
  
  #It sets a pointer to given set in whose respect it will print elements.
  #It takes the binary number and "and" it with the left shift 1.
  #If result of above statement is 1 it will print the corresponding number.

  #@params binary stream
  #@params  structure set pointer
  #return string*/



string printElements (int bin,set* tmp) {
    
    string ele;
    set* extra=tmp;
    extra=extra->next;
    for(int i=0; i<getLength(tmp->name); i++) {

        if(bin & (1<<i)) {

            ele=ele  +getIndexElement(extra,i)->name+" " ;
        }

    }
    return ele;
}




int un (string x,string y) {
    set* se1=search(x);
    set* se2=search(y);
    if(!se1||!se2)
        return 0;
    int a = se1->binary;
    int b = se2->binary;
    return (a | b);
}




int sub (string x,string y) {

    set* se1=search(x);
    set* se2=search(y);
    if(!se1||!se2)
        return 0;
    int a = se1->binary;
    int b = se2->binary;
    return (a &~ b);
}



int inter(string x,string y) {
    set* se1=search(x);
    set* se2=search(y);
    if(!se1||!se2)
        return 0;
    int a = se1->binary;
    int b = se2->binary;

    return (a & b);
}



/*string powPrint(set* tmp,int j){

    string ele;
    set* extra=tmp;
    extra=extra->next;
    for(int i=0; i<getLength(tmp->name); i++) {

        if(j & (1<<i)) {

            ele=ele  +getIndexElement(extra,i)->name+" " ;
        }

    }
    return ele;

}*/



string powerset(set* power){
    string p;
    for(int i = 0; i < pow(2, getLength(power->name)); i++){
        p+="{"+printElements(i,power)+"}   ";
    }
    return p; 
}



void findUnivarsalSet() {
    
    string s1,s2;
    int arr[32];
    memset(arr,0,sizeof(arr));
    int i=0;
    set* ne=head;
    while(ne) {
        arr[i]=getLength(ne->name);
        ne=ne->down;
        i++;
    }
    arr[i-1]='\0';
    sort(arr,arr+(i-1));
    ne=head;
    while(ne) {
        int x=ne->length;
        if(x==arr[i-2]) {
            uni=ne;
            break;
        } else
            ne=ne->down;
    }
}



void printInputSets() {

    cur=head;
    set* tmp;
    while(cur->down) {
        cout<<cur->name<<":  {";
        tmp=cur->next;
        while(tmp) {
            cout<<tmp->name<<" ";
            tmp=tmp->next;
        }
        cout<<"}"<<endl;
        cur=cur->down;
    }
}



