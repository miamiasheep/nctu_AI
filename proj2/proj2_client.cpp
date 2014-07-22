#include<iostream>
#include<fstream>
#include<string>
#include<ctime>
#include<cmath>
#include<cstdlib>
#define max(a,b) (a>b)?a:b
#define min(a,b) (a<b)?a:b
#define swap(a,b,c) {a=0;b=c;}
#define inc(c,v,n,m) {(c==v)?n++:m++;}
using namespace std;
class Node{
public:
Node(int x,int y){cp=-1;b=x;w=y;}
int cp,b,w,eval;
int state[36];
Node *chd[200];
};
Node *root,*temp;
int board[36],T,tricky,colmajor;
int dir[]={0,-6,+6,-1,1},D,OP,fourth[]={19,27},deadlock;
int hands=1,pit[36],pp=-1,take[13],tp=-1,flag,kight[53],kp=-1;

int player;

struct Coordinate
{
  int x;
  int y;
  std::string direction;
};

int Max_val(Node*p);

void cpy(int*s,int*t){for(int i=0;i<36;i++) s[i]=t[i];}

void formal()
{
   int d,i,piece[4]={14,15,20,21};
   if(hands==1) D=(player==1)?200:(board[14])?200:140;
   else if(hands==2)
   {
     if(player==1)
     {
        D=(!board[21])?210:140;
        OP=!(board[21]);
        if(board[21]==2) colmajor=1;
     }
     else
     {
        for(i=0;i<4;i++) if(!board[piece[i]]) break;
        D=piece[i]*10;
     }
   }
   else if(hands==3)
   {
     D=(OP)?220:260;
     if(OP) T=15;
     else T=21;
   }
}

int line(int*w,int d,int c)
{
   int i,j,p,q,s=d-(d/6)*6,t=d-d%6,v,u;
   for(i=d-6;i>=s;i-=6) if(w[i]!=c) break;
   for(j=d+6;j<=s+30;j+=6) if(w[j]!=c) break;
   for(p=d-1;p>=t;p--) if(w[p]!=c) break;
   for(q=d+1;q<=t+5;q++) if(w[q]!=c) break;
   return((j-i)==24||(q-p)==4);
}

int rline(int*w,int d,int c,int r)
{
   int i,j,m,up,lft;
   up=(d/6>2)?1:-1;lft=(d%6>2)?1:-1;
   for(i=1;i<4;i++) if(w[d-6*up*i]!=c) break;
   j=d-6*up*i;
   if(!((up>0&&r==1)||(up<0&&r==2))&&i==4&&((j+up*6)<6||(j+up*6)>29||w[j]!=c)) return 1;
   for(i=1;i<4;i++) if(w[d-lft*i]!=c) break;
   j=d-lft*i;
   if(!((lft>0&&r==3)||(lft<0&&r==4))&&i==4&&(!((j+lft)%6)||!((j+lft)%6-5)||w[j]!=c)) return 1;
   return 0;
}

int mean(int*s,int c)
{
   double dx=0,dy=0,n=0;
   int i;
   for(i=0;i<36;i++)
   {
      if(s[i]==c){n++;dx+=i/6.0;dy+=i%6;}
   }
   dx/=n;dy/=n;
   return (int)dx*6+(int)dy;
}

void build_mov(Node*p,int l)
{
   int i,j,m,c,a,b;
   int* str=p->state;
   c=(l%2)?player:3-player;
   if(!l) return;
   if(p->b==2||p->w==2) return;

   tp=kp=-1;
   for(i=0;i<36;i++)
   {
      if(str[i]==3-c) take[++tp]=i;
      if(str[i]==c)
      {
         if(i>5&&!str[i-6]) kight[++kp]=i*10+1;
         if(i<30&&!str[i+6]) kight[++kp]=i*10+2;
         if((i%6)&&!str[i-1]) kight[++kp]=i*10+3;
         if((i%6-5)&&!str[i+1]) kight[++kp]=i*10+4;
      }
   }
   for(i=0;i<=kp;i++)
   {
      a=kight[i]/10;
      b=kight[i]%10;
      swap(str[a],str[a+dir[b]],c);
      if(line(str,a+dir[b],c)||rline(str,a,c,b))
      {
         for(j=0;j<=tp;j++)
         {
           str[take[j]]=0;
           if(c==1) p->chd[++p->cp]=new Node(p->b-1,p->w);
           else p->chd[++p->cp]=new Node(p->b,p->w-1);
           cpy(p->chd[p->cp]->state,str);
           str[take[j]]=3-c;
         }
      }
      else
      {
         p->chd[++p->cp]=new Node(p->b,p->w);
         cpy(p->chd[p->cp]->state,str);
      }
      swap(str[a+dir[b]],str[a],c);
   }
   for(i=0;i<=p->cp;i++) build_mov(p->chd[i],l-1);
}

