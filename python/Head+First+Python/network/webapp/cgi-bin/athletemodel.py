import sqlite3

db_name='coachdata.sqlite'

def get_names_from_store():
    connection=sqlite3.connect(db_name)
    cursor=connection.cursor()
    results=cursor.execute("""SELECT name FROM athletes""")
    response=[row[0] for row in results.fetchall()]
    connection.close()
    return(response)

def get_namesID_from_store():
    connection=sqlite3.connect(db_name)
    cursor=connection.cursor()
    results=cursor.execute("""SELECT name,id FROM athletes""")
    response=results.fetchall()
    connection.close()
    return(response)

def get_athlete_from_id():
    connection=sqlite3.connect(db_name)
    cursor=connection.cursor()
    results=cursor.execute("""SELECT name, dob FROM athletes WHERE id=?""",(athlete_id,))
    (name,dob)=results.fetchone()
    results=cursor.execute("""SELECT value FROM timing_data WHERE athlete_id=?""",(athlete_id,))
    data=[row[0] for row in results.fetchall()]
    response={	'Name':name,
		'DOB' :dob,
		'data':data,
		'top3':data[0:3]}
    connection.close()
    return(response)
'''
import pickle

from athletelist import AthleteList

def get_coach_data(filename):
    try:
        with open(filename) as f:
            data = f.readline()
        templ = data.strip().split(',')
        return(AthleteList(templ.pop(0), templ.pop(0), templ))
    except IOError as ioerr:
        print('File error (get_coach_data): ' + str(ioerr))
        return(None)

def put_to_store(files_list):
    all_athletes = {}
    for each_file in files_list:
        ath = get_coach_data(each_file)
        all_athletes[ath.name] = ath
    try:
        with open('athletes.pickle', 'wb') as athf:
            pickle.dump(all_athletes, athf)
    except IOError as ioerr:
        print('File error (put_and_store): ' + str(ioerr))
    return(all_athletes)

def get_from_store():
    all_athletes = {}
    try:
        with open('athletes.pickle', 'rb') as athf:
            all_athletes = pickle.load(athf)
    except IOError as ioerr:
        print('File error (get_from_store): ' + str(ioerr))
    return(all_athletes)

def get_names_from_store():
    athletes = get_from_store()
    response = [athletes[each_ath].name for each_ath in athletes]
    return(response)
'''
