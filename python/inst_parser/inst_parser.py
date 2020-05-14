"""
$ python3.6 inst_parser.py -n dma.txt

dma.txt:
e21a1004
284d9a00

bit_0_7   = (inst_word & 0x000000FF)>>0
bit_8_12  = (inst_word & 0x00001F00)>>8	
bit_18_22 = (inst_word & 0x007C0000)>>18		
bit_27_29 = (inst_word & 0x38000000)>>27
"""
import argparse
import numpy as np
import matplotlib.pyplot as plt
import pdb
import os
import sys

def get_bit_val(byte, index):
	if byte & (1<<index):
		return 1
	else:
		return 0

def set_bit_val(byte, index, val):
    if val:
        return byte | (1<<index)
    else:
        return byte & ~(1<<index)

def get_bits_val(byte, index1, index2):
	index = index2 - index1 + 1
	mask = 0
	for i in range(index):
		mask = set_bit_val(mask,i,1)
	mask = mask << index1
	ret = (byte & mask) >> index1
	return ret
	
def parse_dma(inst_word):
    bit_31    = get_bit_val(inst_word, 31)
    bit_0_7   = get_bits_val(inst_word, 0, 7)
    bit_8_12  = get_bits_val(inst_word, 8, 12)
    bit_18_22 = get_bits_val(inst_word, 18, 22)		
    bit_27_29 = get_bits_val(inst_word, 27, 29)	
    if bit_31==1:
        if bit_0_7==4:
            print("load")
            print("    EVR_ID:", bit_8_12)
            print("    VR_ID:", bit_18_22)
            print("    QID:", bit_27_29)
	
def parser_inst(input_name):
    with open(input_name) as fdata:
        for line in fdata.readlines():
            print("=========",line)
            parse_dma(int(line.replace("\n", ""),16))

def arg_parse():
    parser = argparse.ArgumentParser(description='Hist input')
    parser.add_argument('-n', '--input_name' , type=str , default = '0', required = True , help = "weight_file_name")
    return parser.parse_args()

def main():
    args = arg_parse()
    input_name = args.input_name
    if os.path.exists(input_name) and input_name.split('.')[-1] == 'txt':
        sys.path.append(os.path.dirname(input_name))
        parser_inst(input_name) 
    else:
        print("error")

if __name__ == '__main__':
    main()