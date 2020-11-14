import random
import time

def print_banmen():
    for i in range(0,3):
        for j in range(0,3):
            print(banmen[i][j]," ",end="")
        print()
    print("player:o computer:x")
    print()

def index_search(num):
    if num<4:
        i=0
    elif num<7:
        i=1
    else:
        i=2

    if num%3==1:
        j=0
    elif num%3==2:
        j=1
    else:
        j=2
    return i,j

def judge():
    for i in range(0,3):
        pl_cnt=0
        com_cnt=0
        for j in range(0,3):
            if banmen[i][j]=='o':
                pl_cnt+=1
            elif banmen[i][j]=='x':
                com_cnt+=1
        if pl_cnt==3:
            return 1
        if com_cnt==3:
            return 2

    for j in range(0,3):
        pl_cnt=0
        com_cnt=0
        for i in range(0,3):
            if banmen[i][j]=='o':
                pl_cnt+=1
            elif banmen[i][j]=='x':
                com_cnt+=1
        if pl_cnt==3:
            return 1
        if com_cnt==3:
            return 2
    
    diagonal_pl=0
    diagonal_com=0
    for i in range(3):
        if banmen[i][i]=='o':
            diagonal_pl+=1
        elif banmen[i][i]=='x':
            diagonal_com+=1
    if diagonal_pl==3:
        return 1
    if diagonal_com==3:
        return 2
    
    diagonal_pl=0
    diagonal_com=0
    for i in range(0,3):
        for j in range(2,-1):
            if banmen[i][i]=='o':
                diagonal_pl+=1
            elif banmen[i][i]=='x':
                diagonal_com+=1
    if diagonal_pl==3:
        return 1
    if diagonal_com==3:
        return 2

    return  0

def print_result(result):
    if result==1:
        print("You Win!")
    if result==2:
        print_banmen()
        print("You Lose...")
    

banmen=[[1,2,3],[4,5,6],[7,8,9]]
play_cnt=0
while True:
    print_banmen()
    while True:
        pl_num=input("1~9の数字を入力してください:")
        if not(pl_num.isdecimal()):
            continue
        pl_num=int(pl_num)
        if pl_num<10 and pl_num>0:
            pl_index=index_search(pl_num)
            if banmen[pl_index[0]][pl_index[1]]!='o' and banmen[pl_index[0]][pl_index[1]]!='x':
                break
    banmen[pl_index[0]][pl_index[1]]='o'
    play_cnt+=1
    print_banmen()
    if play_cnt>2:
        result=judge()
        if result>0:
            print_result(result)
            break
    if play_cnt>4:
        print("Draw")
        break
    print("相手のターンです...")
    time.sleep(0.7)

    while True:
        i_cnt=0
        com_i=random.randint(0,2)
        for j in range(0,3):
            if banmen[com_i][j]=='o' or banmen[com_i][j]=='x':
                i_cnt+=1
        if i_cnt<3:
            break

    while True:
        com_j=random.randint(0,2)
        #print("com_j:",com_j)
        if banmen[com_i][com_j]!='o' and banmen[com_i][com_j]!='x':
            break

    banmen[com_i][com_j]='x'
    if play_cnt>2:
        result=judge()
        if result>0:
            print_result(result)
            break