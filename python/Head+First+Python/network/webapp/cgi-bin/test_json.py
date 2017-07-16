import json

names = ['1',['21','22'],'3',['41','42','43']]
print(names)
to_transfer = json.dumps(names)
print(to_transfer)
from_transfer = json.loads(to_transfer)
print(from_transfer)
print("==================================")
name2 = {"DOB": "2011-2-26", "TOP3": ["1.01", "1.21", "1.34"], "Name": "Coco Cai"}
print(name2)
to_transfer = json.dumps(name2)
print(to_transfer)
from_transfer = json.loads(to_transfer)
print(from_transfer)
