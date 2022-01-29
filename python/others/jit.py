from numba import jit
import random
import time
import numpy as np
from jax import jit as jax_jit
from tqdm import tqdm
import multiprocessing as mp
# Global Interpreter Lock

def calculate_pi(n=1000):
  acc = 0
  for i in tqdm(range(n)):
    x = random.random()
    y = random.random()
    if (x**2 + y**2) < 1.0:
      acc += 1
  return 4*(acc/n)

@jit
def jit_calculate_pi(n=1000):
  acc = 0
  for i in range(n):
    x = random.random()
    y = random.random()
    if (x**2 + y**2) < 1.0:
      acc += 1
  return 4*(acc/n)

def test_pi():
  start = time.time()
  pi = calculate_pi(10_000_000)
  end = time.time()
  # calculate_pi: 3.1414296 3.975555181503296
  print("calculate_pi:", pi, (end-start))

  start = time.time()
  # jit_calculate_pi = jit(nopython=True)(calculate_pi)
  pi = jit_calculate_pi(10_000_000)
  end = time.time()
  # jit_calculate_pi: 3.1409104 0.6147935390472412
  print("jit_calculate_pi:", pi, (end-start))
  return

def calculate_dot(n = 1_000):
  # M,N,K = 1024,1024,1024
  M,N,K = 16,16,16
  X = np.random.random((M,K))
  Y = np.random.random((K,N))
  Z = np.random.random((M,N))
  for i in range(n):
    Z = X@Y
  return Z

def test_dot():
  start = time.time()
  dot = calculate_dot()
  end = time.time()
  # print("calculate_dot:", dot, (end-start))
  print("calculate_dot:", (end-start))

  start = time.time()
  # WARNING:absl:No GPU/TPU found, falling back to CPU. (Set TF_CPP_MIN_LOG_LEVEL=0 and rerun for more info.)
  jit_calculate_dot = jax_jit(calculate_dot)
  dot = jit_calculate_dot()
  end = time.time()
  # print("calculate_dot:", dot, (end-start))
  print("jit_calculate_dot:", (end-start))
  return

def test_mp():
  pi = 0
  start = time.time()
  for i in range(10):
    pi += calculate_pi(100_000)
  end = time.time()
  print("calculate_pi:", pi, (end-start))

  start = time.time()
  # pi = mp.Pool(4).map(calculate_pi, [1_000]*10)
  pi = mp.Pool().map(calculate_pi, [100_000]*10)
  end = time.time()
  print("mp_calculate_pi:", pi, (end-start))
  return

if __name__ == "__main__":
  # test_pi()
  # test_dot()
  test_mp()
