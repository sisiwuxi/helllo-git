"""
--------------------------------------------------------------------------------
# Method
- Serial in single CPU
  - CPU->IO->CPU->IO
- Parallel CPU & IO
  - CPU->CPU
  -      IO->IO
  -            ->CPU
  - threading
  - asyncio
- Parallel between CPUs
  - CPU1->IO1
  -       CPU2->IO
  - multiprocessing
- Parallel between PCs
  - PC1
  - PC2
# Import
- threading
- multiprocessing
- asyncio
  - parallel function in single thread
- lock
  - file
- queue
  - customer-producer
- Pool
- subprocess

--------------------------------------------------------------------------------
- Intensive
  - computation
    - Compress
    - Encrypt
    - Regular expression search
  - IO
    - Need huge data in external device
    - File
    - Worm
    - Database
- Compare
  - Process
    - Private memory
    - Multi-CPU
  - Thread
    - One CPU
    - rootcause: global interpreter lock(GIL)
  - IO-bound
- Coroutine
  - asyncio sharing same data in one thread 
  - the number of thread = X0,000

--------------------------------------------------------------------------------
# global interpreter lock(GIL)
- python is 100~200 times slower than C++
- rootcause
  - dynamic language: calculate while explainig
  - data type check
  - GIL
- http://www.dabeaz.com/python/UnderstandingGIL.pdf
  - when a thread is running, it holds the GIL
  - GIL released on I/O(read, write, send, recv, etc.)
    - simplifies the management of shared resources

"""
import threading
import requests
import time
import random
import queue
from bs4 import BeautifulSoup

def run_time(func):
  def wrapper(*args, **kw):
    start = time.time()
    res = func(*args, **kw)
    end = time.time()
    print('{0} run time is {1} second'.format(func.__name__, (end - start)))
    return res
  return wrapper

# ----------------------- test_1 ------------------- #
def test_func(a, b):
  print("test_func", a, b)
  return

def test_1():
  t = threading.Thread(target = test_func, args=(100,200))
  t.start()
  t.join()
  return

# ----------------------- test_2 ------------------- #
# urls = [
#   "https://www.cnblogs.com/#p3",
#   "https://www.cnblogs.com/#p4"
# ]

urls = [
  f"https://www.cnblogs.com/#p{page}"
  for page in range(1, 50+1)
]

def craw(url):
  r = requests.get(url)
  print(url, len(r.text))
  return r.text

def parser(html):
  # class="post-item-title"
  soup = BeautifulSoup(html, "html.parser")
  links = soup.find_all("a", class_="post-item-title")
  return [(link["href"], link.get_text()) for link in links]

def do_craw(url_queue:queue.Queue, html_queue:queue.Queue):
  # producer
  while True:
    url = url_queue.get()
    html = craw(url)
    html_queue.put(html)
    print(threading.current_thread().name, f"craw {url}", "url_queue.size=", url_queue.qsize())
    time.sleep(random.randint(1,2))
  return

def do_parser(html_queue:queue.Queue, fout):
  # consumer
  while True:
    html = html_queue.get()
    results = parser(html)
    for res in results:
      fout.write(str(res) + "\n")
    print(threading.current_thread().name, f"results.size", len(results), "html_queue.size=", html_queue.qsize())
    time.sleep(random.randint(1,2))
  return

@run_time
def single_thread():
  for url in urls:
    craw(url)
  return

@run_time
def multi_thread():
  threads = []
  for url in urls:
    threads.append(threading.Thread(target=craw, args=(url,)))
  for thread in threads:
    thread.start()
  for thread in threads:
    thread.join()    
  return

def test_2():
  # craw(urls[0])
  # single_thread() # 4.76s
  multi_thread() # 1.22s
  return

def test_3():
  for result in parser(craw(urls[2])):
    print(result)
  return

def test_4():
  """
  pipeline: 
    input -> processor_1 -> output_1 -> ... -> processor_X -> output_X -> processor_N -> output
  input:    urls
  producer: craw: download urls and get html
  consumer: parser: parse html and save the result
  output:   SQL
  communication between processor:
    q = queue.Queue()
    q.put(item)
    item = q.get()
    q.qsize()
    q.empty()
    q.full()
  """
  url_queue = queue.Queue()
  html_queue = queue.Queue()
  for url in urls:
    url_queue.put(url)
  
  for i in range(3):
    t = threading.Thread(target=do_craw, args=(url_queue, html_queue), name=f"craw{i}")
    t.start()
  
  fout = open("results.txt", "w")
  for i in range(2):
    t = threading.Thread(target=do_parser, args=(html_queue, fout), name=f"parser{i}")
    t.start()
  return

class Account:
  def __init__(self, balance):
    self.balance = balance
    return

lock = threading.Lock()

def draw(account, amount):
  with lock:
    if account.balance >= amount:
      time.sleep(0.1)
      account.balance -= amount
      print(threading.current_thread().name, "success, now=", account.balance)
    else:
      print(threading.current_thread().name, "failed, now=", account.balance)
  return

def test_5():
  account = Account(1000)
  ta = threading.Thread(target=draw, args=(account, 800), name="ta")
  tb = threading.Thread(target=draw, args=(account, 800), name="tb")
  ta.start()
  tb.start()
  return

def test_6():
  """
  thread pool life cycle
       CREATE(alloc)
  start  |
       READY <----------------------
         |                         |
  obtain cpu resource              |
         |                         |
      RUNNING ---sleep/op ---> HANGUP
         |
        END(free)
  task_list
  thread_pool
    reuse thread
    not include CREATE and  END
  """
  from concurrent.futures import ThreadPoolExecutor, as_completed
  # with ThreadPoolExecutor() as pool:
  #   results = pool.map(craw, urls)
  #   for result in results:
  #     print(result)

  # with ThreadPoolExecutor() as pool:
  #   futures = [pool.submit(craw, url) for url in urls]
  #   # order first
  #   for future in futures:
  #     print(future.result())
  #   # complete first
  #   for future in as_completed(futures):
  #     print(future.result())
  
  with ThreadPoolExecutor() as pool:
    htmls = pool.map(craw, urls)
    htmls = list(zip(urls, htmls))
    for url, html in htmls:
      print(url, html)
  
  with ThreadPoolExecutor() as pool:
    futures = {}
    for url, html in htmls:
      future = pool.submit(parser, html)
      futures[future] = url
    # for future, url in futures.items():
    #   print(url, future.result())

    for future in as_completed(futures):
      url = futures[future]
      print(url, future.result())
  return

if __name__ == "__main__":
  # test_1()
  # test_2()
  # test_3()
  # test_4()
  # test_5()
  test_6()

