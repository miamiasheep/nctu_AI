#include<iostream>
#include<vector>
#include<stdlib.h>
#include<queue>
#include<iomanip>
#include<math.h>
#define MAX_COST 100
#define MAX_CHILD 24
#include<time.h>
#define PUZZLE_MAX 5000000
using namespace std;

void printArray(const char* a,int l){
    for(int i=0;i<9;i++){
        cout<<a[i]<<" ";
    }
    cout<<endl;
}


void puzzleCopy(char a[3][3],char b[3][3]){
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            a[i][j]=b[i][j];
        }
    }
}//рb copy a
char puzzleSet[PUZZLE_MAX][3][3];
int puzzleCount=0;
bool comparePuzzle(char a[3][3]){
    bool flag=false;
    //cout<<"in the comparepuzzle: "<<puzzleCount<<endl;
    for(int i=0;i<puzzleCount;i++){
        for(int j=0;j<3;j++){
            for(int k=0;k<3;k++){
                if(a[j][k]!=puzzleSet[i][j][k]){
                    goto next;
                }
            }
        }
        complete: flag=true;
                  goto gameOver;
        next:
            //cout<<"fail\n";
             continue;
    }
        gameOver:
                return flag;
}
void puzzleAdd(char a[3][3]){
    puzzleCopy(puzzleSet[puzzleCount],a);
    puzzleCount++;
  // cout<<puzzleCount<<endl;
}
void puzzleClear(){
    puzzleCount=0;
}
void oneToTwoDi(char *a, char b[3][3]){
    int count=0;
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            b[i][j]=a[count];
            count++;
        }
    }
}
void printPuzzle(char a[][3]){
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            cout<<a[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<"==========================="<<endl;
}//For debug
char *toOneDi(char a[3][3]){
    char ans[9];
    int count=0;
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            ans[count]=a[i][j];
            count++;
        }
    }
    return ans;
}
struct node{
    int g;
    int move;
    //int tile;
    //int   [MAX_COST];
    bool hasChild;
    node* parent;
    node *child[MAX_CHILD];
    char puzzle[3][3];
    node(){
        hasChild=false;
    }

    node(char input[3][3],int cost){
        g=cost;
        hasChild=false;



        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                puzzle[i][j]=input[i][j];
            }
        }

    }
    ////////////////////////////////h1
    int h1(){
        int tile=0;
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                if(puzzle[i][j]!='0')tile++;
            }
        }
        int notMatch;
        notMatch=0;
        int count=0;
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                if((((int)puzzle[i][j]-'0')!=(count+tile-8))&&(puzzle[i][j]!='0')){
                    notMatch++;
                    //cout<<"location: "<<"("<<i<<","<<j<<")"<<endl;
                }
                count++;
            }
        }


        return notMatch;
    }//h1
    ///////h2
    int h2(){
        int ans=0;
        int tile=0;
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                if(puzzle[i][j]!='0')tile++;
            }
        }
        //cout<<"tile: "<<tile<<endl;
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                if(puzzle[i][j]!='0'){
                    ans=ans+abs(i-((int)(puzzle[i][j]-'0'-tile+8)/3))+abs(j-((int)(puzzle[i][j]-'0'-tile+8)%3));

                }
            }
        }
        //cout<<ans<<endl;
        return ans;
    }
    //////h2
};
class CompareNode1{
    public:
    bool operator()(node*& n1,node*& n2){
        if((n1->h1()+n1->g)>(n2->h1()+n2->g))return true;
        return false;
    }
};

class CompareNode2{
    public:
    bool operator()(node*& n1,node*& n2){
        if((n1->h2()+n1->g)>(n2->h2()+n2->g))return true;
        return false;
    }
};

