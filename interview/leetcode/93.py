import pdb

def addDot(src, dst, cnt):
	pdb.set_trace()
	if not src and cnt == 4:
		res.append(dst[:-1])
	if src and (cnt < 4):
		for i in range(1, 4):
			val = int(str(src[:i]))
			if (len(src) >= i) and (0 <= val <= 255) and (str(val) == src[:i]):
				addDot(src[i:], dst + src[:i] + '.', cnt+1)
s="25522"
res=[]
addDot(s, '', 0)
print(res)