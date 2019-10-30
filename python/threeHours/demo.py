def add_x_y(x,y):
  return x+y

def add_x_yd(x,y=2):
  return x+y

def return_x_y(x,y):
  return x,y

def replace_x_y(x,y):
  return y,x

def add_all(*input):
  count=0
  for i in input:
    count=count+i 
  return count

def show_all(*args,**kwargs):
  print(args)
  print(kwargs)
  return kwargs
