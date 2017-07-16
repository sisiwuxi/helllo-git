def sanitize(time_string):	
    if '-' in time_string:
        splitter = '-'
    elif ':' in time_string:
        splitter = ':'
    else:	
        return(time_string)
    (mins, secs) = time_string.split(splitter)
    return(str(mins + '.' +secs))

class AthleteList(list):
    def __init__(self, a_name, a_dob=None, a_times=[]):
        list.__init__([])
        self.name=a_name
        self.dob=a_dob
        self.times=a_times
    def how_big(self):
        return(len(self.thing))
    def top3(self):
        temp = self.times
        return(str(sorted(set([sanitize(each_t) for each_t in temp]))[0:3]))
        #self.top3=sorted(set([sanitize(each_t) for each_t in self.times]))[0:3]

def get_coach_data(filename):
    try:
        with open(filename) as f:
            data=f.readline()
        templ=data.strip().split(',')
        return(AthleteList(templ.pop(0),templ.pop(0),templ))
    except IOError as ioerr:
        print('File error: ' + str(ioerr))
        return(None)     
