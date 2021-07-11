import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib
import peakutils

matplotlib.rc("font",family='MicroSoft YaHei',weight="bold")

def check_p0(file_name):
    t = ".bss                segger_rtt.o"
    p0_list = []
    with open(file_name, 'r') as f:
        for line in f:
            if line.find(t) >= 0:
                pos = line.find('-')
                p0 = line[0:pos].strip()
                p0_list.append(p0)

    print(p0_list)

def check_all():
    print('--------------cb address---------------')
    check_p0('adc_03_sa_interrupt.map')

check_all()

