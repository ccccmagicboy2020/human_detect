import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib
import peakutils

matplotlib.rc("font",family='MicroSoft YaHei',weight="bold")

def check_p0(file_name):
    t = "time domain times trigger value: "
    p0_list = []
    with open(file_name, 'r') as f:
        for line in f:
            if line.find(t) >= 0:
                pos = line.find('-')
                pos2 = line.find(':') + 1
                p0 = float(line[pos2:pos].strip())
                #print(p0, end='\n')
                p0_list.append(p0)

    #p0_list.sort(reverse=True)
    p0_list = p0_list[1:-1]
    p0_len = len(p0_list)
    p0_max = max(p0_list)
    p0_min = min(p0_list)
    p0_sum = sum(p0_list)
    p0_avg = p0_sum / p0_len
    #print(p0_list)
    print("{:d}\t{:.2f}\t{:.2f}\t{:.2f}\t{:.2f}".format(p0_len, p0_max, p0_min, p0_avg, np.median(p0_list)))
    plt.figure(file_name.replace('.log', '') + '-p0')
    plt.title("p0")   
    plt.plot(p0_list)
    ##
    print('\r\n')
    dist = 100
    xx = pd.DataFrame(p0_list)
    #print(xx)
    yy = xx[0]
    #print(yy)
    indexes1 = peakutils.indexes(yy, thres=0.01, min_dist=dist)
    #print(len(indexes1), indexes1)
    print(yy[indexes1].nlargest(5))
    print('\r\n')
    ##
    plt.savefig(file_name.replace('.log', '') + '-p0.png')
    plt.close(file_name.replace('.log', '') + '-p0')

def check_p1(file_name):
    t = "time domain add trigger value: "
    p1_list = []
    with open(file_name, 'r') as f:
        for line in f:
            if line.find(t) >= 0:
                pos = line.find('-')
                pos2 = line.find(':') + 1
                p1 = float(line[pos2:pos].strip())
                #print(p1, end='\n')
                p1_list.append(p1)

    #p1_list.sort(reverse=True)
    p1_list = p1_list[1:-1]
    p1_len = len(p1_list)
    p1_max = max(p1_list)
    p1_min = min(p1_list)
    p1_sum = sum(p1_list)
    p1_avg = p1_sum / p1_len
    #print(p1_list)
    print("{:d}\t{:.2f}\t{:.2f}\t{:.2f}\t{:.2f}".format(p1_len, p1_max, p1_min, p1_avg, np.median(p1_list)))
    plt.figure(file_name.replace('.log', '') + '-p1')    
    plt.title("p1")       
    plt.plot(p1_list)
    ##
    print('\r\n')
    dist = 100
    xx = pd.DataFrame(p1_list)
    #print(xx)
    yy = xx[0]
    #print(yy)
    indexes1 = peakutils.indexes(yy, thres=0.01, min_dist=dist)
    #print(len(indexes1), indexes1)
    print(yy[indexes1].nlargest(5))
    print('\r\n')
    ##
    plt.savefig(file_name.replace('.log', '') + '-p1.png')
    plt.close(file_name.replace('.log', '') + '-p1')

def check_p2(file_name):
    t = "freq domain trigger value: "
    p2_list = []
    with open(file_name, 'r') as f:
        for line in f:
            if line.find(t) >= 0:
                pos = line.find('-')
                pos2 = line.find(':') + 1
                p2 = float(line[pos2:pos].strip())
                #print(p2, end='\n')
                p2_list.append(p2)

    #p2_list.sort(reverse=True)
    p2_list = p2_list[1:-1]
    p2_len = len(p2_list)
    p2_max = max(p2_list)
    p2_min = min(p2_list)
    p2_sum = sum(p2_list)
    p2_avg = p2_sum / p2_len
    #print(p2_list)
    print("{:d}\t{:.2f}\t{:.2f}\t{:.2f}\t{:.2f}".format(p2_len, p2_max, p2_min, p2_avg, np.median(p2_list)))
    plt.figure(file_name.replace('.log', '') + '-p2')    
    plt.title("p2")       
    plt.plot(p2_list)
    ##
    print('\r\n')
    dist = 100
    xx = pd.DataFrame(p2_list)
    #print(xx)
    yy = xx[0]
    #print(yy)
    indexes1 = peakutils.indexes(yy, thres=0.01, min_dist=dist)
    #print(len(indexes1), indexes1)
    print(yy[indexes1].nlargest(5))
    print('\r\n')
    ##
    plt.savefig(file_name.replace('.log', '') + '-p2.png')
    plt.close(file_name.replace('.log', '') + '-p2')

