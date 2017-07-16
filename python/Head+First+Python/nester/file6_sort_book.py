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
		return(time_string)
	(mins, secs) = time_string.split(splitter)
	return(str(mins + '.' +secs))

def fill_data(fn):
	temp=[]
	try:
		with open(fn) as man_file:
			data = man_file.readline()
		temp = data.strip().split(',')
		return({'Name':temp.pop(0),
			'DOB':temp.pop(0),
			'Times':str(sorted(set([replace_symbol(each_t) for each_t in temp]))[0:3])})
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

print('===========name+dob+time==========')
print(man1['Name'] + "," + man1['DOB'] + " = " + man1['Times'])
print(man2['Name'] + "," + man2['DOB'] + " = " + man2['Times'])
print(man3['Name'] + "," + man3['DOB'] + " = " + man3['Times'])
print(man4['Name'] + "," + man4['DOB'] + " = " + man4['Times'])