priority_queue<node*,vector<node*>,CompareNode1> pq1;
priority_queue<node*,vector<node*>,CompareNode1> pq2;
int countPuzzle=0;
void expand(node *current,int hFunc){
    current->hasChild=true;
    char temp[3][3];
    int countChild=0;
    puzzleCopy(temp,current->puzzle);//рcurrent->puzzle狡籹temp皚暗ǐ˙笆

    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            if(current->puzzle[i][j]=='0'){
                if((i-1)>=0){
                    if(current->puzzle[i-1][j]!='0'){
                            puzzleCopy(temp,current->puzzle);
                            swap(temp[i-1][j],temp[i][j]);
                            if(!comparePuzzle(temp)){
                                current->child[countChild]=new node(temp,((current->g)+1));
                                //swap(current->child[countChild]->puzzle[i-1][j],current->child[countChild]->puzzle[i][j]);
                               if(hFunc==1) pq1.push(current->child[countChild]);
                                if(hFunc==2)pq2.push(current->child[countChild]);
                                current->child[countChild]->move=2+((current->puzzle[i-1][j]-'0')*10);
                                current->child[countChild]->parent=current;
                                countChild++;
                                puzzleAdd(temp);

                            }
                    }//0禲(0⊿Τ禬τ琌计)计┕
                }//狦0ぃ穦禬
                if(((i+1)<=2)&&(current->puzzle[i+1][j]!='0')){
                    puzzleCopy(temp,current->puzzle);
                    swap(temp[i+1][j],temp[i][j]);
                    if(!comparePuzzle(temp)){
                        current->child[countChild]=new node(temp,((current->g)+1));
                        //swap(current->child[countChild]->puzzle[i+1][j],current->child[countChild]->puzzle[i][j]);
                        if(hFunc==1)pq1.push(current->child[countChild]);
                        if(hFunc==2)pq2.push(current->child[countChild]);
                        current->child[countChild]->move=4+((current->puzzle[i+1][j]-'0')*10);


                        current->child[countChild]->parent=current;
                        countChild++;
                        puzzleAdd(temp);
                        //cout<<puzzleCount<<endl;
                    }
                }//0┕计┕
                if(((j-1)>=0)&&(current->puzzle[i][j-1]!='0')){
                    puzzleCopy(temp,current->puzzle);
                    swap(temp[i][j-1],temp[i][j]);
                    if(!comparePuzzle(temp)){
                        current->child[countChild]=new node(temp,((current->g)+1));
                    //swap(current->child[countChild]->puzzle[i][j-1],current->child[countChild]->puzzle[i][j]);
                        if(hFunc==1)pq1.push(current->child[countChild]);
                        if(hFunc==2)pq2.push(current->child[countChild]);
                        current->child[countChild]->move=3+((current->puzzle[i][j-1]-'0')*10);
                        current->child[countChild]->parent=current;
                        //puzzleCopy(puzzleSet[puzzleCount],temp);
                        countChild++;
                        puzzleAdd(temp);
                    }

                }//┕オ计┕
                if(((j+1)<=2)&&(current->puzzle[i][j+1]!='0')){
                    puzzleCopy(temp,current->puzzle);
                    swap(temp[i][j+1],temp[i][j]);
                    if(!comparePuzzle(temp)){
                        current->child[countChild]=new node(temp,((current->g)+1));
                        //swap(current->child[countChild]->puzzle[i][j+1],current->child[countChild]->puzzle[i][j]);
                        if(hFunc==1)pq1.push(current->child[countChild]);
                        if(hFunc==2)pq2.push(current->child[countChild]);
                        current->child[countChild]->move=1+((current->puzzle[i][j+1]-'0')*10);
                        current->child[countChild]->parent=current;
                        countChild++;
                        puzzleAdd(temp);
                    }

                }//0┕计┕オ

            }//狦笿じ0

        }//

    }//тじ'0'眔稱快猭よ暗ユ传

}// function expand
///////////////////////////////////////DFSLimit
bool finish=false;
node* ans;

