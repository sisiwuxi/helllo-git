#! /usr/bin/python3
import json
import athletemodel
import yate

names = athletemodel.get_namesID_from_store()

print(yate.start_response('application/json'))
print(json.dumps(sorted(names)))
