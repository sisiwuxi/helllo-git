import pdb
def merge_sort(array):
    def merge(left_array, right_array):
        left_idx, right_idx, merge_array = 0,0,list()
        while left_idx<len(left_array) and right_idx<len(right_array):
            if left_array[left_idx] < right_array[right_idx]:
                merge_array.append(left_array[left_idx])
                left_idx+=1
            else:
                merge_array.append(right_array[right_idx])
                right_idx+=1
        merge_array = merge_array + left_array[left_idx:] + right_array[right_idx:]
        return merge_array
    # pdb.set_trace()
    if len(array) == 1:
        return array
    left_array = merge_sort(array[:len(array)//2])
    right_array = merge_sort(array[len(array)//2:])
    return merge(left_array, right_array)


if __name__ == '__main__':
    array = [10,17,50,7,30,24,27,45,15,5,36,21]
    print(merge_sort(array))