void DFSLimit(node* current,int limit ){
    current->hasChild=true;
    char temp[3][3];
    int countChild=0;
    puzzleCopy(temp,current->puzzle);//рcurrent->puzzle狡籹temp皚暗ǐ˙笆

    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            if(temp[i][j]=='0'){// for each node's action
                if((i-1)>=0){
                    if(temp[i-1][j]!='0'){
                        if(limit>0){
                            current->child[countChild]=new node(temp,((current->g)+1));
                            swap(current->child[countChild]->puzzle[i-1][j],current->child[countChild]->puzzle[i][j]);
                            current->child[countChild]->move=2+((temp[i-1][j]-'0')*10);
                            current->child[countChild]->parent=current;
                            ////add a child node
                            if(current->child[countChild]->h1()!=0){// if the child is not the answer, keep finding
                                DFSLimit(current->child[countChild],limit-1);
                                countChild++;
                            }else{//h1()==0   answer!!!  finish and return
                                finish=true;
                                ans=current->child[countChild];
                                return;
                            }


                        }
                    }//0禲(0⊿Τ禬τ琌计)计┕
                }//狦0ぃ穦禬
                if(((i+1)<=2)&&(temp[i+1][j]!='0')){
                    if(limit>0){
                    current->child[countChild]=new node(temp,((current->g)+1));
                    swap(current->child[countChild]->puzzle[i+1][j],current->child[countChild]->puzzle[i][j]);
                    current->child[countChild]->move=4+((temp[i+1][j]-'0')*10);
                    current->child[countChild]->parent=current;
                    if(current->child[countChild]->h1()!=0){
                        DFSLimit(current->child[countChild],limit-1);
                        countChild++;
                        }else{
                            finish=true;
                            //puzzleCopy(ans->puzzle,current->child[countChild]->puzzle);
                            ans=current->child[countChild];
                            return;
                        }
                    }
                        //cout<<puzzleCount<<endl;

                }//0┕计┕
                if(((j-1)>=0)&&(temp[i][j-1]!='0')){
                    if(limit>0){
                        current->child[countChild]=new node(temp,((current->g)+1));
                        swap(current->child[countChild]->puzzle[i][j-1],current->child[countChild]->puzzle[i][j]);
                        current->child[countChild]->move=3+((temp[i][j-1]-'0')*10);
                        current->child[countChild]->parent=current;
                        puzzleCopy(puzzleSet[puzzleCount],temp);
                        if(current->child[countChild]->h1()!=0){
                            DFSLimit(current->child[countChild],limit-1);
                            countChild++;
                        }else{
                            finish=true;
                            ans=current->child[countChild];
                            return;
                        }
                    }

                }//┕オ计┕
                if(((j+1)<=2)&&(temp[i][j+1]!='0')){
                    if(limit>0){
                        current->child[countChild]=new node(temp,((current->g)+1));
                        swap(current->child[countChild]->puzzle[i][j+1],current->child[countChild]->puzzle[i][j]);
                        current->child[countChild]->move=1+((temp[i][j+1]-'0')*10);
                        current->child[countChild]->parent=current;
                        if(current->child[countChild]->h1()!=0){
                            DFSLimit(current->child[countChild],limit-1);
                            countChild++;
                        }else{
                            finish=true;
                            ans=current->child[countChild];
                            return;
                        }
                    }//limit>=0

                }//0┕计┕オ

            }//狦笿じ0    if meet 0

        }//

    }//    for all possible blank which can be exchanged.
   if(!finish) delete current;//all children are checked , current can be deleted
}

/////////////////////////////////////////////////////////////DFSLimit
bool preCheck(char* puzzle){
    int zeroCount=0;
    for(int i=0;i<9;i++){
        if(puzzle[i]=='0')zeroCount++;
    }
    if(zeroCount>1)return true;// true Τ秆
    ////////////////////////////////
    char pivot;
    int parity=0;
    for(int i=0;i<9;i++){
        //if(puzzle[i]=='0')continue;
        pivot=puzzle[i];
        for(int j=i;j<9;j++){
            if(puzzle[j]>puzzle[i]){
                if(puzzle[i]=='0')continue;
                parity++;
                //cout<<parity<<endl;
            }
        }
    }
    if((parity%2)==0)return true;
    if((parity%2)==1){
        cout<<"the puzzle has no solution"<<endl;
        return false;
    }
}

