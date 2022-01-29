import pdb

class Node(object):
    def __init__(self, val=None, children=None):
        self.val = val
        self.children = children
		
		
class Solution(object):
    def maxDepth(self, root):
        """
        :type root: Node
        :rtype: int
        """
        pdb.set_trace()
        if not root:
            return 0
        if not root.children:
            return 1
        ret = 0
        for i in root.children:
            ret = max(ret, self.maxDepth(i))
            print(ret)
        return ret+1
        # return 1+max(self.maxDepth(i) for i in root.children)

s = Solution()

# root = [1,None,3,2,4,None,5,6]
node1 = Node(1, None)
node2 = Node(2, None)
node3 = Node(3, node5)
node4 = Node(4, None)
node5 = Node(5, None)
node6 = Node(6, None)
s.maxDepth(root)