import numpy as np

#                 _
#         _      | |_   _
#     _  | |_   _|   |_| |_
#   _| | |   | |           |
#              _
#    _        | |
#   | |    _  | |
#   | |_  | |_| |
#   |   | |     |
#   |   | |     |
#
# trap = min(leftmax[i], rightmax[i]) - height[i]
#

def RainWaterTrap(heights):
  h_len = len(heights)
  trap = 0
  for i in range(h_len):
    import pdb;pdb.set_trace()
    left_max = np.max(heights[:i+1])
    right_max = np.max(heights[i:h_len])
    trap += min(left_max, right_max) - heights[i]
    print(i, heights[i], left_max, right_max, trap)
  return trap

  # area = 0
  # for i in range(1,h_len):
  #   import pdb;pdb.set_trace()
  #   left_max = np.max(heights[:i+1])
  #   right_max = np.max(heights[i:h_len])
  #   area += min(left_max, right_max) - heights[i]
  # return area

if __name__ == '__main__':
  heights = [0,1,0,2,1,0,1,3,2,1,2,1]
  # heights = [4,2,0,3,2,5]
  print(RainWaterTrap(heights))
