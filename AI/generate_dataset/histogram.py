#
# python3.6 histogram.py -n /media/sisi/7c7650c7-713b-408d-a76c-c4f5f8846d1b/git/sisiwuxi/helllo-git/AI/normalize_distribution_random/gauss.txt
#
import argparse
import numpy as np
import matplotlib.pyplot as plt
import pdb
import os
import sys

def draw_weight_hist(input_name):
    arr=[]
    with open(input_name) as fdata:
        for line in fdata.readlines():
            lineArr=line.strip().split(',')
            #for index in range(len(lineArr)):
            for index in range(8):
                arr.append(float(lineArr[index]))
    plt.figure("weight")
    print("max = {} in {}".format(np.max(arr), np.argmax(arr)+1))
    print("min = {} in {}".format(np.min(arr), np.argmin(arr)+1))

    n ,bins , patches = plt.hist(arr , bins=200 , density = 1, facecolor = 'green', alpha=1)
    #plt.show()
    plt.savefig("gauss_laplace.png")

def arg_parse():
    parser = argparse.ArgumentParser(description='Hist input')
    parser.add_argument('-n', '--input_name' , type=str , default = '0', required = True , help = "weight_file_name")
    return parser.parse_args()

def main():
    args = arg_parse()
    input_name = args.input_name
    if os.path.exists(input_name) and input_name.split('.')[-1] == 'txt':
        sys.path.append(os.path.dirname(input_name))
        draw_weight_hist(input_name) 
    else:
        print("error")

if __name__ == '__main__':
    main()
