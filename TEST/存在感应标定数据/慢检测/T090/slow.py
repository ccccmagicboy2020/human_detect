import numpy as np

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

    p0_list.sort(reverse=True)
    p0_list = p0_list[1:-1]
    p0_len = len(p0_list)
    p0_max = max(p0_list)
    p0_min = min(p0_list)
    p0_sum = sum(p0_list)
    p0_avg = p0_sum / p0_len
    #print(p0_list)
    print("{:d}\t{:.2f}\t{:.2f}\t{:.2f}\t{:.2f}".format(p0_len, p0_max, p0_min, p0_avg, np.median(p0_list)))

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

    p1_list.sort(reverse=True)
    p1_list = p1_list[1:-1]
    p1_len = len(p1_list)
    p1_max = max(p1_list)
    p1_min = min(p1_list)
    p1_sum = sum(p1_list)
    p1_avg = p1_sum / p1_len
    #print(p1_list)
    print("{:d}\t{:.2f}\t{:.2f}\t{:.2f}\t{:.2f}".format(p1_len, p1_max, p1_min, p1_avg, np.median(p1_list)))

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

    p2_list.sort(reverse=True)
    p2_list = p2_list[1:-1]
    p2_len = len(p2_list)
    p2_max = max(p2_list)
    p2_min = min(p2_list)
    p2_sum = sum(p2_list)
    p2_avg = p2_sum / p2_len
    #print(p2_list)
    print("{:d}\t{:.2f}\t{:.2f}\t{:.2f}\t{:.2f}".format(p2_len, p2_max, p2_min, p2_avg, np.median(p2_list)))

def check_p3(file_name):
    t = "freq domain res trigger value: "
    p2_list = []
    with open(file_name, 'r') as f:
        for line in f:
            if line.find(t) >= 0:
                pos = line.find('-')
                pos2 = line.find(':') + 1
                p2 = float(line[pos2:pos].strip())
                #print(p2, end='\n')
                p2_list.append(p2)

    p2_list.sort(reverse=True)
    #p2_list = p2_list[1:-1]
    p2_len = len(p2_list)
    p2_max = max(p2_list)
    p2_min = min(p2_list)
    p2_sum = sum(p2_list)
    p2_avg = p2_sum / p2_len
    #print(p2_list)
    print("{:d}\t{:.2f}\t{:.2f}\t{:.2f}\t{:.2f}".format(p2_len, p2_max, p2_min, p2_avg, np.median(p2_list)))

def check_p4(file_name):
    t = "freq domain cfar trigger value: "
    p2_list = []
    with open(file_name, 'r') as f:
        for line in f:
            if line.find(t) >= 0:
                pos = line.find('-')
                pos2 = line.find(':') + 1
                p2 = float(line[pos2:pos].strip())
                #print(p2, end='\n')
                p2_list.append(p2)

    p2_list.sort(reverse=True)
    #p2_list = p2_list[1:-1]
    p2_len = len(p2_list)
    p2_max = max(p2_list)
    p2_min = min(p2_list)
    p2_sum = sum(p2_list)
    p2_avg = p2_sum / p2_len
    #print(p2_list)
    print("{:d}\t{:.2f}\t{:.2f}\t{:.2f}\t{:.2f}".format(p2_len, p2_max, p2_min, p2_avg, np.median(p2_list)))

