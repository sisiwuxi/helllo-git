"""movies = [
	"00", "01", "02",
		["10",	
			["20", "21", "22", ["30", "31"]]]]
"""
def print_lol(the_list):
	for each_item in the_list:
		if isinstance(each_item, list):
			print_lol(each_item)
		else:
			print(each_item)

#print_lol(movies)
