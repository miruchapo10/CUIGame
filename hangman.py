path="/j4kadai/hangman/toeic1500.dat"
import random
import msvcrt
with open(path,'r',encoding="EUC-JP") as f:
    lines=f.readlines()

rest=7
REST=7

def word_judge():
    word_len=len(word)
    locate=abs(rest+1-REST)
    cnt=0
    print("単語:",end="")
    if rest==REST:
        j=0
        while j<word_len:
            print("-",end="")
            j+=1
    else :
        j=0
        while j<word_len:
            flag=0
            k=0
            while k<=locate:
                if word[j]==str_list[k]:
                    print(str_list[k],end="")
                    flag=1
                    cnt+=1
                    if cnt==word_len:
                        print("\nYou Win")
                        return 1
                k+=1
            if flag==0:
                print("-",end="")
            j+=1
    print()
    return 0

def used_word():
    print("使われた文字:",end="")
    for x in str_list:
        print(x," ",end="")
    print()

def enter_word():
    while 1:
        flag=0
        print("文字を入力してください:",end="")
        box=msvcrt.getwch()
        print(box,"\n")
        if (box in str_list)==True:
            flag=1
        if flag==0 and box.islower()==True:
            str_list.append(box)
            break

while 1:
    str_list=[] #入力した文字
    word_num=random.randint(1,1500)
    l=lines[word_num-1].split()  #問題の単語
    if len(l[1])<5:
        continue
    #print("word:",l[1]) #[0]:問題番号 [1]:英単語 [2]:意味
    word=l[1]
    rest=REST
    i=0
    while i!=REST:
        if word_judge():
            break
        used_word()
        print("残り回数:",rest)
        enter_word()
        rest-=1
        if rest==0:
            if word_judge()==1:
                break
            else :
                print("You lose")
                break
        i+=1

    while 1:
        print("続けますか(y/n):",end="")
        yn=input()
        if yn=='y' or yn=='n':
            break

    if yn=='n':
        break
