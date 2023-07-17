#include<bits/stdc++.h>
#include<iostream>

using namespace std;

const int MAX = 1000;
const int MIN = -1000;

class Board{
    public:
    int* p1;
    int* p2;
    int m1;
    int m2;
    bool player1;
    Board(){
      p1=new int[6]{4,4,4,4,4,4};
      p2=new int[6]{4,4,4,4,4,4};
      m1=0;
      m2=0; 
      player1=true; 
    }
    Board(int m1,int m2,int* p1,int* p2,bool player1){
        p1=new int[6];
        p2=new int[6];
        for(int i=0;i<6;i++){
            this->p1[i]=p1[i];
            this->p2[i]=p2[i];
        }
        this->m1=m1;
        this->m2=m2;
        this->player1=player1;
    }

    Board(Board* board){
        p1=new int[6];
        p2=new int[6];
        for(int i=0;i<6;i++){
            this->p1[i]=board->p1[i];
            this->p2[i]=board->p2[i];
        } 
        this->m1=board->m1;
        this->m2=board->m2;
        this->player1=board->player1;
       
    }

    bool isOver(){
        if(p1_stone()==0){
            m2=48-m1;
            for(int i=0;i<6;i++)
                p2[i]=0;
        }
        if(p2_stone()==0){
            m1=48-m2;
            for(int i=0;i<6;i++)
                p1[i]=0;
        }
        if((m1+m2)==48)
            return true;
        else
            return false;
    }

    void setBoard(Board* board){
       for(int i=0;i<6;i++){
            this->p1[i]=board->p1[i];
            this->p2[i]=board->p2[i];
        }
        this->m1=board->m1;
        this->m2=board->m2;
        this->player1=board->player1; 
    }

    int p1_stone(){
        return p1[0]+p1[1]+p1[2]+p1[3]+p1[4]+p1[5];
    }

    int p2_stone(){
        return p2[0]+p2[1]+p2[2]+p2[3]+p2[4]+p2[5];
    }
    ~Board(){
        delete[] p1;
        delete[] p2;
    }
};

void Print(Board *board){
    cout<<"---";
    cout<<board->p2[5]<<" "<<board->p2[4]<<" "<<board->p2[3]<<" "<<board->p2[2]<<" "<<board->p2[1]<<" "<<board->p2[0];
    cout<<"---"<<endl;
    cout<<board->m2<<"---------------"<<board->m1<<endl;
    cout<<"---";
    cout<<board->p1[0]<<" "<<board->p1[1]<<" "<<board->p1[2]<<" "<<board->p1[3]<<" "<<board->p1[4]<<" "<<board->p1[5];
    cout<<"---"<<endl;
}

int Heuristic1(Board* board){
    return board->m1-board->m2;
}

int Heuristic2(Board* board){
    int W1=1;
    int W2=1;
    return W1*(board->m1-board->m2)+W2*(board->p1_stone()-board->p2_stone());
}

int Heuristic3(Board* board,int additional){
    int W1=1;
    int W2=1;
    int W3=1;
    return W1*(board->m1-board->m2)+W2*(board->p1_stone()-board->p2_stone())+W3*additional;
}

int Heuristic4(Board* board,int additional,int captured){
    int W1=1;
    int W2=1;
    int W3=1;
    int W4=1;
    return W1*(board->m1-board->m2)+W2*(board->p1_stone()-board->p2_stone())+W3*additional+W4*captured;
}

