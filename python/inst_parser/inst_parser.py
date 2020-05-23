"""
$ python3.6 inst_parser.py -i dma.txt -t dma
$ python3.6 inst_parser.py -i jump.txt -t jump

dma.txt:
e21a1004,284d9a00,00000000,01000000,00000000,00000000,0000000,0000000,0000000
c22c813f,00430000,00000000,00004000,00000006,00000000,0000000,0000000,0000000
82140626,00000000,0800c000,00004000,00000005,00000000,0000000,0000000,0000000

jump.txt:
59d3800e
5dd38000,00000008
5d5b0000,00000025

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
	
def parse_dma(line):
    if(line.count(',')<8):
        print('    Please input 9*32bit DMA instructs.')
        return
    line_re = line.replace("\n","")
    inst_arr = line_re.split(',')

    inst_head = int(inst_arr[0],16)
    head_bit_31    = get_bit_val(inst_head, 31)
    head_bit_0_7   = get_bits_val(inst_head, 0, 7)
    head_bit_8_12  = get_bits_val(inst_head, 8, 12)
    head_bit_18_22 = get_bits_val(inst_head, 18, 22)		
    head_bit_27_29 = get_bits_val(inst_head, 27, 29)	
    if head_bit_31==1:
        if head_bit_0_7==4:
            print("load")
            print("    EVR_ID:", head_bit_8_12)
            print("    VR_ID:", head_bit_18_22)
            print("    QID:", head_bit_27_29)
    print("")
    inst_dma_0 = int(inst_arr[1],16)
    inst_dma_0_emi_addr = get_bits_val(inst_dma_0, 0, 31)
    print("    emi_addr:", '0x%x'%inst_dma_0_emi_addr)
    print("")
    inst_dma_1 = int(inst_arr[2],16)
    inst_dma_1_priority = get_bits_val(inst_dma_1, 0, 3)
    inst_dma_1_l2_start_byte = get_bits_val(inst_dma_1, 4, 7)
    inst_dma_1_yuv_cut_en = get_bits_val(inst_dma_1, 8, 11)	
    inst_dma_1_pixel_mode = get_bits_val(inst_dma_1, 12, 13)	
    inst_dma_1_cmprs_mode = get_bits_val(inst_dma_1, 14, 15)	
    inst_dma_1_weight_cmprs_mode = get_bit_val(inst_dma_1, 16)
    inst_dma_1_fixed_cmprs_mode = get_bit_val(inst_dma_1, 17)	
    inst_dma_1_emi_byte_mode = get_bit_val(inst_dma_1, 18)
    inst_dma_1_l2_byte_mode = get_bit_val(inst_dma_1, 19)	
    inst_dma_1_emi_extract_mode = get_bit_val(inst_dma_1, 20)	
    inst_dma_1_l2_extract_mode = get_bit_val(inst_dma_1, 21)
    inst_dma_1_bias_mode = get_bit_val(inst_dma_1, 22)	
    inst_dma_1_w256_mode = get_bit_val(inst_dma_1, 23)	
    inst_dma_1_reg_cm3x3_en = get_bit_val(inst_dma_1, 24)	
    inst_dma_1_reg_bri_en = get_bit_val(inst_dma_1, 25)	
    print("    priority:", inst_dma_1_priority)	
    print("    l2_start_byte:", inst_dma_1_l2_start_byte)	
    print("    yuv_cut_en:", inst_dma_1_yuv_cut_en)	
    print("    pixel_mode:", inst_dma_1_pixel_mode)	
    print("    cmprs_mode:", inst_dma_1_cmprs_mode)	
    print("    weight_cmprs_mode:", inst_dma_1_weight_cmprs_mode)	
    print("    fixed_cmprs_mode:", inst_dma_1_fixed_cmprs_mode)	
    print("    emi_byte_mode:", inst_dma_1_emi_byte_mode)	
    print("    l2_byte_mode:", inst_dma_1_l2_byte_mode)										   
    print("    emi_extract_mode:", inst_dma_1_emi_extract_mode)										   
    print("    l2_extract_mode:", inst_dma_1_l2_extract_mode)
    print("    bias_mode:", inst_dma_1_bias_mode)	
    print("    w256_mode:", inst_dma_1_w256_mode)										   
    print("    reg_cm3x3_en:", inst_dma_1_reg_cm3x3_en)										   
    print("    reg_bri_en:", inst_dma_1_reg_bri_en)										   
    print("")										   
    inst_dma_2 = int(inst_arr[3],16)
    inst_dma_2_l2_start_addr = get_bits_val(inst_dma_2, 0, 13)	
    inst_dma_2_emi_len_0 = get_bits_val(inst_dma_2, 14, 31)
    inst_dma_2_total_length = get_bits_val(inst_dma_2, 14, 31)
	
    inst_dma_3 = int(inst_arr[4],16)
    inst_dma_3_emi_len_1 = get_bits_val(inst_dma_3, 0, 13)	
    inst_dma_3_total_length_l2_cmprs = get_bits_val(inst_dma_3, 0, 13)		
    inst_dma_3_emi_stride_0_l = get_bits_val(inst_dma_3, 14, 31)
	
    inst_dma_4 = int(inst_arr[5],16)							
    inst_dma_4_emi_len_2 = get_bits_val(inst_dma_4, 0, 13)			
    inst_dma_4_emi_stride_1_l = get_bits_val(inst_dma_4, 14, 31)	
    inst_dma_4_uv_offset_0_13 = get_bits_val(inst_dma_4, 0, 13)			
    inst_dma_4_uv_offset_14_19 = get_bits_val(inst_dma_4, 14, 31)	
	
    inst_dma_5 = int(inst_arr[6],16)
    inst_dma_5_emi_stride_0_h = get_bits_val(inst_dma_5, 0, 8)	
    inst_dma_5_emi_stride_1_h = get_bits_val(inst_dma_5, 9, 17)	
    inst_dma_5_l2_stride_0_h = get_bits_val(inst_dma_5, 18, 22)	
    inst_dma_5_l2_stride_1_h = get_bit_val(inst_dma_5, 23)

    inst_dma_6 = int(inst_arr[7],16)	
    inst_dma_6_l2_len_0 = get_bits_val(inst_dma_6, 0, 17)	
    inst_dma_6_l2_stride_0_l = get_bits_val(inst_dma_6, 18, 31)

    inst_dma_7 = int(inst_arr[8],16)
    inst_dma_7_l2_len_1 = get_bits_val(inst_dma_7, 0, 13)	
    inst_dma_7_l2_stride_1_l = get_bits_val(inst_dma_7, 14, 31)
    print("    l2_start_addr:", '0x%x'%inst_dma_2_l2_start_addr)	
    print("    emi_len_0:", inst_dma_2_emi_len_0)
    print("    emi_len_1:", inst_dma_3_emi_len_1)
    print("    emi_len_2:", inst_dma_4_emi_len_2)
    print("    l2_len_0:", inst_dma_6_l2_len_0)
    print("    l2_len_1:", inst_dma_7_l2_len_1)
    inst_dma_emi_stride_0 = inst_dma_3_emi_stride_0_l | (inst_dma_5_emi_stride_0_h>>18)
    print("    emi_stride_0:", '0x%x'%inst_dma_emi_stride_0)	
    inst_dma_emi_stride_1 = inst_dma_4_emi_stride_1_l | (inst_dma_5_emi_stride_1_h>>18)
    print("    emi_stride_1:", '0x%x'%inst_dma_emi_stride_1)	
    inst_dma_l2_stride_0 = inst_dma_6_l2_stride_0_l | (inst_dma_5_l2_stride_0_h>>14)
    print("    l2_stride_0:", '0x%x'%inst_dma_l2_stride_0)
    inst_dma_l2_stride_1 = inst_dma_7_l2_stride_1_l | (inst_dma_5_l2_stride_1_h>>18)
    print("    l2_stride_1:", '0x%x'%inst_dma_l2_stride_1)
    print("")

    print("    total_length:", '0x%x'%inst_dma_2_total_length)
    print("    total_lengtj_l2_cmprs:", '0x%x'%inst_dma_3_total_length_l2_cmprs)
    uv_offset = inst_dma_4_uv_offset_0_13 | (inst_dma_4_uv_offset_14_19>>14)
    print("    uv_offset:", '0x%x'%uv_offset)
    print("")

def parse_jump(line):
    if(len(line)<8):
        print('    Please input 1*32bit instruct.')
        return
    line_re = line.replace("\n","")
    inst_arr = line_re.split(',')

    inst_0 = int(inst_arr[0],16)
    jump_bit_pc_addr = get_bits_val(inst_0, 0, 9)
    jump_bit_jump_base_addr_sr_id = get_bits_val(inst_0, 10, 13)
    jump_bit_jump_sr_src0_id = get_bits_val(inst_0, 14, 17)
    jump_bit_jump_sr_src1_id = get_bits_val(inst_0, 18, 21)	
    jump_bit_jump_sr_type = get_bit_val(inst_0, 22)
    jump_bit_jump_condition = get_bits_val(inst_0, 23, 25)	
    jump_bit_jump_32_mode = get_bit_val(inst_0, 26)
    jump_bit_jump_mode = get_bit_val(inst_0, 27)
    if jump_bit_jump_32_mode == 1:
        inst_1 = int(inst_arr[1],16)
        jump_bit_pc_addr = get_bits_val(inst_0, 0, 31)
    print("    pc_addr:", jump_bit_pc_addr)
    print("    jump_base_addr_sr_id(dynamic_long_jump only):", jump_bit_jump_base_addr_sr_id)
    print("    jump_sr_src0_id(RW):", jump_bit_jump_sr_src0_id)
    print("    jump_sr_src1_id(RO):", jump_bit_jump_sr_src1_id)	
    print("    jump_sr_type(0-sr16, 1-sr32):", jump_bit_jump_sr_type)
    print("    jump_condition(>,<,==,!=,pass,>=,<=):", jump_bit_jump_condition)
    print("    jump_32_mode(0-short,1-long):", jump_bit_jump_32_mode)
    print("    jump_mode(0-static,1-dynamic):", jump_bit_jump_mode)	
    print("")
	
def parser_inst(input_name, inst_type):
    with open(input_name) as fdata:
        for line in fdata.readlines():
            print("=========",line)
            if 'dma' == inst_type:
                parse_dma(line)
            if 'jump' == inst_type:
                parse_jump(line)
def arg_parse():
    parser = argparse.ArgumentParser(description='Instrutions input')
    parser.add_argument('-i', '--input_name' , type=str , default = '0', required = True , help = "weight_file_name")
    parser.add_argument('-t', '--inst_type' , type=str , default = '0', required = True , help = "inst_type")
    return parser.parse_args()

def main():
    args = arg_parse()
    input_name = args.input_name
    inst_type = args.inst_type
    if os.path.exists(input_name) and input_name.split('.')[-1] == 'txt':
        sys.path.append(os.path.dirname(input_name))
        parser_inst(input_name, inst_type) 
    else:
        print("error")

if __name__ == '__main__':
    main()