class Solution_2:
  def addTwoNumbers(self, L1, L2):
    if L1 == None:
      return L2
    if L2 == None:
      return L1
    # assert len(L1) == len(L2), f"{len(L1)} != {len(L2)}"
    LEN = max(len(L1), len(L2))
    ret = []
    carry = 0
    for i in range(LEN):
      if i >= len(L1):
        t = L2[i]+1 if carry==1 else L2[i]
      elif i >= len(L2):
        t = L1[i]+1 if carry==1 else L1[i]
      else:
        t = L1[i]+L2[i]+1 if carry==1 else L1[i]+L2[i]
      if t>=10:
        carry = 1
        t = t-10
      # print(i, carry, t)
      ret.append(t)
    return ret

def leecode_2():
  L1 = [2,4,3]
  L2 = [5,6,4]
  # L1 = [0]
  # L2 = [0]
  s = Solution_2()
  r = s.addTwoNumbers(L1, L2)
  print(r)

class Solution_33:
  def verifyPostOrder(self, postorder:[int]) -> bool:
    def recur(postorder, left, right):
      if left >= right:
        return True
      mid = left
      root = postorder[right]
      while postorder[mid] < root: mid += 1
      r = mid
      while r < right:
        if postorder[r] < root:
          return False
        t += 1
      return recur(postorder, left, mid-1) and recur(postorder, mid, right-1)
          
    return recur(postorder, 0, len(postorder)-1)

def leecode_33():
  """
          A
        B   C
      E  F   D
  preorder traversal: DLR, ABEFCD
  inorder traversal: LDR, EBFACD
  postorder traversal: LRD, EFBDCA
  """
  L = [1,6,3,2,5]
  # L = [1,3,2,6,5]
  s = Solution_33()
  r = s.verifyPostOrder(L)
  print(r)

if __name__ == "__main__":
  # leecode_2()
  leecode_33()