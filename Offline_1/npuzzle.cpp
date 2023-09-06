#include <iostream>
#include <stdlib.h>
#include <queue>
#include <bits/stdc++.h>


using namespace std;

int r,c;



class node{
    public:
        int** state=nullptr;
        node* parent=nullptr;
        int depth=0;
        int h=0;
        int f=0;

        node(int** state,node* parent,int depth,int h,int f){
            this->state=state;
            this->parent=parent;
            this->depth=depth;
            this->h=h;
            this->f=f;
        }

        node(int row,node* parent,int depth,int** state){
            this->state=new int*[row];
            for(int i=0;i<row;i++){
                this->state[i]=new int[row];
            }
            for(int i=0;i<row;i++)
                for(int j=0;j<row;j++)
                    this->state[i][j]=state[i][j];
            this->parent=parent;
            this->depth=depth;
            this->h=0;
            this->f=0;
        }

        node(){
            this->state=nullptr;
            this->parent=nullptr;
            this->depth=0;
            this->h=0;
            this->f=f;
        }

};

struct myComp {
  bool operator()(node& x, node& y)
  {
    return x.f > y.f;
  }
};



bool solvable(int** puzzle,int row){
    
    int *arr=new int[row*row-1];
    int cnt=0;
    int irr=0;
    int zrr=0;

    for(int i=0;i<row;i++)
        for(int j=0;j<row;j++){
            if(puzzle[i][j]!=0){
                arr[cnt]=puzzle[i][j];
                cnt++;
            }
        }

    for(int i=0;i<cnt;i++)
        for(int j=i+1;j<cnt;j++)
            if(arr[i]>arr[j])
                irr++;

    
    if(row%2==1){
        if(irr%2==0)
            return true;
        else
            return false;
    }else{
        for(int i=(row-1);i>=0;i--){
            for(int j=0;j<row;j++)
                if(puzzle[i][j]==0)
                    zrr=row-i;
        }           

        if(((zrr%2==0)&&(irr%2==1))||((zrr%2==1)&&(irr%2==0)))
            return true;
        else
            return false;
    }

    
}


void cord(int** dest,int number,int row){
    for(int i=0;i<row;i++)
        for(int j=0;j<row;j++)
            if(dest[i][j]==number){
                r=i;
                c=j;
            }
}

int manhattan(int** puzzle,int** dest,int row){
    int man=0;
    for(int i=0;i<row;i++)
        for(int j=0;j<row;j++)
            if(puzzle[i][j]!=0){
                cord(dest,puzzle[i][j],row);
                man=man+abs(i-r)+abs(j-c);
            }
    return man;
}

int hamming(int** puzzle,int** dest,int row){
    int ham=0;

    for(int i=0;i<row;i++)
        for(int j=0;j<row;j++)
            if(dest[i][j]!=0)
                if(dest[i][j]!=puzzle[i][j])
                    ham++;

    return ham;
}

void RecursivePrinter(node* temp,int row){
    if(temp->parent!=nullptr)
        RecursivePrinter(temp->parent,row);

    for(int i=0;i<row;i++){
        cout<<endl;
        for(int j=0;j<row;j++)
            if(temp->state[i][j]==0)
                cout<<"*"<<" ";
            else
                cout<<temp->state[i][j]<<" ";
    }  
    cout<<endl;
}

string storage(node n,int row){
    string s="";
    for(int i=0;i<row;i++)
        for(int j=0;j<row;j++)
            s=s+to_string(n.state[i][j]);
    
    return s;
}

void AstarMan(int** puzzle,int** dest,int row){
   
    node first(puzzle,nullptr,0,manhattan(puzzle,dest,row),manhattan(puzzle,dest,row));
    priority_queue<node, vector<node>, myComp> pq;
    pq.push(first);
    map<string,int> closedList;

    int zr,zc;

    while(!pq.empty()){
    
        node* n=new node(row,pq.top().parent,pq.top().depth,pq.top().state);  
        n->h=pq.top().h;
        n->f=pq.top().f;

        pq.pop();
        if(n->h==0){
           // cout<<"successful---------------------------------"<<endl;
            node* temp=n;
            cout<<"Minimum number of moves = "<<n->depth<<endl;
            RecursivePrinter(temp,row);
            break;
        }else{
            for(int i=0;i<row;i++)
                for(int j=0;j<row;j++)
                    if(n->state[i][j]==0){
                        zr=i;
                        zc=j;
                    }
                
            //left
            if(zc>0){
                node n2(row,n,n->depth+1,n->state);
                n2.state[zr][zc]=n2.state[zr][zc-1];
                n2.state[zr][zc-1]=0;
                n2.h=manhattan(n2.state,dest,row);
                n2.f=n2.h+n2.depth;
                string ss=storage(n2,row);
                auto it = closedList.find(ss);
                if (it == closedList.end()) {
                    pq.push(n2);
                    closedList.emplace(ss,1);
                }
            }
            //right
            if(zc<(row-1)){
                node n2(row,n,n->depth+1,n->state);                                      
                n2.state[zr][zc]=n2.state[zr][zc+1];   
                n2.state[zr][zc+1]=0;                 
                n2.h=manhattan(n2.state,dest,row);      
                n2.f=n2.h+n2.depth;
                string ss=storage(n2,row);
                auto it = closedList.find(ss);
                if (it == closedList.end()) {
                    pq.push(n2);
                    closedList.emplace(ss,1);
                }     
            }
            //top
            if(zr>0){                
                node n2(row,n,n->depth+1,n->state);
                n2.state[zr][zc]=n2.state[zr-1][zc];
                n2.state[zr-1][zc]=0;
                n2.h=manhattan(n2.state,dest,row);
                n2.f=n2.h+n2.depth;
                string ss=storage(n2,row);
                auto it = closedList.find(ss);
                if (it == closedList.end()) {
                    pq.push(n2);
                    closedList.emplace(ss,1);
                }
            }
            //bottom
            if(zr<(row-1)){
                node n2(row,n,n->depth+1,n->state);
                n2.state[zr][zc]=n2.state[zr+1][zc];
                n2.state[zr+1][zc]=0;
                n2.h=manhattan(n2.state,dest,row);
                n2.f=n2.h+n2.depth;
                string ss=storage(n2,row);
                auto it = closedList.find(ss);
                if (it == closedList.end()) {
                    pq.push(n2);
                    closedList.emplace(ss,1);
                }
            } 
        }
    }
    //cout<<"astar end"<<endl;
}



