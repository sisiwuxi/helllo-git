import numpy as np

# np.random.permutation is useful when you need to shuffle ordered pairs, especially for classification
# def permutation(self, object x):
#   ...
#   if isinstance(x, (int, np.integer)):
#     arr = np.arange(x)
#   else:
#     arr = np.array(x)
#   self.shuffle(arr)
#   return arr


def test():
  n = 4
  x = np.arange(n)

  # --- integer --- #
  print("--- integer ---")
  print("n=", n)
  # unsupport
  # np.random.shuffle(n)
  # print("s(n)=",n)
  # return a shuffled range of n
  p = np.random.permutation(n)
  print("p(n)=",p)

  # --- array --- #
  print("--- array ---")
  print("x=", x)
  # shuffles the array inplace
  np.random.shuffle(x)
  print("s(x)=",x)
  # return a shuffled copy of the array randomly
  p = np.random.permutation(x)
  print("p(x)=",p)
  return


if __name__ == "__main__":
  test()

