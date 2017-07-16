class NamedList(list):
    def __init__(self, a_name):
        list.__init__([])
        self.name = a_name
sisi=NamedList("sisi.wu")
print(type(sisi))
print(dir(sisi))

sisi.append("coco")
sisi.extend(['one',"two",'one2'])
print(sisi)

for attr in sisi:
	print(sisi.name + " is a " + attr + ".")
