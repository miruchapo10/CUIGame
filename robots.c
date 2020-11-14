#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#ifdef __unix
extern char getChar(void);
#define getch() getChar()
#else
#include <conio.h>
#endif

#define X 60
#define Y 20
#define min(x,y) (x>y)?y:x
#define rand_range(min,max) (min)+(rand())%((max)-(min)+(1))

typedef struct info {
	int x;
	int y;
	bool alive_flag;
}info_t;

void reset_field(int field[Y + 5][X + 5]);
info_t* make_robots(int field[Y + 5][X + 5], int robot_num);
void teleport(int field[Y + 5][X + 5], info_t* human);
void field_print(int field[Y + 5][X + 5]);
int move_human(int field[Y + 5][X + 5], info_t* human);
void move_robots(int field[Y + 5][X + 5], info_t* human, info_t* robo, int robo_num);
int judge(int field[Y + 5][X + 5], info_t* human, info_t* robo, int robo_num);
int score_calc(info_t* robo,int robo_num,int level);

int main(void)
{
	int level = 1;
	int score=0,tmp_score=0;
	bool move_flag = true; //これ以降動くかのフラグ
	int robo_num = 0;
	int judge_result;
	int field[Y + 5][X + 5];
	FILE* fp;
	fp = fopen("result.txt", "a");
	info_t* robo, * human;
	human = (info_t*)malloc(sizeof(info_t));

	while (1) {
		move_flag = true;
		reset_field(field);
		robo_num = min(level * 5, 40);
		robo = make_robots(field, robo_num);
		teleport(field, human);
		field_print(field);

		while (1) {
			if (move_flag == true) {
				if (move_human(field, human)) move_flag = false;
			}
			move_robots(field, human, robo, robo_num);
			judge_result = judge(field, human, robo, robo_num);
			score=score_calc(robo,robo_num,level);
			if (judge_result == 1){ //ゲームオーバーの場合
				fprintf(fp, "%d", score);
				fputc('\n', fp);
				printf("score:%d\n", score);
				break;
			}
			field_print(field);
			printf("score:%d\n", score);
			if (judge_result == 2) { //レベルクリアした場合
				score+=level*10;
				printf("score:%d\n",score);
				printf("level%d Clear!\n\n", level);
				level++;
				break;
			}
		}
		if (judge_result == 1) break;
	}

	free(robo);
	free(human);
	fclose(fp);
	return 0;
}

void reset_field(int field[Y + 5][X + 5])
{
	int i, j;
	for (i = 1; i < Y + 1; i++) {
		for (j = 1; j < X + 1; j++) {
			field[i][j] = 0;
		}
	}
}

info_t* make_robots(int field[Y + 5][X + 5], int robot_num) {
	int i;
	info_t* robo;
	robo = (info_t*)malloc(sizeof(info_t) * robot_num);
	srand((unsigned)time(NULL));
	for (i = 0; i < robot_num; i++) {
		robo[i].y = rand_range(1, Y);
		robo[i].x = rand_range(1, X);
		robo[i].alive_flag = true;
		field[robo[i].y][robo[i].x] = 1;
	}
	return robo;
}

void teleport(int field[Y + 5][X + 5], info_t* human)
{
	while (1) {
		srand((unsigned)time(NULL));
		human->y = rand_range(1, Y);
		human->x = rand_range(1, X);
		human->alive_flag = true;
		if (field[human->y][human->x] == 0) {
			field[human->y][human->x] = 2;
			break;
		}
	}
}

void field_print(int field[Y + 5][X + 5])
{
	int i, j;
	for (i = 0; i < Y + 2; i++) {
		for (j = 0; j < X + 2; j++) {
			if (i == 0 || i == Y + 1) printf("-");
			else if (j == 0 || j == X + 1) printf("|");
			else if (field[i][j] == 1) printf("+"); //ロボット
			else if (field[i][j] == 2) printf("@"); //プレイヤー
			else if (field[i][j] == 3) printf("*"); //スクラップ
			else printf(" ");
		}
		printf("\n");
	}
}

int move_human(int field[Y + 5][X + 5], info_t* human)
{
	char c;
	int tmp_x, tmp_y;
	field[human->y][human->x] = 0;

	while (1) {
		tmp_x = human->x;
		tmp_y = human->y;
		printf("Press the key(0~9),'p':これ以降動かない:");
		c = getch();
		printf("%c\n", c);
		if (c == 'p') { //これ以降動かない
			field[human->y][human->x] = 2;
			return 1;
		}
		if (isdigit(c)) {
			switch (c) {
			case '0':
				teleport(field, human);
				break;

			case '1':
				tmp_x--;
				tmp_y++;
				break;

			case '2':
				tmp_y++;
				break;

			case '3':
				tmp_x++;
				tmp_y++;
				break;

			case '4':
				tmp_x--;
				break;

			case '6':
				tmp_x++;
				break;

			case '7':
				tmp_x--;
				tmp_y--;
				break;

			case '8':
				tmp_y--;
				break;

			case '9':
				tmp_x++;
				tmp_y--;
				break;

			default:
				break;
			}
			if (tmp_y > 0 && tmp_y <= Y && tmp_x > 0 && tmp_x <= X) break;
		}
	}
	if (c != '0') {
		human->x = tmp_x;
		human->y = tmp_y;
		field[human->y][human->x] = 2;
	}

	return 0;
}

void move_robots(int field[Y + 5][X + 5], info_t* human, info_t* robo, int robo_num)
{
	int i;
	for (i = 0; i < robo_num; i++) {
		if (robo[i].alive_flag == false) continue;
		field[robo[i].y][robo[i].x] = 0;
		if (human->x > robo[i].x) robo[i].x++;
		else if (human->x < robo[i].x) robo[i].x--;
		if (human->y > robo[i].y) robo[i].y++;
		else if (human->y < robo[i].y) robo[i].y--;
	}
	
}

int judge(int field[Y + 5][X + 5], info_t* human, info_t* robo, int robo_num)
{
	int i, j, cnt = 0;
	for (i=0;i<robo_num;i++) {
		if (robo[i].alive_flag == false) continue;
		if (robo[i].x == human->x && robo[i].y == human->y) {
			printf("\nGame Over\n");
			return 1;
		}
	}

	for(i=0;i<robo_num;i++){
		for(j=i+1;j<robo_num;j++){
			if (robo[i].x == robo[j].x && robo[i].y == robo[j].y) {
				robo[i].alive_flag = robo[j].alive_flag = false;
			}
		}
		if (robo[i].alive_flag == false)field[robo[i].y][robo[i].x] = 3,cnt++;
		else field[robo[i].y][robo[i].x] = 1;
  	}
	return (cnt == robo_num) ? 2 : 0;
}

int score_calc(info_t* robo,int robo_num,int level)
{	
	int i,score=0;
	for(i=0;i<robo_num;i++){
		if(robo[i].alive_flag==false) score++;
	}
	if(level>1){
		for(i=2;i<=level;i++){
			score += min((i-1)*5,40);
			score += (i-1) * 10;
		}
	}
	return score;
}
