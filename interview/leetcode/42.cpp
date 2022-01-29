class Solution {
public:
    int trap(vector<int>& height) {
        int res = 0;
        int max_left = 0;
        int max_right = 0;
        for (int i=0; i<height.size(); i++) {
            max_left = 0;
            max_right = 0;
            for (int l=0; l<(i+1); l++) {
                max_left = max(max_left, height[l]);
            }
            for (int r=i; r<height.size(); r++) {
                max_right = max(max_right, height[r]);
            }
            res += min(max_left,max_right) - height[i];
        }            
        return res;
    }
};
