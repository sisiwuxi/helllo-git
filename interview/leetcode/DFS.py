# https://leetcode.cn/problems/binary-tree-inorder-traversal/solution/python3-er-cha-shu-suo-you-bian-li-mo-ban-ji-zhi-s/
class TreeNode():
  def __init__(self, val, left=None, right=None):
    self.val = val
    self.left = None
    self.right = None

class Solution_94():
  def preorderTraversal(self, root:TreeNode) -> list:
    res = []
    stack = []
    cur = root

    while stack or cur:
      while cur:
        res.append(cur.val)
        stack.append(cur)
        cur = cur.left
      cur = stack.pop()
      cur = cur.right
    return res

  def inorderTraversal(self, root:TreeNode) -> list:
    res = []
    stack = []
    cur = root

    while stack or cur:
      while cur:
        stack.append(cur)
        cur = cur.left
      cur = stack.pop()
      res.append(cur.val)
      cur = cur.right
    return res

  def postorderTraversal(self, root:TreeNode) -> list:
    res = []
    stack = []
    cur = root

    while stack or cur:
      while cur:
        res.append(cur.val)
        stack.append(cur)
        cur = cur.right
      cur = stack.pop()
      cur = cur.left
    return res[::-1]


def leecode_94():
  """
          A
        B   C
      E  F   D
  preorder traversal: DLR, ABEFCD
  inorder traversal: LDR, EBFACD
  postorder traversal: LRD, EFBDCA
  """
  root = [1,None,2,3]
  tn = TreeNode(1)
  tn.right = TreeNode(2)
  tn.right.left = TreeNode(3)

  s = Solution_94()
  r = s.preorderTraversal(tn)
  print(r)
  r = s.inorderTraversal(tn)
  print(r)
  r = s.postorderTraversal(tn)
  print(r)

if __name__ == "__main__":
  leecode_94()