void build(Node*p,int l)
{
   int i,j,c,k;
   int* str=p->state;
   pp=tp=-1;
   c=(l%2)?player:3-player;
   if(!l) return;
   if(hands>12&&(p->b==2||p->w==2)) return;

   int m=(p==root)?(player==1)?mean(p->state,1):mean(p->state,2):0;

   if(tricky||player==2)
   {
      for(i=m;i>m-36;i--)
      {
       j=(i<0)?i+36:i;
       if(!str[j]) pit[++pp]=j;
       if(str[j]==3-c) take[++tp]=j;
      }
   }
   else
   {
      if(colmajor)
      {
        for(i=m%6;i>m%6-6;i--)
        {
           for(j=0;j<6;j++)
           {
              k=(i<0)?i+6:i;
              if(!str[j*6+k]) pit[++pp]=j*6+k;
              if(str[j*6+k]==3-c) take[++tp]=j*6+k;
           }
        }
      }
      else{
        for(i=m;i<m+36;i++)
        {
          j=(i>35)?i-36:i;
          if(!str[j]) pit[++pp]=j;
          if(str[j]==3-c) take[++tp]=j;
        }
      }
   }
   for(i=0;i<=pp;i++)
   {
      str[pit[i]]=c;
      if(line(str,pit[i],c))
      {
         if(p==root&&deadlock){str[pit[i]]=0;continue;}
         for(j=0;j<=tp;j++)
         {
            str[take[j]]=0;
            if(p==root&&!deadlock&&line(str,take[j],3-c))
            {
               str[take[j]]=3-c;
               continue;
            }
            if(c==1) p->chd[++p->cp]=new Node(p->b-1,p->w+1);
            else p->chd[++p->cp]=new Node(p->b+1,p->w-1);
            cpy(p->chd[p->cp]->state,str);
            str[take[j]]=3-c;
         }
      }
      else
      {
         if(c==1) p->chd[++p->cp]=new Node(p->b,p->w+1);
         else p->chd[++p->cp]=new Node(p->b+1,p->w);
         cpy(p->chd[p->cp]->state,str);
      }
      str[pit[i]]=0;
   }
   for(i=0;i<=p->cp;i++) build(p->chd[i],l-1);
}

int exceed(int r,int i){return(r==1)?!(i>5):(r==2)?!(i<30):(r==3)?!(i%6!=0):!(i%6!=5);}

int wall(int*s,int c)
{
   int i,j,m=0,n=0,Vt=0,ans=0,ans2=0;
   for(i=0;i<6;i++)
   {
      for(j=i;j<i+36;j+=6)
      {
         if(s[j]!=c){Vt=0;if(m==3) ans++; m=0;}
         else{
            if(!Vt)
            {
              m++;
              if(m>3) Vt=1;
            }
         }
      }
   }
   Vt=0;
   for(i=0;i<36;i+=6)
   {
      for(j=i;j<i+6;j++)
      {
         if(s[j]!=c){Vt=0;if(n==3) ans2++; n=0;}
         else{
            if(!Vt)
            {
              n++;
              if(n>3) Vt=1;
            }
         }
      }
   }
   return ans+ans2;
}

int Md(int*s)
{
   double dx=0,dy=0,dx2=0,dy2=0,n=0,n2=0;
   for(int i=0;i<36;i++)
   {
      if(s[i]==player){n++;dx+=i/6.0;dy+=i%6;}
      if(s[i]==3-player){n2++;dx2+=i/6.0;dy2+=i%6;}
   }
   dx/=n;dy/=n;
   dx2/=n2;dy2/=n2;
   return (int)sqrt((dx-dx2)*(dx-dx2)+(dy-dy2)*(dy-dy2));
}

