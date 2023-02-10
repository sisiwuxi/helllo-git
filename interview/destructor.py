import gc

class MyClass:
    def __init__(self):
        print("Constructor called.")

    def __del__(self):
        # release resource: handle, network, memory
        # clean: close file, shutdown
        # maintain: status 
        print("Destructor called.")

obj = MyClass()
del obj

gc.collect()  # 强制进行垃圾回收