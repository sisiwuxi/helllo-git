import numpy as np

def rain(heights):
  h_len = len(heights)
  area = 0
  for i in range(1,h_len):
    left_max = np.max(heights[:i+1])
    right_max = np.max(heights[i:h_len])
    area += min(left_max, right_max) - heights[i]
  return area

heights = [0,1,0,2,1,0,1,3,2,1,2,1]
print(rain(heights))