def check_p3(file_name):
    t = "freq domain res trigger value: "
    p3_list = []
    with open(file_name, 'r') as f:
        for line in f:
            if line.find(t) >= 0:
                pos = line.find('-')
                pos2 = line.find(':') + 1
                p2 = float(line[pos2:pos].strip())
                #print(p2, end='\n')
                p3_list.append(p2)

    #p3_list.sort(reverse=True)
    #p3_list = p3_list[1:-1]
    p2_len = len(p3_list)
    p2_max = max(p3_list)
    p2_min = min(p3_list)
    p2_sum = sum(p3_list)
    p2_avg = p2_sum / p2_len
    #print(p3_list)
    print("{:d}\t{:.2f}\t{:.2f}\t{:.2f}\t{:.2f}".format(p2_len, p2_max, p2_min, p2_avg, np.median(p3_list)))
    plt.figure(file_name.replace('.log', '') + '-p3')
    plt.title("p3")       
    plt.plot(p3_list)
    ##
    print('\r\n')
    dist = 100
    xx = pd.DataFrame(p3_list)
    #print(xx)
    yy = xx[0]
    #print(yy)
    indexes1 = peakutils.indexes(yy, thres=0.01, min_dist=dist)
    #print(len(indexes1), indexes1)
    print(yy[indexes1].nlargest(5))
    print('\r\n')
    ##
    plt.savefig(file_name.replace('.log', '') + '-p3.png') 
    plt.close(file_name.replace('.log', '') + '-p3')

def check_p4(file_name):
    t = "freq domain cfar trigger value: "
    p4_list = []
    with open(file_name, 'r') as f:
        for line in f:
            if line.find(t) >= 0:
                pos = line.find('-', 38)
                pos2 = line.find(':') + 1
                p2 = float(line[pos2:pos].strip())
                #print(p2, end='\n')
                p4_list.append(p2)
            else:
                p2 = 0.00
                p4_list.append(p2)

    #p4_list.sort(reverse=True)
    #p4_list = p4_list[1:-1]
    p2_len = len(p4_list)
    p2_max = max(p4_list)
    p2_min = min(p4_list)
    p2_sum = sum(p4_list)
    p2_avg = p2_sum / p2_len
#    print(p4_list)
    print("{:d}\t{:.2f}\t{:.2f}\t{:.2f}\t{:.2f}".format(p2_len, p2_max, p2_min, p2_avg, np.median(p4_list)))
    plt.figure(file_name.replace('.log', '') + '-p4')
    plt.title("p4")      
    plt.plot(p4_list)
    ##
    print('\r\n')
    dist = 5
    xx = pd.DataFrame(p4_list)
    #print(xx)
    yy = xx[0]
    #print(yy)
    indexes1 = peakutils.indexes(yy, thres=0.01, min_dist=dist)
    #print(len(indexes1), indexes1)
    print(yy[indexes1].nlargest(5))
    print('\r\n')
    ##
    plt.savefig(file_name.replace('.log', '') + '-p4.png')
    plt.close(file_name.replace('.log', '') + '-p4')

def check_p5(file_name):
    t = "spp_threshold: "
    p5_list = []
    with open(file_name, 'r') as f:
        for line in f:
            if line.find(t) >= 0:
                pos = line.find('-')
                pos2 = line.find(':') + 1
                p5 = float(line[pos2:pos].strip())
                p5_list.append(p5)

    p5_len = len(p5_list)
    p5_max = max(p5_list)
    p5_min = min(p5_list)
    p5_sum = sum(p5_list)
    p5_avg = p5_sum / p5_len
    print("{:d}\t{:.2f}\t{:.2f}\t{:.2f}\t{:.2f}".format(p5_len, p5_max, p5_min, p5_avg, np.median(p5_list)))
    plt.figure(file_name.replace('.log', '') + '-p5')
    plt.title("p5")      
    plt.plot(p5_list)
    ##
    print('\r\n')
    dist = 5
    xx = pd.DataFrame(p5_list)
    #print(xx)
    yy = xx[0]
    #print(yy)
    indexes1 = peakutils.indexes(yy, thres=0.01, min_dist=dist)
    #print(len(indexes1), indexes1)
    print(yy[indexes1].nlargest(5))
    print('\r\n')
    ##
    plt.savefig(file_name.replace('.log', '') + '-p5.png')
    plt.close(file_name.replace('.log', '') + '-p5')

