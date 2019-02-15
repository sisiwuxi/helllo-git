# 切片是python的一种操作符，可对列表(list)，元组(tuple)，字符串进行裁剪操作，可将其理解为特殊的函数。
# 常用方法，以列表list为例，自行拓展理解其用法：
# s=[0,1,2,3,4,5,6,7,8,9]
# s[0:3]=s[:3]=s[0,1,2]		取前三个
# s[-3:]=[7,8,9]			取后三个
# s[1:]=[1,2,3,4,5,6,7,8,9]	取 1~所有 个
# s[:-1]=[0,1,2,3,4,5,6,7,8]取 0~最后一个之间 个
# s[0:5:2]=[0,2,4]			0~5 每2个取第一个
# s[-5::2]=[0,2,4]			-5~所有 每两个取第一个
# s[::2]=[0,2,4,6,8]		0~所有 每两个取第一个
# 切片也可操作字符串、元组tuple

# s=[0,1,2,3,4,5,6,7,8,9]
# s[0:3]=s[:3]=s[0,1,2]
# s[-3:]=[7,8,9]
# s[1:]=[1,2,3,4,5,6,7,8,9]
# s[:-1]=[0,1,2,3,4,5,6,7,8]
# s[0:5:2]=[0,2,4]
# s[-5::2]=[0,2,4]
# s[::2]=[0,2,4,6,8]

def trim(s):
        if s=='':
                return s
        while(s[0]==' '):
                s=s[1:]
                if s=='':
                        return s
        while(s[-1]==' '):
                s=s[:-1]
        return s

if trim('hello  ') != 'hello':
    print('test fail!')
elif trim('  hello') != 'hello':
    print('test fail!')
elif trim('  hello  ') != 'hello':
    print('test fail!')
elif trim('  hello  world  ') != 'hello  world':
    print('test fail!')
elif trim('') != '':
    print('test fail!')
elif trim('    ') != '':
    print('test fail!')
else:
    print('test success!')
