import pdb

class Solution(object):
    def numTrees(self, n):
        """
        :type n: int
        :rtype: int
        """
        #pdb.set_trace()
        res = [0] * (n+1)
        res[0], res[1] = 1, 1


        for i in range(2, n+1):
            for j in range(1, i+1):
                res[i] += res[j-1] * res[i-j]
                print('res[',i,'] += res[',j-1,'] * res[',i-j,'] --> ',res[i], res[j-1], res[i-j])
        
        return res[n]
n = 3
s = Solution()
print(s.numTrees(n))

"""
res = 1, 1, 0, 0

i = 2,3

i = 2
	j = 1,2
		res[2] += res[0] * res[1] = 0+1 = 1
		res[2] += res[1] * res[0] = 1+1 = 2

res = 1, 1, 2, 0

i = 3
	j = 1,2,3
		res[ 3 ] += res[ 0 ] * res[ 2 ] = 0+1*2 = 2
		res[ 3 ] += res[ 1 ] * res[ 1 ] = 2+1*1 = 3
		res[ 3 ] += res[ 2 ] * res[ 0 ] = 3+2*1 = 5



"""