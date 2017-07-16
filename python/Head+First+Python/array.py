movies = ["1", "2", "3"]
print(movies)
print(movies[1])
print(len(movies))

movies.append("4")
print(movies)
movies.pop()
print(movies)
movies.extend(["5","6"])
print(movies)
movies.remove("5")
print(movies)
movies.insert(1,"5")
print(movies)

print("===================insert year after name===================")
movies = ["1", 1990, "2", 1991, "3", 1992]
print(movies)
print(len(movies))


print("========insert '"' "'"=====\" \'=======")


print("===================nesting array===================")
movies = [
	"00", "01", "02",
		["10",	
			["20", "21", "22"]]]
print(movies[0])
print(movies[1])
print(movies[2])
print(movies[3])
print(movies[3][0])
print(movies[3][1])
print(movies[3][1][0])
print(movies[3][1][1])
print(movies[3][1][2])

