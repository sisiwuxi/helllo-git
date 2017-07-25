import sqlite3

connection=sqlite3.connect('coachdata.sqlite')
cursor=connection.cursor()

cursor.execute("""CREATE TABLE athletes(
    id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE NOT NULL,
    name TEXT NOT NULL,
    dob DATE NOT NULL )""")

cursor.execute("""CREATE TABLE timing_data(
    athlete_id INTEFER NOT NULL,
    value TEXT NOT NULL,
    FOREIGN KEY (athlete_id) REFERENCES athletes)""")
#cursor.execute("INSERT INTO athletes(name,dob) VALUES(?,?)",(name,dob))
connection.commit()
connection.close()
