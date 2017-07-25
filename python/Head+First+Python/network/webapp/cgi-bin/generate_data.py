#! /usr/bin/python3
import cgi
import json
import sys

import athletemodel
import yate

form_data = cgi.FieldStorage()
athlete_name = form_data['which_athlete'].value
athlete=athletemodel.get_athlete_from_id(athlete_id)
print(yate.start_response('application/json'))
print(json.dumps(athletes[athlete_name]).as_dict)

