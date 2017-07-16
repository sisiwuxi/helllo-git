class Athlete:
	def __init__(self, a_name, a_dob=None, a_times=[]):
		#The code to initialize a "Athlete" object.
		self.name=a_name
		if(a_dob==None):
			self.dob='2010-2-26'	
		else:
			self.dob=a_dob
		self.times=a_times
	def how_big(self):
		return(len(self.thing))
	def top3(self):
		return(sorted(set([replace_symbol(each_t) for each_t in self.times]))[0:3])
	def add_time(self, temp_time):
		self.times.append(temp_time)
	def add_times(self, temp_times=[]):
		self.times.extend(temp_times)

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
	try:
		with open(fn) as man_file:
			data = man_file.readline()
		temp = data.strip().split(',')
		return Athlete(temp.pop(0), temp.pop(0), temp);
	except IOError as ioerr:
		print('File error: ' + str(ioerr))
		return(None)


man1=fill_data('21.txt')
man2=fill_data('22.txt')
man3=fill_data('23.txt')
man4=fill_data('24.txt')

print('===========name+dob+time==========')
#print(man1.name + "," + man1.dob + "," + str(man1.times) + "," + man1.top3())
print(man1.name + "," + man1.dob + "," + str(man1.top3()))
print(man2.name + "," + man2.dob + "," + str(man2.top3()))
print(man3.name + "," + man3.dob + "," + str(man3.top3()))
print(man4.name + "," + man4.dob + "," + str(man4.top3()))

print('===========Add_time1==========')
man1.add_time('2:33')
man1.add_times(['1:11','2:22'])
print(man1.name + "," + man1.dob + "," + str(man1.top3()))
print('===========Add_time2==========')
add = Athlete('add')
add.add_time('1:23')
#add.add_times(['2:34','3:45'])
print(add.name + "," + add.dob + "," + str(add.top3()))
#print(add.name + "," + add.dob)