int block(int*s,int c)
{
   int i,j,ans=0,mv[]={0,1,6,7};
   for(i=0;i<=28;i++)
   {
     if(i%6!=5)
     {
       for(j=0;j<4;j++) if(s[i+mv[j]]!=c) break;
       if(j==4) ans++;
     }
   }
   return ans;
}

int utility(Node*p)
{
   int i,x,m=0,n=0,c,y,j,f,f2;
   int *s=p->state,sword,Vt,heur;
   kp=-1;
   f2=(player==1)?13:12;
   if(hands<f2)
   {
      for(i=0;i<36;i++)
      {
        if(s[i]==0)
        {
          if(line(s,i,3-player)) m++;
          if(line(s,i,player)) n++;
        }
      }
   }
   else
   {
      for(i=0;i<36;i++)
      {
         if(s[i])
         {
           c=s[i];
           for(j=1;j<5;j++)
           {
              if(!exceed(j,i)&&!s[i+dir[j]])
              {
                 swap(s[i],s[i+dir[j]],c);
                 if(line(s,i+dir[j],c)||rline(s,i,c,j)) inc(c,player,n,m);
                 swap(s[i+dir[j]],s[i],c);
              }
           }
         }
      }
   }
   x=(player==1)?p->w:p->b;
   y=(player==1)?p->b:p->w;
   f=(player==1)?13:12;
   Vt=(player==1)?5:3;
   heur=(player==1)?x*2-y*5+n-m:wall(p->state,2)+x*3-y*5+n*2-m*2;
   if(y==2&&hands>11) p->eval=2000;
   else if(x==2&&hands>11) p->eval=-2000;
   else
   {
      sword=(player==1)?(tricky)?-4*wall(p->state,2)+wall(p->state,1)-Md(p->state):wall(p->state,1):-Md(p->state)/2-3*block(p->state,1);
      p->eval=(hands<f)?(sword+x*3-y*Vt+n*2-m*2):heur;
   }
   return p->eval;
}

int Min_val(Node*p)
{
   int i,v=2000;
   if(p->cp==-1) return utility(p);
   for(i=0;i<=p->cp;i++) v=min(v,Max_val(p->chd[i]));
   p->eval=v;
   return v;
}

int Max_val(Node*p)
{
   int i,v=-2000;
   if(p->cp==-1) return utility(p);
   for(i=0;i<=p->cp;i++) v=max(v,Min_val(p->chd[i]));
   p->eval=v;
   return v;
}

int cpl(int x){return(x<3)?3-x:7-x;}

void ext(int*s)
{
   int k=-1,i,j,pos;
   for(i=0;i<36;i++) if(s[i]&&!root->state[i]){pos=i;break;}
   for(i=1;i<5;i++)
      if(!exceed(i,pos)&&(!s[pos+dir[i]]&&root->state[pos+dir[i]]==player)) break;
   if(i==5) D=pos*10;
   else D=(pos+dir[i])*10+cpl(i);
   for(j=0;j<36;j++) if(!s[j]&&root->state[j]==3-player) break;
   T=(j==36)?-1:j;
}

void testD()
{
   deadlock=0;
   for(int i=0;i<36;i++)
   {
      if(board[i]==3-player)
      {
         board[i]=0;
         if(!line(board,i,3-player)){board[i]=3-player;return;}
         board[i]=3-player;
      }
   }
   deadlock=1;
}

void cap(int d)
{
   int i,j,m,n;
   m=mean(board,3-player);
   if(!line(board,d,player)) return;
   for(i=m;i>m-36;i--)
   {
      if(board[(i+36)%36]==3-player)
      {
         if(!line(board,(i+36)%36,3-player)){n=i;break;}
      }
   }
   T=(n+36)%36;
}

int attack()
{
   if(!deadlock&&!board[fourth[OP]]){D=fourth[OP]*10;cap(fourth[OP]);return 1;}
   else if(!deadlock&&OP&&!board[fourth[OP]-6]){D=(fourth[OP]-6)*10;cap(fourth[OP]-6);return 1;}
   else if(!deadlock&&!OP&&!board[fourth[OP]+1]){D=(fourth[OP]+1)*10;cap(fourth[OP]+1);return 1;}
   return 0;
}

