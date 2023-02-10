import pdb

"""	
class Solution(object):
    def letterCombinations(self, digits):
        ret = []
        ch = []
        length = len(str(digits))
        if (0 <= length <= 4):
            for i in range(length):
                if digits[i] == '2':
                    ch.append(['a','b','c'])
                elif digits[i] == '3':
                    ch.append(['d','e','f'])
                elif digits[i] == '4':
                    ch.append(['g','h','i'])
                elif digits[i] == '5':
                    ch.append(['j','k','l'])
                elif digits[i] == '6':
                    ch.append(['m','n','o'])
                elif digits[i] == '7':
                    ch.append(['p','q','r','s'])
                elif digits[i] == '8':
                    ch.append(['t','u','v'])
                elif digits[i] == '9':
                    ch.append(['w','x','y','z'])
                else:
                    print("unsupport ", digits[i])
                    return []
            print(ch)
            
            for a in ch[0]:
                for b in ch[1]:
                    ret.append(a+b)
        else:
            return []
        return ret
"""

class Solution(object):
    def letterCombinations(self, digits):
        """
        :type digits: str
        :rtype: List[str]
        """
        dic = {2: ['a', 'b', 'c'],
               3: ['d', 'e', 'f'],
               4: ['g', 'h', 'i'],
               5: ['j', 'k', 'l'],
               6: ['m', 'n', 'o'],
               7: ['p', 'q', 'r', 's'],
               8: ['t', 'u', 'v'],
               9: ['w', 'x', 'y', 'z'],
               }
        ret = []
        if len(digits) == 0:
            return []
        fir = dic[int(digits[0])]
        if len(digits) == 1:
            return fir
        sec = self.letterCombinations(digits[1:])
        for f in fir:
            for s in sec:        
                ret.append(f+s)
        return ret


if __name__ == '__main__':
    s = Solution()
    print(s.letterCombinations('234'))

	
	
	
	
	
	
	