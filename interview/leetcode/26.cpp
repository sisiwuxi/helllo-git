/*
输入：nums = [1,1,2]
输出：2, nums = [1,2]

输入：nums = [0,0,1,1,1,2,2,3,3,4]
输出：5, nums = [0,1,2,3,4]
*/

class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        if (nums.size() == 0) {
            return 0;
        }
        int ret = 0;
        for (int i=0; i<nums.size(); i++) {
            if (nums[ret] != nums[i]) {
                ret++;
                nums[ret] = nums[i];
            }
        }
        ret++;
        return ret;
    }
};


