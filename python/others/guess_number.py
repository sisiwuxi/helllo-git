from random import randint

num=randint(1,100)
bingo=False
print 'Guess number'

while bingo==False:
    answer=input()

    if answer<num:
        print 'too small'

    if answer>num:
        print 'too big'

    if answer==num:
        print 'equal,%d is right'%answer
        bingo=True

