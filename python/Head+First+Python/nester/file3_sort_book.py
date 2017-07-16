man1=[]
man2=[]
man3=[]
man4=[]

clean_man1=[]
clean_man2=[]
clean_man3=[]
clean_man4=[]

unique_man1=[]
unique_man2=[]
unique_man3=[]
unique_man4=[]

def replace_symbol(time_string):
	if '-' in time_string:
		splitter = '-'
	elif ':' in time_string:
		splitter = ':'
	else:	
		return(time_string)
	(mins, secs) = time_string.split(splitter)
	return(mins + '.' +secs)	

'''
with open('1.txt') as man1_file:
	data = man1_file.readline()
man1 = data.strip().split(',')

with open('2.txt') as man2_file:
	data = man2_file.readline()
man2 = data.strip().split(',')

with open('3.txt') as man3_file:
	data = man3_file.readline()
man3 = data.strip().split(',')

with open('4.txt') as man4_file:
	data = man4_file.readline()
man4 = data.strip().split(',')
'''

def fill_data(fn):
	try:
		with open(fn) as man_file:
			data = man_file.readline()
		return(data.strip().split(','))
	except IOError as ioerr:
		print('File error: ' + str(ioerr))
		return(None)

man1=fill_data('1.txt')
man2=fill_data('2.txt')
man3=fill_data('3.txt')
man4=fill_data('4.txt')
print('============man=========')
print(man1)
print(man2)
print(man3)
print(man4)
'''
for each_t in man1:
	clean_man1.append(replace_symbol(each_t))
for each_t in man2:
	clean_man2.append(replace_symbol(each_t))
for each_t in man3:
	clean_man3.append(replace_symbol(each_t))
for each_t in man4:
	clean_man4.append(replace_symbol(each_t))
'''
clean_man1 = sorted([replace_symbol(each_t) for each_t in man1])
clean_man2 = sorted([replace_symbol(each_t) for each_t in man2])
clean_man3 = sorted([replace_symbol(each_t) for each_t in man3])
clean_man4 = sorted([replace_symbol(each_t) for each_t in man4])


'''
for each_t in clean_man1:
	if(each_t not in unique_man1):
		unique_man1.append(each_t)
for each_t in clean_man2:
	if(each_t not in unique_man2):
		unique_man2.append(each_t)
for each_t in clean_man3:
	if(each_t not in unique_man3):
		unique_man3.append(each_t)
for each_t in clean_man4:
	if(each_t not in unique_man4):
		unique_man4.append(each_t)
'''
print('============clean_man=========')
print(clean_man1)
print(clean_man2)
print(clean_man3)
print(clean_man4)
print('============unique_man=========')
unique_man1 = sorted(set([replace_symbol(each_t) for each_t in man1]))[0:3]
unique_man2 = sorted(set([replace_symbol(each_t) for each_t in man2]))[0:3]
unique_man3 = sorted(set([replace_symbol(each_t) for each_t in man3]))[0:3]
unique_man4 = sorted(set([replace_symbol(each_t) for each_t in man4]))[0:3]



print('=====================')
print(unique_man1[0:3])
print(unique_man2[0:3])
print(unique_man3[0:3])
print(unique_man4[0:3])