//the search function
void proj1(const  char* init, int alg, int hFunc, vector<int>* path){
    //move 11 1 left;  12 1 down; 13 1 right; 14 1 up;
   path=new vector<int>();
    node* root;
    char test[10];
    char test2[3][3];
//int path[MAX_COST];
    node* temp;
    char puzzle[9];//the input puzzle
    //cout<<"init"<<endl;
    test[0]=init[0];
    test[1]=init[1];
    test[2]=init[2];
    test[3]=init[3];
    test[4]=init[4];
    test[5]=init[5];
    test[6]=init[6];
    test[7]=init[7];
    test[8]=init[8];
    test[9]=init[9];
    /****
    for(int i=2;i<9;i++){
        test[i]=init[i];
    }
    ***/
    //printArray(test,9);
    oneToTwoDi(test,test2);
    //printPuzzle(test2);
    root=new node(test2,0);
    cout<<"the origin puzzle is"<<endl;
    printPuzzle(root->puzzle);
    puzzleAdd(root->puzzle);
    int limit=1;
    if(alg==1){//IDS
        if(preCheck(test)){
        if(root->h1()==0){
            ans=root;
        }else{
            while(!finish){
                //puzzleClear();
                root=new node(test2,0);
               //puzzleAdd(root->puzzle);
                DFSLimit(root,limit);
                limit++;
                //delete root;
            }
        }
            printPuzzle(ans->puzzle);
            cout<<"the total cost is: "<<ans->g<<endl;
            temp=ans;
            /*******
            cout<<"check:\n";
            printPuzzle(ans->parent->puzzle);
            cout<<ans->parent->g<<endl;
            *****/
            for(int i=0;i<ans->g;i++){

                       //path[ans->g-1-i]=temp->move;
                        path->push_back(temp->move);
                        //system("pause");
                        temp=temp->parent;
                         //system("pause");

                    }
                    for(int i=ans->g-1;i>=0;i--){
                        if(i==0){
                            cout<<path->at(i);
                            //cout<<path.at(i);
                            break;
                        }
                        cout<<path->at(i)<<"->";

                    }

                    cout<<endl;
        }else{
            cout<<"the puzzle has no solution"<<endl;
        }
    }
    if(alg==2){//A*
    //node *root;
    //char test[10]="012346587";
    //char test[10]="000001234";
    //char test[10]="724506831";
    if(preCheck(test)){
        if(hFunc==1){

            pq1.push(root);

            while(true){
                if(pq1.top()->h1()==0){
                    cout<<"find the answer!!"<<endl;
                    cout<<"the total cost is: "<<pq1.top()->g<<endl;
                    printPuzzle(pq1.top()->puzzle);
                    cout<<"the path is: \n";
                    temp=pq1.top();
                    for(int i=0;i<pq1.top()->g;i++){
                        //path[pq1.top()->g-1-i]=temp->move;
                        path->push_back(temp->move);
                        temp=temp->parent;
                    }
                    for(int i=(pq1.top()->g-1);i<pq1.top()->g>=0;i--){
                        if(i==0){
                            cout<<path->at(i);
                            break;
                        }
                        cout<<path->at(i)<<"->";

                    }
                    cout<<endl;
                    break;
                }
                expand(pq1.top(),hFunc);
                pq1.pop();
            }//while
        }//hMethod1;
        if(hFunc==2){
            pq2.push(root);
            while(true){
                if(pq2.top()->h2()==0){
                    cout<<"find the answer!!"<<endl;
                    cout<<"the total cost is: "<<pq2.top()->g<<endl;
                    printPuzzle(pq2.top()->puzzle);
                    cout<<"the path is: \n";
                    temp=pq2.top();
                    //system("pause");
                    for(int i=0;i<pq2.top()->g;i++){
                        //path[pq2.top()->g-1-i]=temp->move;
                        //system("pause");
                       // cout<<temp->move<<endl;
                        path->push_back(temp->move);
                        //system("pause");
                        temp=temp->parent;
                    }
                    //system("pause");
                    for(int i=(pq2.top()->g-1);i>=0;i--){
                        if(i==0){
                            cout<<path->at(i);
                            break;
                        }
                        cout<<path->at(i)<<"->";

                    }
                    cout<<endl;
                    break;
                }
                expand(pq2.top(),hFunc);
                pq2.pop();
            }//while

        }//hMethod==2
    }else{
        cout<<"there is no solution for this puzzle\n";
    }
    }
    puzzleClear();
    while(!pq1.empty()){
        pq1.pop();
    }
    while(!pq2.empty()){
        pq2.pop();
    }
    delete path;
    //cout<<"haha"<<endl;
}
//////the search function