void Game(Board* board,int n){

        if(board->player1){
            int take=board->p1[n-1];
            board->p1[n-1]=0;
            int i=n;
            bool repeat=false;
            if(i==6 && take>0){
                board->m1++;
                take--;
                if(take==0)
                    repeat=true;
            }
            while(true){
                for(;i<6;i++){
                    if(board->p1[i]==0 && take==1 && board->p2[5-i]!=0){
                        board->m1=board->m1+1+board->p2[5-i];
                        board->p2[5-i]=0;
                        take--;
                    }
                    if(take>0){
                        board->p1[i]++;
                        take--;
                    } 
                    if(i==5 and take==1){
                        board->m1++;
                        repeat=true;
                        take--;//give second chance to current player
                    }
                    if(i==5 and take>1){
                       board->m1++; 
                       take--;
                    }
                    if(take==0)
                        break;//break from current player
                }
                if(take==0)
                        break;//break from current player 
                for(i=0;i<6;i++){
                    board->p2[i]++;
                    take--;
                    if(take==0)
                        break;//break from current player
                }
                if(take==0)
                        break;//break from current player
                i=0;
            }
            if(repeat)//another chance
                board->player1=true;
            else
                board->player1=false;     
        }
        else{
            int take=board->p2[n-1];
            board->p2[n-1]=0;
            int i=n;
            bool repeat=false;
            if(i==6 && take>0){
                board->m2++;
                take--;
                if(take==0)
                    repeat=true;
            }
            while(true){
                for(;i<6;i++){
                    if(board->p2[i]==0 && take==1 && board->p1[5-i]!=0){
                        board->m2=board->m2+1+board->p1[5-i];
                        board->p1[5-i]=0;
                        take--;
                    }
                    if(take>0){
                        board->p2[i]++;
                        take--;
                    }
                    if(i==5 and take==1){
                        board->m2++;
                        repeat=true;
                        take--;//give second chance to current player
                    }
                    if(i==5 and take>1){
                       board->m2++; 
                       take--;
                    }
                    if(take==0)
                        break;//break from current player
                }
                if(take==0)
                        break;//break from current player 
                for(i=0;i<6;i++){
                    board->p1[i]++;
                    take--;
                    if(take==0)
                        break;//break from current player
                }
                if(take==0)
                        break;//break from current player
                i=0;
            }
            if(repeat)//another chance
                board->player1=false;
            else
                board->player1=true;            
              
        }   
}

int minimax(int depth,bool maximizingPlayer, int alpha,int beta,Board* board){
    int move=0;
    if (depth == 11 || board->isOver()){
        return Heuristic1(board);
    }
    if (maximizingPlayer)
    {   
        int best=MIN;
        for (int i = 0; i < 6; i++)
        {
            Board* boardChild=new Board(board);
             
            if(boardChild->p1[i]==0){
                delete boardChild;
                continue;
            }
            Game(boardChild,i+1);
            int val = minimax(depth + 1,boardChild->player1,alpha,beta,boardChild);
            if(best<val)
                move=i+1;
            best = max(best,val);
            alpha = max(alpha,best);
            if (beta<=alpha)
                break;
        }
        if(depth==0)
            return move;
        else
            return best;
    }
    else
    {
        int best=MAX;
        for (int i = 0; i < 6; i++)
        {   
            Board* boardChild=new Board(board);
            
           
            if(boardChild->p2[i]==0){
                delete boardChild;
                continue;
            }
            Game(boardChild,i+1);
            
            int val = minimax(depth + 1,boardChild->player1,alpha,beta,boardChild);
            if(best>val)
                move=i+1;
            best = min(best, val);
            beta = min(beta, best);
            if (beta <= alpha)
                break;
        }
        if(depth==0)
            return move;
        else
            return best;
    }
}

void GameAI(Board* board){
    int move=minimax(0,board->player1,MIN,MAX,board);
    Game(board,move);
}

int main(){

    int n;
    Board* board=new Board();
        
   
    int p1_score=0;
    int p2_score=0;

    Print(board);

    while(true){
        if(board->isOver())
            break;
        if(board->player1){
            // cout<<"Player 1:"<<endl;
            // while(true){
            // cin>>n;
            //     if(n>6||n<1)
            //         cout<<"give a number between 1 to 6"<<endl;
            //     else if(board->p1[n-1]==0)
            //         cout<<"give a number of a pot that is not empty"<<endl;
            //     else
            //         break;
            // }
            // Game(board,n);
            GameAI(board);
        }else{
            
            // cout<<"Player 2:"<<endl;
            // while(true){
            // cin>>n;
            //     if(n>6||n<1)
            //         cout<<"give a number between 1 to 6"<<endl;
            //     else if(board->p2[n-1]==0)
            //         cout<<"give a number of a pot that is not empty"<<endl;
            //     else
            //         break;
            // }
            // Game(board,n);
            GameAI(board);
        }
        
        Print(board);
    }
    cout<<"game over"<<endl;
    if(board->m1>board->m2)
        cout<<"Player 1 wins!"<<endl;
    else if(board->m1<board->m2)
        cout<<"Player 2 wins!"<<endl;
    else
        cout<<"Draw"<<endl;
    cout<<"score:"<<endl;
    cout<<"---------------------------"<<endl;
    cout<<"player 1:"<<board->m1<<endl;
    cout<<"player 2:"<<board->m2<<endl;

    return 0;
}