void game()
{
   int i,j,m=0,n=0,minimax;
   if(hands==4&&board[fourth[OP]])
   {
     tricky=1;
     fourth[OP]=(OP)?fourth[OP]-6:fourth[OP]+1;
   }
   if(hands<=2||(player==1&&hands==3)) formal();
   else if(player==1&&hands<13&&attack()) ;
   else
   {
      for(i=0;i<36;i++)
      {
        if(board[i]==2) m++;
        if(board[i]==1) n++;
      }
      root=new Node(m,n);
      cpy(root->state,board);
      if(hands<13) build(root,3);
      else build_mov(root,3);
      minimax=Max_val(root);
      for(i=0;i<=root->cp;i++) if(root->chd[i]->eval==minimax) break;
      ext(root->chd[i]->state);
   }
}

int Ending()
{
   int i,j;
   for(i=0;i<36;i++)
   {
      for(j=1;j<=4;j++)
      {
         if(board[i]==player&&!exceed(j,i)&&!board[i+dir[j]])
         {
            swap(board[i],board[i+dir[j]],player);
            if(line(board,i+dir[j],player)||rline(board,i,player,j))
            {
               D=i*10+j;
               return 1;
            }
            swap(board[i+dir[j]],board[i],player);
         }
      }
   }
   return 0;
}

///////////my code////////////////////

void LoadChessBoard(int[6][6]);
Coordinate action(int[6][6],string cmd,int err_msg);
int main(int argc,char * argv[])
{
	srand((unsigned)time(NULL));
	player = atoi(argv[1]);       //player id
	int chessboard[6][6]; //chessboard state
	int id = 0;
	Coordinate output;
	string line,cmd;
	int err_msg;
	while (true)
	{
		ifstream ReqFile("request.txt");
		getline(ReqFile,line,' ');
		if (atoi(line.c_str()) > id)
		{
			id = atoi(line.c_str());
			getline(ReqFile,line,' ');
			cmd = line.c_str();
			getline(ReqFile,line);
			err_msg = atoi(line.c_str());
			LoadChessBoard(chessboard);
			output = action(chessboard,cmd,err_msg);
			cout<<"Read: "<<id<<" "<<cmd<<" "<<err_msg<<endl;
			ofstream fout("action.txt");
			id++;
			if (cmd.compare("/move")==0)
			{
				fout<<id<<" "<<cmd<<" "<<output.x<<" "<<output.y<<" "<<output.direction;
				cout<<"Write: "<<id<<" "<<cmd<<" "<<output.x<<" "<<output.y<<" "<<output.direction<<endl;
			}
			else if(cmd.compare("/drop")==0)
			{
				fout<<id<<" "<<cmd<<" "<<output.x<<" "<<output.y;
				cout<<"Write: "<<id<<" "<<cmd<<" "<<output.x<<" "<<output.y<<endl;
			}
			else
			{
                fout<<id<<" "<<cmd<<" "<<output.x<<" "<<output.y;
				cout<<"Write: "<<id<<" "<<cmd<<" "<<output.x<<" "<<output.y<<endl;
			}
		}
		ReqFile.close();
	}
	return 0;
}

Coordinate action(int chessboard[6][6], string cmd, int err_msg)
{
	Coordinate point;
	int Q=0,i,j;

    if(cmd.compare("/capture")==0)
    {
       point.x=T/6;
       point.y=T%6;
       point.direction="";
       return point;
    }
    for(i=0;i<6;i++)
      for(j=0;j<6;j++)
        board[i*6+j]=chessboard[i][j];
    for(i=0;i<36;i++) if(board[i]==3-player) Q++;
    T=-1;
    if(cmd.compare("/move")==0&&Q==3&&Ending())
    {
       for(i=0;i<36;i++) if(board[i]==3-player){T=i;break;}
    }
    else{testD();game();}

	point.x = (D/10)/6;  //assign your x-coordinate here
	point.y = (D/10)%6;  //assign your y-coordinate here
	string direction[] = {"","up","down","left","right"};
	point.direction = direction[D%10]; //direction: ""=do nothing,"up","down","left","right"
	printf("%d rounds\n",hands);
	hands++;
	return point;
}

void LoadChessBoard(int chessboard[6][6])
{
	ifstream BoardFile("chessboard.txt");
	string line;
	int j = 0;
	for (int i=0;i<6;i++)
	{
		j = 0;
		getline(BoardFile,line);
		for(int k=0;k<12;k=k+2)
		{
			chessboard[i][j] = line[k] - '0';
			j++;
		}
	}
	BoardFile.close();
}