void AstarHam(int** puzzle,int** dest,int row){
   
    node first(puzzle,nullptr,0,hamming(puzzle,dest,row),hamming(puzzle,dest,row));
    priority_queue<node, vector<node>, myComp> pq;
    pq.push(first);
    map<string,int> closedList;

    int zr,zc;

    while(!pq.empty()){
    
        node* n=new node(row,pq.top().parent,pq.top().depth,pq.top().state);  
        n->h=pq.top().h;
        n->f=pq.top().f;

        pq.pop();
        if(n->h==0){
           // cout<<"successful---------------------------------"<<endl;
            node* temp=n;
            cout<<"Minimum number of moves = "<<n->depth<<endl;
            RecursivePrinter(temp,row);
            break;
        }else{
            for(int i=0;i<row;i++)
                for(int j=0;j<row;j++)
                    if(n->state[i][j]==0){
                        zr=i;
                        zc=j;
                    }
                
            //left
            if(zc>0){
                node n2(row,n,n->depth+1,n->state);
                n2.state[zr][zc]=n2.state[zr][zc-1];
                n2.state[zr][zc-1]=0;
                n2.h=hamming(n2.state,dest,row);
                n2.f=n2.h+n2.depth;
                string ss=storage(n2,row);
                auto it = closedList.find(ss);
                if (it == closedList.end()) {
                    pq.push(n2);
                    closedList.emplace(ss,1);
                }
            }
            //right
            if(zc<(row-1)){
                node n2(row,n,n->depth+1,n->state);                                      
                n2.state[zr][zc]=n2.state[zr][zc+1];   
                n2.state[zr][zc+1]=0;                 
                n2.h=hamming(n2.state,dest,row);      
                n2.f=n2.h+n2.depth;                     
                string ss=storage(n2,row);
                auto it = closedList.find(ss);
                if (it == closedList.end()) {
                    pq.push(n2);
                    closedList.emplace(ss,1);
                }      

            }
            //top
            if(zr>0){                
                node n2(row,n,n->depth+1,n->state);
                n2.state[zr][zc]=n2.state[zr-1][zc];
                n2.state[zr-1][zc]=0;
                n2.h=hamming(n2.state,dest,row);
                n2.f=n2.h+n2.depth;              
                string ss=storage(n2,row);
                auto it = closedList.find(ss);
                if (it == closedList.end()) {
                    pq.push(n2);
                    closedList.emplace(ss,1);
                }
            }
            //bottom
            if(zr<(row-1)){
                node n2(row,n,n->depth+1,n->state);
                n2.state[zr][zc]=n2.state[zr+1][zc];
                n2.state[zr+1][zc]=0;
                n2.h=hamming(n2.state,dest,row);
                n2.f=n2.h+n2.depth;
                string ss=storage(n2,row);
                auto it = closedList.find(ss);
                if (it == closedList.end()) {
                    pq.push(n2);
                    closedList.emplace(ss,1);
                }
            } 
        }
    }
    //cout<<"astar end"<<endl;
}

int main() {
    int row;
    int cnt=0;

    cin>>row;
                                                 
    int **puzzle=new int*[row];
    for(int i=0;i<row;i++){
        puzzle[i]=new int[row];
    }

    int **dest=new int*[row];
    for(int i=0;i<row;i++){
        dest[i]=new int[row];
    }

    for(int i=0;i<row;i++)
        for(int j=0;j<row;j++)
            cin>>puzzle[i][j];

    for(int i=0;i<row;i++)
        for(int j=0;j<row;j++){
            cnt++;
            if(i==(row-1)&&j==(row-1))
                dest[i][j]=0;
            else
                dest[i][j]=cnt;
        }
            

    if(solvable(puzzle,row)){
    cout<<"solvable"<<endl;
       cout<<endl;
       cout<<"manhattan:"<<endl;
       AstarMan(puzzle,dest,row);
       cout<<endl;
       cout<<"hamming:"<<endl;
       AstarHam(puzzle,dest,row);
    }   
    else
        cout<<"unsolvable"<<endl;
    

 	return 0;
};

