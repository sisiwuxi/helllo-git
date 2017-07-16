import sys

man1=[]
man2=[]
man3=[]
man4=[]

clean_man1=[]
clean_man2=[]
clean_man3=[]
clean_man4=[]
#===============replace symbol in each struct=========#
def replace_symbol(time_string):
	#print(time_string)
	if '-' in time_string:
		splitter = '-'
	elif ':' in time_string:
		splitter = ':'
	else:	
		return(time_string)
	(mins, secs) = time_string.split(splitter)
	return(mins + '.' +secs)
#===============sort each struct=========#
def replace_struct(the_list, the_out_list):
	for each_item in the_list:
		if isinstance(each_item, list):
			replace_struct(each_item, the_out_list)
		else:
			the_out_list.append(replace_symbol(each_item))
#===============write_time in each file=========#
def write_time(the_list, the_out_list, fn=sys.stdout):
	for each_line in fn:
		try:
			time = each_line.strip().split(',')
			the_list.append(time)
		except ValueError:
			pass
	print(the_list)
	replace_struct(the_list, the_out_list)
	print('---------->')
	print(the_out_list)
#===============open file & write_time=========#
try:
	with open('1.txt') as man1_file, open('2.txt') as man2_file, open('3.txt') as man3_file, open('4.txt') as man4_file:
		write_time(man1, clean_man1, fn=man1_file)
		write_time(man2, clean_man2, fn=man2_file)
		write_time(man3, clean_man3, fn=man3_file)
		write_time(man4, clean_man4, fn=man4_file)
except IOError as err:
	print('ERROR: ' + str(err))
#===============write to file=========#
try:
	with open('output_file3.txt', 'w') as output_file3:
		print(sorted(clean_man1), file=output_file3)
		print(sorted(clean_man2), file=output_file3)
		print(sorted(clean_man3), file=output_file3)
		print(sorted(clean_man4), file=output_file3)
except IOError as err:
	print('ERROR: ' + str(err))

