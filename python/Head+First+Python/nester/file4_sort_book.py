man1=[]
man2=[]
man3=[]
man4=[]

man1_name=[]
man2_name=[]
man3_name=[]
man4_name=[]

man1_dob=[]
man2_dob=[]
man3_dob=[]
man4_dob=[]

time_man1=[]
time_man2=[]
time_man3=[]
time_man4=[]

def replace_symbol(time_string):
	if '-' in time_string:
		splitter = '-'
	elif ':' in time_string:
		splitter = ':'
	else:	
		return(time_string)
	(mins, secs) = time_string.split(splitter)
	return(mins + '.' +secs)	


def fill_data(fn):
	try:
		with open(fn) as man_file:
			data = man_file.readline()
		return(data.strip().split(','))
	except IOError as ioerr:
		print('File error: ' + str(ioerr))
		return(None)


man1=fill_data('21.txt')
man2=fill_data('22.txt')
man3=fill_data('23.txt')
man4=fill_data('24.txt')
print('============info=========')
print(man1)
print(man2)
print(man3)
print(man4)


(man1_name, man1_dob) = man1.pop(0), man1.pop(0)
(man2_name, man2_dob) = man2.pop(0), man2.pop(0)
(man3_name, man3_dob) = man3.pop(0), man3.pop(0)
(man4_name, man4_dob) = man4.pop(0), man4.pop(0)
print('============name+dob=========')
print(man1_name + "," + man1_dob)
print(man2_name + "," + man2_dob)
print(man3_name + "," + man3_dob)
print(man4_name + "," + man4_dob)


time_man1 = sorted(set([replace_symbol(each_t) for each_t in man1]))[0:3]
time_man2 = sorted(set([replace_symbol(each_t) for each_t in man2]))[0:3]
time_man3 = sorted(set([replace_symbol(each_t) for each_t in man3]))[0:3]
time_man4 = sorted(set([replace_symbol(each_t) for each_t in man4]))[0:3]
print('===========name+dob+time==========')
print(man1_name + "," + man1_dob + " = " + str(time_man1))
print(man2_name + "," + man2_dob + " = " + str(time_man2))
print(man3_name + "," + man3_dob + " = " + str(time_man3))
print(man4_name + "," + man4_dob + " = " + str(time_man4))


