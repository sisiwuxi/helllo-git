import numpy as np
def softmax(x):
    import numpy as np
    return np.exp(x) / np.sum(np.exp(x), axis=0)
    
print(softmax([4, 5, 10]))

#[0.00245611 0.00667641 0.99086747]
