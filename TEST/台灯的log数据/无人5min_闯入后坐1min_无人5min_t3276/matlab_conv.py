import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib
import peakutils
import csv

matplotlib.rc("font",family='MicroSoft YaHei',weight="bold")

def conv_to_csv(key, file_name, offset = 0):
    counter = 0
    keyword = {"p0": "time domain times trigger value: ", 
               "p1": "time domain add trigger value: ",
               "p2": "freq domain trigger value: ",
               "p3": "freq domain res trigger value: ",
               "p4": "freq domain cfar trigger value: ",
               "p5": "spp_threshold: ",
               }

    with open(file_name, 'r') as f:
        for line in f:
            if line.find(keyword[key]) >= 0:
                counter += 1
                pos = line.find('-', offset)
                pos2 = line.find(':') + 1
                p = float(line[pos2:pos].strip())
                site = {
                        "num": counter, 
                        "value": p,
                        }
                with open(file_name.replace('.log', '-') + key + '.csv', 'a', newline='') as csvfile:
                    fieldnames = ['num', 'value']
                    writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
                    writer.writerow(site)

def check_all():
    conv_to_csv('p0', 'test.log')
    conv_to_csv('p1', 'test.log')
    conv_to_csv('p2', 'test.log')
    conv_to_csv('p3', 'test.log')
    conv_to_csv('p4', 'test.log', 38)
    conv_to_csv('p5', 'test.log')

check_all()

