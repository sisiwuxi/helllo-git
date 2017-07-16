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

class Athlete(list):
    def __init__(self, a_name, a_dob=None, a_times=[]):
        list.__init__([])
        self.name=a_name
        self.dob=a_dob
        self.times=a_times
    def how_big(self):
        return(len(self.thing))
    def top3(self):
        temp = self.times
        return(str(sorted(set([replace_symbol(each_t) for each_t in temp]))[0:3]))
        #self.top3=sorted(set([replace_symbol(each_t) for each_t in self.times]))[0:3]


a=Athlete('name_a', '2011-1-1', ['2:58','2.58','1.56','2:55','2.28','3.56'])
b=Athlete('name_b', '2011-2-2', ['3:58','2.58','1.36','1:58','2.57','1.56'])

#print(type(a), a)
#print(type(b), b)
print(a.name, a.dob, a.times)
print(b.name, b.dob, b.times)

c=Athlete('name_c')
c.append('2011-3-3')
print(c.name, c.dob, c.times)
c.extend(['2.22','1-21','3:33'])
print(c.top3())

