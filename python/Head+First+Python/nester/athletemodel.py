import pickle
from athletelist import AthleteList

def get_coach_data(filename):
    try:
        with open(filename) as f:
            data=f.readline()
        templ=data.strip().split(',')
        return(AthleteList(templ.pop(0),templ.pop(0),templ))
    except IOError as ioerr:
        print('File error: ' + str(ioerr))
        return(None)

def put_to_store(files_list):
    all_athletes={}
    for each_file in files_list:
        ath=get_coach_data(each_file)
        all_athletes[ath.name]=ath
    try:
        with open('athletes.pickle','wb') as athf:
            pickle.dump(all_athletes, athf)
    except IOError as ioerr:
        print('File error (put_and_store):' + str(ioerr))
    return(all_athletes)

def get_from_store():
    all_athletes={}
    try:
        with open('athletes.pickle','rb') as athf:
            all_athletes=pickle.load(athf)
    except IOError as ioerr:
        print('File error (get_from_store):' + str(ioerr))
    return(all_athletes)

the_files=['21.txt','22.txt','23.txt','24.txt']
data=put_to_store(the_files)
print(data)
print('================put_to_store==================')
for each_athlete in data:
    #print(data[each_athlete].name + ' ' + data[each_athlete].dob + ' ' + str(data[each_athlete].times))
    print(data[each_athlete].name + ' ' + data[each_athlete].dob + ' ' + str(data[each_athlete].top3()))
print('================get_from_store==================')
data_copy = get_from_store()
for each_athlete in data_copy:
    #print(data_copy[each_athlete].name + ' ' + data_copy[each_athlete].dob + ' ' + str(data_copy[each_athlete].times))
    print(data_copy[each_athlete].name + ' ' + data_copy[each_athlete].dob + ' ' + str(data_copy[each_athlete].top3()))
