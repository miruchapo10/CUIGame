#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>

typedef struct{
    bool mine_flag;
    bool open_flag;
    int around_mine;
    bool put_mflag;
}particle;

int enter_scale(int scale[2]); //大きさとMの個数入力
void board_reset(particle **board,int scale[2]); //ボードリセット
void make_ms(particle **board,int scale[2],int m); //m生成
void calc_around(particle **board,int scale[2]); //周りのMの個数
void board_print(particle **board,int scale[2]); //盤面を出力
int enter_xy(particle **board,int xy[2],int scale[2]); //座標入力
int judge(particle **board,int scale[2],int xy[2]); //勝ち負け判定
void time_print(int measureTime); //タイムを出力
void put_mark(particle **board,int xy[2]); //Mを置く
void search(particle **board,int line,int row,int x,int y); //1以上を壁として開ける
void answer(particle **board,int scale[2]); //間違えた場合、盤面を出力

int main()
{
    time_t start_time,end_time;
    particle *base,**board;
    int scale[2],xy[2];
    int i,mode,m;

    m=enter_scale(scale);
    base=malloc(sizeof(particle) * scale[0] * scale[1]);
    board=malloc(sizeof(particle *)*scale[0]);
    for(i=0;i<scale[0];i++) board[i]=base+i*scale[0];
    
    board_reset(board,scale);
    make_ms(board,scale,m);
    calc_around(board,scale);
    
    start_time=time(NULL);
    while(1){
        board_print(board,scale);
        mode=enter_xy(board,xy,scale);
        if(mode==1) put_mark(board,xy);
        else{
            if(judge(board,scale,xy)) break;
        }
    }
    end_time=time(NULL);
    time_print(end_time-start_time);

    free(base);
    free(board);

    return 0;
}

int enter_scale(int scale[2])
{
    int line,row;
    int m;

    while(1){
        printf("line row->");
        scanf("%d %d",&line,&row);
        if(line>0 && row>0 && line<20 && row<20) break;
    }
    getchar();
    scale[0]=line;
    scale[1]=row;

    while(1){
        printf("Mの個数を入力してください:");
        scanf("%d",&m);
        if(m>0 && scale[0]*scale[1] > m) break;
    }
    return m;
}

void board_reset(particle **board,int scale[2])
{
    int i,j;
    for(i=0;i<scale[0];i++){
        for(j=0;j<scale[1];j++){
            board[i][j].mine_flag=false;
            board[i][j].open_flag=false;
            board[i][j].around_mine=0;
            board[i][j].put_mflag=false;
        }
    }
}

void make_ms(particle **board,int scale[2],int m)
{
    int x,y;
    int i;

    srand((unsigned)time(NULL));
    for(i=0;i<m;i++){
        x=rand()%scale[0];
        y=rand()%scale[1];
        if(board[x][y].mine_flag==true){
            i--;
            continue;
        }
        board[x][y].mine_flag=true;
    }
}

void calc_around(particle **board,int scale[2])
{
    int i,j,k,l;
    int m_cnt=0;
    for(i=0;i<scale[0];i++){
        for(j=0;j<scale[1];j++){
            if(board[i][j].mine_flag==true) continue;
            m_cnt=0;
            for(k=i-1;k<=i+1;k++){
                if(k<0 || k==scale[0]) continue;
                for(l=j-1;l<=j+1;l++){
                    if(l<0 || l==scale[1]) continue;
                    if(k==i && l==j) continue;
                    if(board[k][l].mine_flag==true) m_cnt++;
                }
            }
            board[i][j].around_mine=m_cnt;
        }
    }
}

void board_print(particle **board,int scale[2])
{
    int i,j;
    for(i=0;i<scale[1];i++){
        if(i==0) printf("  %d",i);
        else printf(" %d",i);
    }
    printf("\n");

    for(i=0;i<scale[0];i++){
        printf("%d ",i);
        for(j=0;j<scale[1];j++){
            if(board[i][j].put_mflag==true) printf("M ");
            else if(board[i][j].open_flag==false) printf(". ");
            else printf("%d ",board[i][j].around_mine);
        }
        printf("\n");
    }
}

int enter_xy(particle **board,int xy[2],int scale[2])
{
    int x,y;
    char mode;
    printf("s:開ける m:Mを置く\n");
    printf("x[0-%d] y[0-%d]\n",scale[1]-1,scale[0]-1);
    while(1){
        printf("x y mode[s,m]->");
        scanf("%d %d %c%*c",&x,&y,&mode);
        if(board[y][x].open_flag==true || x>=scale[1] || x<0|| y>=scale[0] || y<0) continue;
        if(mode=='s' || mode=='m') break;
    }
    xy[0]=x;
    xy[1]=y;

    if(mode=='m') return 1;
    return 0;
}

int judge(particle **board,int scale[2],int xy[2])
{
    int i,j;
    int x=xy[0];
    int y=xy[1];
    int line=scale[0];
    int row=scale[1];
    bool flag=false;

    if(board[y][x].mine_flag==true){
        printf("You Lose.\n");
        answer(board,scale);
        return 1;
    }

    search(board,line,row,x,y); //0を自動で開ける
    board[y][x].open_flag=true;

    for(i=0;i<line;i++){
        for(j=0;j<row;j++){
            if(board[i][j].mine_flag==true) continue;
            if(board[i][j].open_flag==false) flag=true; 
        }
    }
    if(flag==false){
        printf("You Win.\n");
        return 2;
    }

    return 0;
}

void put_mark(particle **board,int xy[2])
{
    int x=xy[0];
    int y=xy[1];
    board[y][x].put_mflag=true;
}

void search(particle **board,int line,int row,int x,int y)
{
    if(x<0 || x>=row || y<0 || y>=line || board[y][x].around_mine>=1) return;

    if(board[y][x].open_flag) return;
    board[y][x].open_flag=true;

    search(board,line,row,x+1,y); //右
    search(board,line,row,x-1,y); //左
    search(board,line,row,x,y-1); //上
    search(board,line,row,x,y+1); //下
}

void answer(particle **board,int scale[2])
{
    int i,j;
    printf("----answer----\n");
    for(i=0;i<scale[1];i++){
        if(i==0) printf("  %d",i);
        else printf(" %d",i);
    }
    printf("\n");

    for(i=0;i<scale[0];i++){
        printf("%d ",i);
        for(j=0;j<scale[1];j++){
            if(board[i][j].mine_flag==true) printf("M ");
            else printf("%d ",board[i][j].around_mine);
        }
        printf("\n");
    }
}

void time_print(int measureTime)
{
    if(measureTime<60) printf("time:%dsecond\n",measureTime);
    else printf("time:%dm %ds",measureTime/60,measureTime%60);
}
