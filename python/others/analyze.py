import re
import binascii
import argparse

unit = {'0':'conv', '1':'vec0_conv', '2':'vec0_mem', '3':'vec1', '4':'dma0', '5':'dma1', '6':'dma2', '7':'dma4', '8':'dma5', '9':'dma6', 'a':'scalar', 'b':'risc_depend', 'c':'risc_flag', 'd':'risc_task', 'e':'error01', 'f':'error02' }


if __name__ == '__main__':
    parser=argparse.ArgumentParser(description='the argparse package')
    parser.add_argument('--file', '-f', help='process filename')
    parser.add_argument('--ref', '-r', help='ref filename')
    parser.add_argument('--offset', '-o', type=int, help='signal period')
    parser.add_argument('--write', '-w', help='vcd filename')
    params=vars(args)
    file = open(f"{.file}",'rb')
    file1 = open(f"{.ref}",'r')
    refdata = file1.read()
    bindata = file.read()
    hexdata = binascii.hexlify(bindata)
    hex_str = hexdata.decode("ascii")
    IPU_log_frame = []
    IPU_log = []
    IPU_data = []
    offset = args.offset
    IPU_count = {}
    IPU_start = {}
    IPU_end = {}
    IPU_summary = {}
    cross = []
    for i in range(0, int(len(hec_str)/16)):
        IPU_log_frame.append("{hex_str[i*16:i*16+16]}")
    for item in IPU_log_frame:
        IPU_log.append(f"{item[14:16]}{item[12:14]}{item[10:12]}{item[8:10]}{item[6:8]}{item[4:6]}{item[2:4]}{item[0:2]}")
    for i in range(0, len(IPU_log)):
        IPU_summary[IPU_log[i][0]] = 0
        IPU_count[IPU_log[i][0]] = 0 
    for i in range(0, len(IPU_log)):
        IPU_summary[IPU_log[i][0]] += int(IPU_log[i][7:11],16)
        IPU_start[IPU_log[i][0]] = int(IPU_log[i][1:7],16)
        IPU_end[IPU_log[i][0]] = int(IPU_log[i][1:7],16)+int(IPU_log[i][7:11],16)
        if IPU_log[i] not in cross:
                IPU_data.append(f"{int(IPU_log[i][1:7],16)+(IPU_count[IPU_log[i][0]])*offset},"f"{bin(int(IPU_log[i][11:16],16)-(int(IPU_log[i][11:16])>>19<<19))},"f"{IPU_log[i][0]}")
                IPU_data.append(f"{int(IPU_log[i][1:7],16)+int(IPU_log[j][7:11],16)+(IPU_count[IPU_log[i][0]])*offset},""0bz,"f"{IPU_log[i][0]}")
        for j in range (i+1, len(IPU_log)):
            if(IPU_log[j][0] == IPU_log[i][0]):
                if(int(IPU_log[j][1:7],16)<IPU_start[IPU_log[i][0]]):
                    IPU_start[IPU_log[i][0]] = int(IPU_log[i][1:7],16)
                    IPU_count[IPU_log[j][0]] += 1
                elif ((int(IPU_log[j][1:7],16)>IPU_start[IPU_log[i][0]])&(int(IPU_log[j][1:7],16)<IPU_end[IPU_log[i][0]])):
                    cross.append(IPU_log[j])
                    IPU_start[IPU_log[i][0]] = int(IPU_log[i][1:7],16)
                    del IPU_data[-1]
                    del IPU_data[-1]
                    IPU_data.append(f"{int(IPU_log[i][1:7],16)+(IPU_count[IPU_log[i][0]])*offset},"f"{bin(int(IPU_log[i][11:16],16)-(int(IPU_log[i][11:16])>>19<<19))},"f"{IPU_log[i][0]}")
                    IPU_data.append(f"{int(IPU_log[i][1:7],16)+(IPU_count[IPU_log[i][0]])*offset},""0bx,"f"{IPU_log[i][0]}")
                    IPU_data.append(f"{int(IPU_log[i][1:7],16)+int(IPU_log[i][7:11],16)+(IPU_count[IPU_log[i][0]])*offset},"f"{bin(int(IPU_log[j][11:16],16)-(int(IPU_log[i][11:16])>>19<<19))},"f"{IPU_log[i][0]}")
                    IPU_data.append(f"{int(IPU_log[i][1:7],16)+int(IPU_log[j][7:11],16)+(IPU_count[IPU_log[i][0]])*offset},""0bz,"f"{IPU_log[i][0]}")
                    break
                elif ((int(IPU_log[j][11:16],16)-(int(IPU_log[j][11:16],16)>>19<<19)) == (int(IPU_log[i][11:16],16)-(int(IPU_log[i][11:16],16)>>19<<19))):
                    break
                else:
                    IPU_start[IPU_log[i][0]]=int(IPU_log[j][1:7],16)
                    break

    IPU_data.sort(key=lambda i:int(re.match(r'((\w*),(\w*),(\w*))',i).group(2)))

    signal_start=int(re.match(r'((\w*),(\w*),(\w*))',IPU_data[0]).group(2))
    print("signal_start---", signal_start)
    signal_end=int(re.match(r'((\w*),(\w*),(\w*))',IPU_data[len(IPU_data-1)]).group(2))
    print("signal_end---", signal_end)
    output = open(f"{args.write}",'w')
    output.write(refdata+'\n')
    for item in IPU_data:
        item=item.split(",")
        signal_name=item[2]
        signal_time="#"+item[0]
        signal_id=re.match(r'((\w)(b.*))',item[1]).group(3)
        output.write(signal_time+'\n'+signal_id+' ('+signal_name+'\n')
    output.write('end'+'\n')
    output.write('signal_start----'+f"{signal_start}"+'\n')
    output.write('signal_end----'+f"{signal_end}"+'\n')

    for k,v in IPU_summary.items():
        print(unit[k]+'--------summary----',v,'---- percent of whole period ----','{:.6f}%'.format(v/(signal_end-signal_start)*100))
        output.write(unit[k]+'--------summary----'+f"{v}"+'---- percent of whole period ----'+f"{'{:.6f}%'.format(v/(signal_end-signal_start)*100)"+'\n')
    output.close()
    file.close
    file1.close


