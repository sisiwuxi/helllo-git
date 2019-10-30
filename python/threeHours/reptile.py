import requests, json

def reptile():
  #url=u"https://static.wanmen.org/uni.3736a8b940120e3c79f6.css"
  #https://www.wanmen.org/uni/catalog
  url="http://api.wanmen.org/genre/majors/courses?attributes=web&brand=univ&retrieve=newCourse"
  r=requests.get(url)
  print(r)
  d=json.loads(r.content.decode('utf-8'))
  print(d)

  for item in d:
   majors = item['Majors']
   for major in majors:
     courses = item['Courses']
     for course in courses:
       print(course['name'])
       print(course['price'])


def json_demo():
  coco={
    "name":u"coco",
    "age":u"28",
    "gender":"Female",
    "Favorite":[u"reading,swimming"],
  }
  s=json.dumps(coco)
  print(s)
