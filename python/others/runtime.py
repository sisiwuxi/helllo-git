import datetime
import functools
import time

def run_time(func):
  def wrapper(*args, **kw):
    start = time.time()
    res = func(*args, **kw)
    end = time.time()
    print('{0} run time is {1}'.format(func.__name__, (end - start)))
    return res
  return wrapper

@run_time
def test():
  time.sleep(1)
  return "test"

if __name__ == "__main__":
  test()