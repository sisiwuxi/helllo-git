#如果直接对文件对象调用 read() 方法，会导致不可预测的内存占用。好的方法是利用固定长度的缓冲区来不断读取文件内容。
def read_file(fpath): 
	print(fpath)
	BLOCK_SIZE = 16
	f=open(fpath, 'rb')
	try:
		block = f.read(BLOCK_SIZE)
		if block:
			yield block
		else:
			print('block='+str(block))
			f.close()
			return 
	finally:
		f.close()
print('start')
read_file('1.txt')
