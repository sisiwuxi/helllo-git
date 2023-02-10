# from multiprocessing import Process, Queue, Lock
import multiprocessing as MP

def f_lock(idx, lock, q=None):
  lock.acquire()
  try:
    all_strs = []
    lim_strs = []
    for i in range(10):
      all_strs.append("all_strs_" + str(idx*10 + i) + " ")
    for i in range(1):
      lim_strs.append("all_strs_" + str(idx*10 + i) + " ")
    if q != None:
      q.put([all_strs, lim_strs])
  finally:
    lock.release()

def f(idx, q=None):
  all_strs = []
  lim_strs = []
  for i in range(100):
    all_strs.append("all_strs_" + str(idx*10 + i) + " ")
  for i in range(10):
    lim_strs.append("all_strs_" + str(idx*10 + i) + " ")
  if q != None:
    q.put([all_strs, lim_strs])


def test():
  fout = open("./del.txt", "w")
  q = MP.Queue()
  lock = MP.Lock()
  p = MP.Process(target=f_lock, args=(4, lock, q))
  p.start()
  p.join()
  p.terminate()
  print(q.get())
  # q.task_done()

  q = MP.Queue()
  processes = []
  for i in range(8):
    # p = Process(target=f_lock, args=(i, lock, q))
    p = MP.Process(target=f, args=(i, q))
    processes.append(p)
  for p in processes:
    p.start()
  for p in processes:
    p.join()
  for p in processes:
    strs = q.get()
    # print(strs)
    all_strs = strs[0]
    lim_strs = strs[1]
    
    for all_str in all_strs:
      fout.write(all_str)
  for p in processes:
    p.terminate()
  fout.close()
  return True

if __name__ == "__main__":
  test()