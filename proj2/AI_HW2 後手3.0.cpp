#include<cstdio>
#include<cstdlib>
#include<cmath>
#define max(a,b) (a>b)?a:b
#define min(a,b) (a<b)?a:b
#define swap(a,b,c) {a=0;b=c;}
#define inc(c,n,m) {(c==1)?n++:m++;}
class Node{
public:
Node(int x,int y){cp=-1;b=x;w=y;}
int cp,b,w,eval;
char state[36];
Node *chd[200];
};
Node *root,*temp;
char board[36];
int dir[]={0,-6,+6,-1,1},B,W;
int hands,pit[36],pp=-1,take[13],tp=-1,flag,kight[53],kp=-1;
int Max_val(Node*p);

void cpy(char*s,char*t){for(int i=0;i<36;i++) s[i]=t[i];}

void formal()
{
   int d,piece[4]={14,15,20,21},i,j,p,q,m;
   if(hands==1) d=(board[14])?20:14;
   else if(hands==2)
   {
      for(i=0;i<4;i++) if(!board[piece[i]]) break;
      d=piece[i];
   }
   board[d]=1;
}

int line(char*w,int d,int c)
{
   int i,j,p,q,s=d-(d/6)*6,t=d-d%6,v,u;
   for(i=d-6;i>=s;i-=6) if(w[i]!=c) break;
   for(j=d+6;j<=s+30;j+=6) if(w[j]!=c) break;
   for(p=d-1;p>=t;p--) if(w[p]!=c) break;
   for(q=d+1;q<=t+5;q++) if(w[q]!=c) break;
   return((j-i)==24||(q-p)==4);
}

int rline(char*w,int d,int c,int r)
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

void build_mov(Node*p,int l)
{
   int i,j,m,c,a,b;
   char* str=p->state;
   c=(l%2)?1:2;
   if(!l) return;
   if(hands>12&&(p->b==2||p->w==2)) return;

   tp=kp=-1;
   for(i=0;i<36;i++)
   {
      if(str[i]==3-c) take[++tp]=i;
      if(str[i]==c)
      {
         if(i>5&&!str[i-6]) kight[++kp]=i*10+1;
         if(i<30&&!str[i+6]) kight[++kp]=i*10+2;
         if((i%6)&&!str[i-1])
         {
            kight[++kp]=i*10+3;
            if(hands==13&&p==root) printf("%d\n",i);
         }
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
   int i,j,c;
   char* str=p->state;
   pp=tp=-1;
   c=(l%2)?1:2;
   if(!l) return;
   if(hands>12&&(p->b==2||p->w==2)) return;
   for(i=0;i<36;i++)
   {
     if(!str[i]) pit[++pp]=i;
     if(str[i]==3-c) take[++tp]=i;
   }
   for(i=0;i<=pp;i++)
   {
      str[pit[i]]=c;
      if(line(str,pit[i],c))
      {
         for(j=0;j<=tp;j++)
         {
            str[take[j]]=0;
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

int utility(Node*p)
{
   int i,x,m=0,n=0,c;
   char *s=p->state;
   kp=-1;
   if(hands<12)
   {
      for(i=0;i<36;i++)
      {
        if(s[i]==0)
        {
          if(line(s,i,2)) m++;
          if(line(s,i,1)) n++;
        }
      }
   }
   else
   {
      for(i=0;i<36;i++)
      {
         if(s[i])
         {
           c=s[i]-0;
           if(i>5&&!s[i-6])
           {
              swap(s[i],s[i-6],c);
              if(line(s,i-6,c)||rline(s,i,c,1)) inc(c,n,m);
              swap(s[i-6],s[i],c);
           }
           if(i<30&&!s[i+6])
           {
              swap(s[i],s[i+6],c);
              if(line(s,i+6,c)||rline(s,i,c,2)) inc(c,n,m);
              swap(s[i+6],s[i],c);
           }
           if((i%6)&&!s[i-1])
           {
              swap(s[i],s[i-1],c);
              if(line(s,i-1,c)||rline(s,i,c,3)) inc(c,n,m);
              swap(s[i-1],s[i],c);
           }
           if((i%6-5)&&!s[i+1])
           {
              swap(s[i],s[i+1],c);
              if(line(s,i+1,c)||rline(s,i,c,4)) inc(c,n,m);
              swap(s[i+1],s[i],c);
           }
         }
      }
   }
   if(p->b==2) p->eval=2000;
   else if(p->w==2) p->eval=-2000;
   else p->eval=(hands<12)?((p->w)*5-(p->b)*5+n*2-m):((p->w)-(p->b)*5+n-m*2);
   return p->eval;
}
// best sofar 4 3 2 2 ; 1 5 1 2
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

void print()
{
   for(int i=0;i<6;i++)
   {
     for(int j=0;j<6;j++)
     {
        if(board[i*6+j]==1) printf("O");
        else if(board[i*6+j]==2) printf("X");
        else printf("~");
     }
     printf("\n");
   }
}

void game()
{
   int i,j,m=0,n=0,minimax;
   if(hands<=2) formal();
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
      cpy(board,root->chd[i]->state);
   }
}

int exceed(int r,int i){return(r==1)?!(i>6):(r==2)?!(i<30):(r==3)?!(i%6!=0):!(i%6!=5);}

int Ending()
{
   int i,j;
   for(i=0;i<36;i++)
   {
      for(j=1;j<=4;j++)
      {
         if(board[i]==1&&!exceed(j,i)&&!board[i+dir[j]])
         {
            swap(board[i],board[i+dir[j]],1);
            if(line(board,i+dir[j],1)||rline(board,i,1,j)) return 1;
            swap(board[i+dir[j]],board[i],1);
         }
      }
   }
   return 0;
}

int main(void)
{
   int i,j,m,n;
   hands=1;
   printf("Welcome to play Smile Sheep 1.0\n======================\n");
   while(hands<=300)
   {
       printf("Your turn. %d\n",hands);
       n=0;
       if(hands<13) do{scanf("%d %d",&i,&j);}while(board[6*i+j]);
       else do{scanf("%d %d %d",&i,&j,&n);}while(board[6*i+j+dir[n]]);

       printf("A legal move.\n");
       board[6*i+j+dir[n]]=2;
       if(hands>12) board[6*i+j]=0;

       print();
       if(line(board,6*i+j+dir[n],2)||(hands>12&&rline(board,6*i+j,2,n)))
       {
          printf("take a chess.\n");
          do{scanf("%d %d",&i,&j);}while(board[i*6+j]!=1);
          printf("A legal move.\n");
          board[6*i+j]=0;
          W--;
       }
       if(hands>12&&W==2){printf("You win the game!\n");exit(0);}
       printf("My turn. %d\n",hands);
       if(hands>12&&B==3&&Ending())
       {
         for(i=0;i<36;i++) if(board[i]==2){board[i]=0;break;}
       }
       else game();
       if(hands>12)
       {
          B=W=0;
          for(i=0;i<36;i++)
          {
            if((board[i]-0)==2) B++;
            if((board[i]-0)==1) W++;
          }
       }
       print();
       if(hands>12&&B==2){printf("You Lose!\nWhat a smart AI!\n");exit(0);}

       hands++;
   }
   return 0;
}
