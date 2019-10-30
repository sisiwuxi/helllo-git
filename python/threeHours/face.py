import requests
import json

def detect_face(image_url, api_key="CkI_sAAw0DshzGcb3kzwgGsN5WuAFxXR", api_secret="nUMNFBv-zFAv91Zyx7-eE25mIgRo_dM4", return_landmark=1, return_attributes="gender,age,smiling,headpose,facequality,blur,eyestatus,emotion,ethnicity,beauty,mouthstatus,eyegaze,skinstatus"):
  url="https://api-cn.faceplusplus.com/facepp/v3/detect"
  params = {
    "api_key": api_key,
    "api_secret": api_secret,
    "image_url": image_url,
    "return_landmark": return_landmark,
    "return_attributes": return_attributes
  }
  r = requests.post(url=url, params=params)
  return r


url=u"https://timgsa.baidu.com/timg?image&quality=80&size=b9999_10000&sec=1555247505945&di=f4c27da243eaf3439ab8deca3077bf59&imgtype=0&src=http%3A%2F%2Fb-ssl.duitang.com%2Fuploads%2Fitem%2F201602%2F25%2F20160225001211_2YUdL.jpeg"
r=detect_face(image_url=url)
data=json.loads(r.content.decode('utf-8')) 

print(data)

