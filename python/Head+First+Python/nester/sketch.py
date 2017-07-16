import os
os.getcwd()
os.chdir('chapter3')
os.getcwd()

try:
	data = open('s.txt')
	print(data.readline(), end='')
	print(data.readline(), end='')
	data.seek(0)
	for each_line in data:
		print(each_line, end='')
	data.seek(0)

	for each_line in data:
		try:
			(role, line_spoken) = each_line.split(":", 1)
			print(role, end='')
			print(' said: ', end='')
			print(line_spoken, end='')
		except ValueError:
			pass
	data.close()
except IOError:
	print('The data file is missing!')
