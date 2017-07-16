import sisiwuxi_nester
import pickle

man=[]
other=[]
#===============open input and obtain man&other=========#
try:
	with open('input.txt') as data:
		for each_line in data:
			try:
				(role, line_spoken) = each_line.split(':', 1)
				line_spoken = line_spoken.strip()
				if role == 'man':
					man.append(line_spoken)
				elif role == 'other':
					other.append(line_spoken)
			except ValueError:
				pass
except IOError as err:
	print('ERROR: ' + str(err))
#===============write to new files=========#
try:
	with open('man.txt', 'wb') as man_file, open('other.txt', 'wb') as other_file:
		pickle.dump(man, man_file)
		pickle.dump(other, other_file)
except IOError as err:
	print('ERROR: ' + str(err))
except pickle.pickleError as perr:
	print('PICKLING ERROR: ' + str(perr))

#===============read to new files=========#
new_man = []
try:
	with open('man.txt', 'rb') as man_file:
		new_man = pickle.load(man_file)
except IOError as err:
	print('ERROR: ' + str(err))
except pickle.pickleError as perr:
	print('PICKLING ERROR: ' + str(perr))

sisiwuxi_nester.print_lol(new_man)
