import pdb
str1='good'
str2='bye'
print str1+str2
print 'very'+' '+str1
print 'my age is '+str(18)
num=18
print 'my age is %d' %num
print 'my age is %f' %4.99
print 'my age is %0.2f' %4.99
print '%s is good' %str1
print 'today is %s' %'friday'

for i in range(0,5):
    print '*',
print
for i in range(0,5):
    for j in range(0,5):
        print i,j
for i in range(0,5):
    for j in range(0,5):
        print '*',
    print
for i in range(0,5):
    for j in range(0,i+1):
        print '*',
    print
    pdb.set_trace()
