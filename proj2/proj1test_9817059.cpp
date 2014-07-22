#include<time.h>
#include<vector>
#include"proj1search_9817059.h"
#include<iostream>
using namespace std;
char* test(int tile){
    char input[10];
    int count='1';
    int counter=0;
    int temp;
    srand(time(0));
    input[9]=0;
    for(int i=0;i<9;i++){
        input[i]='0';
    }


    while(true){
        if(counter<tile){
            temp=(rand()%9);
            if(input[temp]=='0'){
                input[temp]=count;
                count++;
                counter++;
                continue;
            }else{
                temp=((temp*37+1783)%9);
                continue;
            }
        }else{
            break;
        }

    }
    return input;
}
///////////////////////////////////////////////test function
int main(){
    //for(int i=0;i<10;i++){
        vector<int>* path;
		  path=new vector<int>();
        double a,b;
        char temp[10];
        for(int i=0;i<10;i++){
            temp[i]=test(8)[i];
        }
        a=clock();
        proj1("142358067",2,2,path);
        b=clock();
        cout<<"total time: "<<(b-a)/1000<<" seconds"<<endl;


    return 0;
}
