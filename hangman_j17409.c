#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>
//#include <termio.h>

#define WORD_FILE "/j4kadai/toeic1500.dat"
#define REST 7
#define getChar() getch()

typedef struct {
    int word_num;
    char target[15];
    char japanese[50];
}t_word;

t_word data[1501];
int rest=REST;
char moji[REST];
char incollect[10][15];

int getWord(); //問題の単語取得
int word_judge(int); //入力された文字が含まれているか、正解かの判定
int enter_word(int dataNum); //文字の入力
void used_word(); //使われた文字の表示
int lose_judge(int dataNum,int play_count); //不正解の判定とその場合の処理
int incollect_print(int count); //不正解単語の表示
void rate_print(int play_cnt,int incollect_cnt); //正解率の表示
//char getChar();

int main(void)
{
    int num;
    int i;
    char yn;
    int play_count=0;
    int incollect_cnt=0;

    while(1){
        if(play_count==10){
            printf("10回プレイしたので終了します\n");
            incollect_cnt=incollect_print(play_count);
            rate_print(play_count,incollect_cnt);
            return 0;
        }
        i=0;
        rest=REST; //初期化
        memset(moji,'\0',sizeof(moji)); //使われた文字の初期化
        num=getWord();
        while(i!=REST){
            if(word_judge(num)){ //正解した場合、終了する
                play_count++; 
                break;
            }
            used_word();
            printf("残り回数:%d\n",rest);
            if(enter_word(num)){ //最後の入力単語が正解で終了
                play_count++;
                break;
            }
            if(lose_judge(num,play_count)){ //残り0で終了
                play_count++;
                break;
            }
            i++;
        }

        while(1){
            printf("続けますか(y/n)?:");
            yn=getch();
            printf("%c\n\n",yn);
            if(yn=='y' || yn=='n') break; 
        }
        if(yn=='n'){
            incollect_cnt=incollect_print(play_count);
            rate_print(play_count,incollect_cnt);
            printf("\nBye\n");
            break;
        }
    }

    return 0;
}

int getWord()
{
    FILE *fp;
    int num,i=1;

    if((fp=fopen(WORD_FILE,"r"))==NULL){
        fprintf(stderr,"Error:ファイルがオープンできません:%s\n",WORD_FILE);
        exit(1);
    }
    while(!feof(fp)){
        fscanf(fp,"%d %s",&data[i].word_num,data[i].target);
        fgets(data[i].japanese,100,fp);
        //printf("%d %s %s",data[i].word_num,data[i].target,data[i].japanese);
        i++;
    }
    while(1){
        srand((unsigned int)time(NULL));
        num=rand()%1500+1;
        if(strlen(data[num].target)>=5) break;
    }

    //printf("\nword:%s\n",data[num].target);
    fclose(fp);

    return num;
}

int word_judge(int num)
{
    int i,j,len,cnt=0;
    bool flag=0; //その文字があったかの判定
    int locate=abs(rest+1-REST); //何文字まで入力されているか
    len=strlen(data[num].target);
    printf("単語:");
    if(rest==REST){
        for(i=0;i<len;i++) printf("-");
    }
    else {
        for(i=0;i<len;i++){
            flag=0;
            for(j=0;j<=locate;j++){
                if(data[num].target[i]==moji[j]){
                    printf("%c",moji[j]);
                    flag=1;
                    cnt++;
                    if(cnt==len){
                        printf("\nYou Win!\n");
                        return 1;
                    }
                }
            }
            if(flag==0) printf("-");
        }
    }
    printf("\n");
    return 0;
}

void used_word()
{
    int i;
    int num=abs(rest-REST);
    printf("使われた文字:");
    if(rest!=REST){
        for(i=0;i<=num;i++){
            printf("%c ",moji[i]);
        }
    }
    printf("\n");
}

int enter_word(int dataNum)
{
    int i,flag; //過去に使われたかのフラグ
    int num=abs(rest-REST);
    while(1){
        flag=0;
        printf("文字を入力してください:");
        moji[num]=getch();
        printf("%c\n",moji[num]);
        for(i=0;i<num;i++){
            if(moji[i]==moji[num]) flag=1;
        }
        if(flag==0 && islower(moji[num])!=0) break; //islower小文字かを判定
    }
    printf("\n");
    rest--;
    if(rest==0 && word_judge(dataNum)==1) return 1;
    return 0;
}

/*char getChar()
{
    struct termio old_term, new_term;

    char    c;

    ioctl(0, TCGETA, &old_term);
    new_term = old_term;
    //new_term.c_lflag &= ~(ICANON | ECHO);
    new_term.c_lflag &= ~(ICANON);
    ioctl(0, TCSETAW, &new_term);

    c = getchar();

    ioctl(0, TCSETAW, &old_term);

    return (c);
}*/

int lose_judge(int dataNum,int count)
{
    if(rest==0){
        printf("You lose! answer:%s\n",data[dataNum].target);
        strcpy(incollect[count],data[dataNum].target); //間違えた単語をコピー
        //printf("incollect:%s\n",incollect[count]);
        return 1;
    }
    return 0;
}

int incollect_print(int count)
{
    int i,incollect_cnt=0;
    printf("incollect\n");
    printf("----------\n");
    for(i=0;i<=count;i++){
        if(incollect[i][0]!='\0'){
            incollect_cnt++;
            printf("%s\n",incollect[i]);
        }
    }
    return incollect_cnt;
}

void rate_print(int play_cnt,int incollect_cnt)
{
    //printf("play_cnt:%d,incollect:%d\n",play_cnt,incollect_cnt);
    int collect=play_cnt-incollect_cnt;
    if(incollect_cnt==0) printf("\ncollect:%d / incollect:%d / rate:%f",collect,incollect_cnt,(float)collect);
    else printf("\ncollect:%d / incollect:%d / rate:%f",collect,incollect_cnt,(float)collect/incollect_cnt);
}