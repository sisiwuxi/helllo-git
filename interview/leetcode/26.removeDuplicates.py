# nums = [1,1,2]
# 2, nums = [1,2]

# nums = [0,0,1,1,1,2,2,3,3,4]
# 5, nums = [0,1,2,3,4]
# 

# // class Solution {
# // public:
# //     int removeDuplicates(vector<int>& nums) {
# //         if (nums.size() == 0) {
# //             return 0;
# //         }
# //         int ret = 0;
# //         for (int i=0; i<nums.size(); i++) {
# //             if (nums[ret] != nums[i]) {
# //                 ret++;
# //                 nums[ret] = nums[i];
# //             }
# //         }
# //         ret++;
# //         return ret;
# //     }
# // };

def removeDuplicates(nums):
    if len(nums) == 0:
        return 0
    idx = 0
    for i in range(len(nums)):
        if nums[idx] != nums[i]:
            idx+=1
            nums[idx] = nums[i]
    idx+=1
    return idx, nums[0:idx]

if __name__ == '__main__':
    nums = [1,1,2]
    # nums = [0,0,1,1,1,2,2,3,3,4]
    idx, nums = removeDuplicates(nums)
    print(idx)
    print(nums)
