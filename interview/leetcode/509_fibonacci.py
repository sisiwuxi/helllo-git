"""
f(n) = f(n-1) + f(n-2)
"""
class Solution(object):
    # @staticmethod
    def fib_recursion(self, n):
        if n == 0:
            return 0
        elif n == 1:
            return 1
        else:
            return self.fib_recursion(n-1) + self.fib_recursion(n-2)
            
    # @staticmethod
    def fib_iteration(self, n):
        if n == 1 or n == 2:
            return 1
        else:
            fn1 = 1
            fn2 = 1
            fn = 0
            for i in range(2,n,1):
                fn = fn1 + fn2
                fn1 = fn2
                fn2 = fn
        return fn

if __name__ == "__main__":
    s=Solution()
    n=10
    print(s.fib_recursion(n))
    print(s.fib_iteration(n))