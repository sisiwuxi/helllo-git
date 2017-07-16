names = ['name1', 'name2', 'name3']
print(isinstance(names, list))
num_names=len(names)
print(isinstance(num_names, list))


print("===================nesting array===================")
movies = [
	"00", "01", "02",
		["10",	
			["20", "21", "22", ["30", "31"]]]]
for each_item in movies:
	if isinstance(each_item, list):
		for nested_item in each_item:
			if isinstance(nested_item, list):
				for deeper_item in nested_item:
					print(deeper_item)
			else:
				print(nested_item)
	else:
		print(each_item)

