#include<bits/stdc++.h>
#include<iostream>
#include<time.h> 

using namespace std;

const int MAX = 1000;
const int MIN = -1000;

//!need variable depth for both player
//!need variable heuristic for both player

class Board{
    public:
    int* p1;
    int* p2;
    int m1;
    int m2;
    bool player1;
    int additonal;
    int capture;
    Board(){
      p1=new int[6]{4,4,4,4,4,4};
      p2=new int[6]{4,4,4,4,4,4};
      m1=0;
      m2=0; 
      player1=true; 
      additonal=0;
      capture=0;
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
        this->additonal=board->additonal;
        this->capture=board->capture;
       
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
        this->additonal=board->additonal;
        this->capture=board->capture;
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

    void flush(){
        p1=new int[6]{4,4,4,4,4,4};
        p2=new int[6]{4,4,4,4,4,4};
        m1=0;
        m2=0; 
        player1=true; 
        additonal=0;
        capture=0;
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
    srand(time(0));
	W1 = rand() % 8 + 1;
    srand(time(0));
	W2 = rand() % 10 + 1;	
    return W1*(board->m1-board->m2)+W2*(board->p1_stone()-board->p2_stone());
}

int Heuristic3(Board* board){
    int W1=1;
    int W2=1;
    int W3=1;
    srand(time(0));
	W1 = rand() % 10 + 1;
    srand(time(0));
	W2 = rand() % 10 + 1;
    srand(time(0));
	W3 = rand() % 10 + 1;
    return W1*(board->m1-board->m2)+W2*(board->p1_stone()-board->p2_stone())+W3*board->additonal;
}

int Heuristic4(Board* board){
    int W1=1;
    int W2=1;
    int W3=1;
    int W4=1;
    srand(time(0));
	W1 = rand() % 10 + 1;
    srand(time(0));
	W2 = rand() % 10 + 1;
    srand(time(0));
	W3 = rand() % 10 + 1;
    srand(time(0));
	W4 = rand() % 10 + 1;
    return W1*(board->m1-board->m2)+W2*(board->p1_stone()-board->p2_stone())+W3*board->additonal+W4*board->capture;
}

int Heuristic5(Board* board){
    int W1=1;
    int W2=1;
    srand(time(0));
	W1 = rand() % 10 + 1;
    srand(time(0));
	W2 = rand() % 10 + 1;
    return W1*board->additonal+W2*board->capture;
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
                if(take==0){
                    repeat=true;
                    board->additonal++;
                }
                    
            }
            while(true){
                for(;i<6;i++){
                    if(board->p1[i]==0 && take==1 && board->p2[5-i]!=0){
                        board->m1=board->m1+1+board->p2[5-i];
                        board->capture=board->capture+board->p2[5-i];
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
                        board->additonal++;
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
                if(take==0){
                    repeat=true;
                    //board->additonal++;
                }
                    
            }
            while(true){
                for(;i<6;i++){
                    if(board->p2[i]==0 && take==1 && board->p1[5-i]!=0){
                        board->m2=board->m2+1+board->p1[5-i];
                        //board->capture=board->capture+board->p1[5-i];
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
                        //board->additonal++;
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

int minimax(int depth,bool maximizingPlayer, int alpha,int beta,Board* board,int h){
    int move=0;
    if (depth == 11 || board->isOver()){
        
        if(h==1){
            return Heuristic1(board);
        }
        else if(h==2){
            return Heuristic2(board);
        }
        else if(h==3){
            return Heuristic3(board);
        }
        else if(h==4){
            return Heuristic4(board);
        }
        else{
            return Heuristic5(board);
        }

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
            int val = minimax(depth + 1,boardChild->player1,alpha,beta,boardChild,h);
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
            
            int val = minimax(depth + 1,boardChild->player1,alpha,beta,boardChild,h);
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

void GameAI(Board* board,int h){
    int move=minimax(0,board->player1,MIN,MAX,board,h);
    Game(board,move);
}

int main(){

    int n;
    Board* board=new Board();
    int h=1;
    cout<<"which heuristic you want to select:"<<endl;
    cin>>h;
    if(h>5||h<1)
        h=1;
        
//&--AI/Human_vs_AI/Human-------------------------------------------------
    
    // int p1_score=0;
    // int p2_score=0;

    // Print(board);

    // while(true){
    //     if(board->isOver())
    //         break;
    //     if(board->player1){
    //         //Player-1---Human------------------------------------------------
    //         // cout<<"Player 1:"<<endl;
    //         // while(true){
    //         // cin>>n;
    //         //     if(n>6||n<1)
    //         //         cout<<"give a number between 1 to 6"<<endl;
    //         //     else if(board->p1[n-1]==0)
    //         //         cout<<"give a number of a pot that is not empty"<<endl;
    //         //     else
    //         //         break;
    //         // }
    //         // Game(board,n);
    //         //Player-1---AI--------------------------------------------------
    //         //GameAI(board,h);
    //     }else{
    //         //Player-2---Human------------------------------------------------
    //         // cout<<"Player 2:"<<endl;
    //         // while(true){
    //         // cin>>n;
    //         //     if(n>6||n<1)
    //         //         cout<<"give a number between 1 to 6"<<endl;
    //         //     else if(board->p2[n-1]==0)
    //         //         cout<<"give a number of a pot that is not empty"<<endl;
    //         //     else
    //         //         break;
    //         // }
    //         //Player-2---AI--------------------------------------------------
    //         // Game(board,n);
    //         GameAI(board,h);
    //     }
        
    //     Print(board);
    // }
    
    // cout<<"game over"<<endl;
    // if(board->m1>board->m2)
    //     cout<<"Player 1 wins!"<<endl;
    // else if(board->m1<board->m2)
    //     cout<<"Player 2 wins!"<<endl;
    // else
    //     cout<<"Draw"<<endl;
    // cout<<"score:"<<endl;
    // cout<<"---------------------------"<<endl;
    // cout<<"player 1:"<<board->m1<<endl;
    // cout<<"player 2:"<<board->m2<<endl;

//&---AI_vs_AI-Multiple_instances-----------------------------------------------

int p1=0;
int p2=0;
int p3=0;
for(int i=0;i<3;i++){
    board->flush();
        while(true){
        if(board->isOver())
            break;
        if(board->player1){
            GameAI(board,h);
        }else{
            GameAI(board,h);
        } 
    }
    if(board->m1>board->m2)
        p1++;
    else if(board->m1<board->m2)
        p2++;
    else
        p3++;
}
cout<<"p1 wins: "<<p1<<" times"<<endl;
cout<<"p2 wins: "<<p2<<" times"<<endl;
cout<<"Draw: "<<p3<<" times"<<endl;


    return 0;
}