def check_all():
    print('--------------5.0米---------------')
    print("{:s}\t{:s}\t{:s}\t{:s}\t{:s}".format('len', 'max', 'min', 'avg', 'median'))
    check_p0('慢检测-左半平面-5.25M-90°.log')
    check_p1('慢检测-左半平面-5.25M-90°.log')
    check_p2('慢检测-左半平面-5.25M-90°.log')
    check_p3('慢检测-左半平面-5.25M-90°.log')
    check_p4('慢检测-左半平面-5.25M-90°.log')      
    print('--------------4.5米---------------')
    print("{:s}\t{:s}\t{:s}\t{:s}\t{:s}".format('len', 'max', 'min', 'avg', 'median'))
    check_p0('慢检测-左半平面-4.75M-90°.log')
    check_p1('慢检测-左半平面-4.75M-90°.log')
    check_p2('慢检测-左半平面-4.75M-90°.log')  
    check_p3('慢检测-左半平面-4.75M-90°.log')
    check_p4('慢检测-左半平面-4.75M-90°.log')     
    print('--------------4.0米---------------')
    print("{:s}\t{:s}\t{:s}\t{:s}\t{:s}".format('len', 'max', 'min', 'avg', 'median'))
    check_p0('慢检测-左半平面-4.25M-90°.log')
    check_p1('慢检测-左半平面-4.25M-90°.log')
    check_p2('慢检测-左半平面-4.25M-90°.log')
    check_p3('慢检测-左半平面-4.25M-90°.log')
    check_p4('慢检测-左半平面-4.25M-90°.log')  
    print('--------------3.5米---------------')
    print("{:s}\t{:s}\t{:s}\t{:s}\t{:s}".format('len', 'max', 'min', 'avg', 'median'))
    check_p0('慢检测-左半平面-3.75M-90°.log')
    check_p1('慢检测-左半平面-3.75M-90°.log')
    check_p2('慢检测-左半平面-3.75M-90°.log')
    check_p3('慢检测-左半平面-3.75M-90°.log')
    check_p4('慢检测-左半平面-3.75M-90°.log')   
    print('--------------3.0米---------------')
    print("{:s}\t{:s}\t{:s}\t{:s}\t{:s}".format('len', 'max', 'min', 'avg', 'median'))
    check_p0('慢检测-左半平面-3.25M-90°.log')
    check_p1('慢检测-左半平面-3.25M-90°.log')
    check_p2('慢检测-左半平面-3.25M-90°.log') 
    check_p3('慢检测-左半平面-3.25M-90°.log') 
    check_p4('慢检测-左半平面-3.25M-90°.log')
    print('--------------2.5米---------------')
    print("{:s}\t{:s}\t{:s}\t{:s}\t{:s}".format('len', 'max', 'min', 'avg', 'median'))
    check_p0('慢检测-左半平面-2.75M-90°.log')
    check_p1('慢检测-左半平面-2.75M-90°.log')
    check_p2('慢检测-左半平面-2.75M-90°.log')
    check_p3('慢检测-左半平面-2.75M-90°.log')
    check_p4('慢检测-左半平面-2.75M-90°.log')
    print('--------------2.0米---------------')
    print("{:s}\t{:s}\t{:s}\t{:s}\t{:s}".format('len', 'max', 'min', 'avg', 'median'))
    check_p0('慢检测-左半平面-2.25M-90°.log')
    check_p1('慢检测-左半平面-2.25M-90°.log')
    check_p2('慢检测-左半平面-2.25M-90°.log')
    check_p3('慢检测-左半平面-2.25M-90°.log')
    check_p4('慢检测-左半平面-2.25M-90°.log') 
    print('--------------1.5米---------------')
    print("{:s}\t{:s}\t{:s}\t{:s}\t{:s}".format('len', 'max', 'min', 'avg', 'median'))
    check_p0('慢检测-左半平面-1.75M-90°.log')
    check_p1('慢检测-左半平面-1.75M-90°.log')
    check_p2('慢检测-左半平面-1.75M-90°.log')  
    check_p3('慢检测-左半平面-1.75M-90°.log')
    check_p4('慢检测-左半平面-1.75M-90°.log')  
    print('--------------1.0米---------------')
    print("{:s}\t{:s}\t{:s}\t{:s}\t{:s}".format('len', 'max', 'min', 'avg', 'median'))
    check_p0('慢检测-左半平面-1.25M-90°.log')
    check_p1('慢检测-左半平面-1.25M-90°.log')
    check_p2('慢检测-左半平面-1.25M-90°.log')  
    check_p3('慢检测-左半平面-1.25M-90°.log')
    check_p4('慢检测-左半平面-1.25M-90°.log')  
    print('--------------0.5米---------------')
    print("{:s}\t{:s}\t{:s}\t{:s}\t{:s}".format('len', 'max', 'min', 'avg', 'median'))
    check_p0('慢检测-左半平面-0.75M-90°.log')
    check_p1('慢检测-左半平面-0.75M-90°.log')
    check_p2('慢检测-左半平面-0.75M-90°.log') 
    check_p3('慢检测-左半平面-0.75M-90°.log') 
    check_p4('慢检测-左半平面-0.75M-90°.log')

check_all()

