#! /usr/bin/python3
import cgi
import json
import athletemodel
import yate
import sys

athletes = athletemodel.get_from_store()
form_data = cgi.FieldStorage()
athlete_name = form_data['which_athlete'].value

print(yate.start_response('application/json'))
#print(json.dumps(athletes[athlete_name]))
print(json.dumps(athletes[athlete_name]), file=sys.stderr)

