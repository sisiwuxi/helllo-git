import pdb
def factorial_recursion(n):
    if n==1:
        return 1
    else:
        return factorial_recursion(n-1)*n

def factorial_iteration(n):
    product = 1
    for i in range(2,n+1,1):
        product *= i
    return product

if __name__ == "__main__":
    n = 6
    print(factorial_recursion(n))
    print(factorial_iteration(n))