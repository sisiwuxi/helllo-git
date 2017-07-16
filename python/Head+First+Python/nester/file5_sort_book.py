man1=[]
man2=[]
man3=[]
man4=[]

man1_data = {}
man2_data = {}
man3_data = {}
man4_data = {}

def replace_symbol(time_string):	
	temp=[]
	if '-' in time_string:
		splitter = '-'
	elif ':' in time_string:
		splitter = ':'
	else:	
		return(str(sorted(set([replace_symbol(each_t) for each_t in time_string]))[0:3]))
	(mins, secs) = time_string.split(splitter)
	temp = str(mins + '.' +secs)
	return(str(sorted(set([replace_symbol(each_t) for each_t in temp]))[0:3]))	

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

man1_data['Name'] = man1.pop(0)
man1_data['DOB'] = man1.pop(0)
man1_data['Times'] = replace_symbol(man1)

man2_data['Name'] = man2.pop(0)
man2_data['DOB'] = man2.pop(0)
man2_data['Times'] = replace_symbol(man2)

man3_data['Name'] = man3.pop(0)
man3_data['DOB'] = man3.pop(0)
man3_data['Times'] = replace_symbol(man3)

man4_data['Name'] = man4.pop(0)
man4_data['DOB'] = man4.pop(0)
man4_data['Times'] = replace_symbol(man4)

print('===========name+dob+time==========')
print(man1_data['Name'] + "," + man1_data['DOB'] + " = " + man1_data['Times'])
print(man2_data['Name'] + "," + man2_data['DOB'] + " = " + man2_data['Times'])
print(man3_data['Name'] + "," + man3_data['DOB'] + " = " + man3_data['Times'])
print(man4_data['Name'] + "," + man4_data['DOB'] + " = " + man4_data['Times'])

