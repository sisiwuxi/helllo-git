# any integer other than 0 or ± 1 that is not divisible without remainder by any other integers except ± 1 and ± the integer itself
import math

# O(x)
def isPrime(x):
  for i in range(2,x-1):
    if x%i == 0:
      return False
  # print(x)
  return True

# O(x) -> O(sqrt(x))
def isPrime_2(x):
  # for (i*i) in range(2,x):
  upper_bound = max(x, math.floor(math.sqrt(x)))
  for i in range(2, upper_bound):
    if x%i == 0:
      return False
  # print(x)
  return True

# Brute Force
# O(n)
def bf(n):
  count = 0
  for i in range(2,n):
    # count += 1 if isPrime(i)==True else 0
    count += 1 if isPrime_2(i)==True else 0
  return count

def eratosthenes(n):
  count = 0
  # 0 = prime
  mask = [0]*(n+1)
  
  for i in range(2,n+1):
    if(mask[i] == 0):
      for j in range(i,(n//i)+1):
        mask[i*j] = 1
      count += 1
      # print(i)
  return count

if __name__ == '__main__':
  print(bf(100))
  print(eratosthenes(100))