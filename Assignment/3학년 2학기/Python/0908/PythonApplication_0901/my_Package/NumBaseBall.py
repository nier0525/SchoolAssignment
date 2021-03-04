import random

def GameSetting():
    global number
    number = ['0', '0', '0']
    index = 0

    while True:
        number[index] = str(random.randint(0, 9))

        for i in range(index):
            if i == index:
                continue

            if number[i] == number[index]:
                index -= 1
                break      

        index += 1
        if index == 3:
            break

    print(number)    

def GameRun():
     global count
     count = 1

     while True:
        print('입력')
        answer = input()
        
        if len(answer) != 3 & answer.isnumeric() == False:
            print('옳지 않은 입력입니다.')            
            continue

        Error = False
        
        for i in range(3):
            for j in range(i):
                if answer[i] == answer[j]:
                    print('같은 수를 2번 이상 입력하였습니다.')
                    Error = True
                    break
            if Error == True:
                break

        if Error == True:
            continue

        if GameAnswer(answer) == True:
            break

        count += 1
        
def GameAnswer(_answer):
    strike = 0
    ball = 0
    out = 0

    for i in range(3):
        nohit = False
        for j in range(3):
            if number[i] == _answer[j]:
                nohit = True
                if i == j:
                    strike += 1
                else:
                    ball += 1
        if nohit == False:
            out += 1

    if strike == 3:        
        print('{0} 횟수만에 맞추었습니다.'.format(count))
        return True
    else:
        print('Strike : {0}, Ball : {1}, Out : {2}'.format(strike, ball, out))        
        return False

print('Beginning of NumBaseBall')
print('name : {0}'.format(__name__))
print('End of NumBaseBall')
