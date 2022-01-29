def compareVersion(version1, version2):
	v1 = [int(i) for i in version1.split('.')] 
	v2 = [int(i) for i in version2.split('.')] 
	max_len = max(len(v1), len(v2)) 
	v1 += [0]*(max_len - len(v1)) 
	v2 += [0]*(max_len - len(v2))
	if v1 > v2: return 1 
	elif v1 < v2: return -1 
	else: return 0


ver1="1.11"
ver2="1.10.5"
print(compareVersion(ver1, ver2))