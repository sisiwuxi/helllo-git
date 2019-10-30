from bs4 import BeautifulSoup
import requests

response = requests.get('https://movie.douban.com/subject/30166972/')
bs = BeautifulSoup(response.text, 'lxml')

name = bs.find('span', attrs={'property': 'v:itemreviewed'}).text
score = bs.find('strong', attrs={'property': 'v:average'}).text
summary = bs.find('div', id='link-report').text.strip()

print('name:\n'+name)
print('score:\n'+score)
print('summary:\n'+summary)