def check_all():
#    print('--------------正对面1.0m5分钟t4096---------------')
#    print("{:s}\t{:s}\t{:s}\t{:s}\t{:s}".format('len', 'max', 'min', 'avg', 'median'))
#    check_p0('正对面1m5min_t4096.log')
#    check_p1('正对面1m5min_t4096.log')
#    check_p2('正对面1m5min_t4096.log')
#    check_p3('正对面1m5min_t4096.log')
#    check_p4('正对面1m5min_t4096.log')    #

#    print('--------------正对面1.0m5分钟t3276---------------')
#    print("{:s}\t{:s}\t{:s}\t{:s}\t{:s}".format('len', 'max', 'min', 'avg', 'median'))
#    check_p0('正对面1m5min_t3276.log')
#    check_p1('正对面1m5min_t3276.log')
#    check_p2('正对面1m5min_t3276.log')
#    check_p3('正对面1m5min_t3276.log')
#    check_p4('正对面1m5min_t3276.log')       #

#    print('--------------无人情况5min_t4096---------------')
#    print("{:s}\t{:s}\t{:s}\t{:s}\t{:s}".format('len', 'max', 'min', 'avg', 'median'))
#    check_p0('无人情况5min_t4096.log')
#    check_p1('无人情况5min_t4096.log')
#    check_p2('无人情况5min_t4096.log')
#    check_p3('无人情况5min_t4096.log')
#    check_p4('无人情况5min_t4096.log')  

#    print('--------------正对面1m5min_t8192---------------')
#    print("{:s}\t{:s}\t{:s}\t{:s}\t{:s}".format('len', 'max', 'min', 'avg', 'median'))
#    check_p0('正对面1m5min_t8192.log')
#    check_p1('正对面1m5min_t8192.log')
#    check_p2('正对面1m5min_t8192.log')
#    check_p3('正对面1m5min_t8192.log')
#    check_p4('正对面1m5min_t8192.log')    

#    print('--------------无人5min_t8192---------------')
#    print("{:s}\t{:s}\t{:s}\t{:s}\t{:s}".format('len', 'max', 'min', 'avg', 'median'))
#    check_p0('无人5min_t8192.log')
#    check_p1('无人5min_t8192.log')
#    check_p2('无人5min_t8192.log')
#    check_p3('无人5min_t8192.log')
#    check_p4('无人5min_t8192.log')   

#    print('--------------无人5min_t3276---------------')
#    print("{:s}\t{:s}\t{:s}\t{:s}\t{:s}".format('len', 'max', 'min', 'avg', 'median'))
#    check_p0('无人5min_t3276.log')
#    check_p1('无人5min_t3276.log')
#    check_p2('无人5min_t3276.log')
#    check_p3('无人5min_t3276.log')
#    check_p4('无人5min_t3276.log')  
#    check_p5('无人5min_t3276.log')


#    print('--------------无人5min_闯入后坐1min_无人5min_t3276---------------')
#    print("{:s}\t{:s}\t{:s}\t{:s}\t{:s}".format('len', 'max', 'min', 'avg', 'median'))
#    check_p0('无人5min_闯入后坐1min_无人5min_t3276.log')
#    check_p1('无人5min_闯入后坐1min_无人5min_t3276.log')
#    check_p2('无人5min_闯入后坐1min_无人5min_t3276.log')
#    check_p3('无人5min_闯入后坐1min_无人5min_t3276.log')
#    check_p4('无人5min_闯入后坐1min_无人5min_t3276.log')  
#    check_p5('无人5min_闯入后坐1min_无人5min_t3276.log')       

    print('--------------老板屋无人10min.log---------------')
    print("{:s}\t{:s}\t{:s}\t{:s}\t{:s}".format('len', 'max', 'min', 'avg', 'median'))
    check_p0('老板屋无人10min.log')
    check_p1('老板屋无人10min.log')
    check_p2('老板屋无人10min.log')
    check_p3('老板屋无人10min.log')
    check_p4('老板屋无人10min.log')  
    check_p5('老板屋无人10min.log')  

check_all()

