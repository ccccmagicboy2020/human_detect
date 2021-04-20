import serial
import asyncio
from struct import *
import csv
import time

async def main_co():
    
    task1 = asyncio.create_task(
        send_command0(3))   # 3s

    task2 = asyncio.create_task(
        rev_command0(0.01))

    await task1
    await task2

async def send_command0(delay):
    global ser
    global packet_pir_delay
    global packet_load_radar_parameter
    global packet_Light_threshold3
    global packet_Light_threshold4    
    global packet_save_upssa0
    
    while True:
        print('this message is from send function')
        result = ser.write(packet_pir_delay)
        ser.flush()
        result = ser.write(packet_load_radar_parameter)
        ser.flush()
        result = ser.write(packet_Light_threshold3)
        ser.flush()
        result = ser.write(packet_Light_threshold4)
        ser.flush()        
        
        result = ser.write(packet_save_upssa0)
        ser.flush() 
        
        await asyncio.sleep(delay)  # 阻塞直到协程sleep(2)返回结果
        
async def rev_command0(delay):
    global ser
    while True:
        first_byte = ser.read().hex().upper()
        #print(first_byte)
        if '55' == first_byte:
            second_byte = ser.read().hex().upper()
            if 'AA' == second_byte:
                protocol_version = ser.read().hex().upper()
                if '00' == protocol_version:
                    command0 = ser.read().hex().upper()
                    if 'C0' == command0:
                        #print('bingo!')
                        #
                        #
                        length = int.from_bytes(pack('cc', ser.read(), ser.read()), byteorder='big', signed=False)
                        #print(length)
                        if 8 == length:
                            avg = int.from_bytes(pack('c', ser.read()), byteorder='big', signed=False)
                            light_ad = int.from_bytes(pack('c', ser.read()), byteorder='big', signed=False)
                            SUM0 = int.from_bytes(pack('cc', ser.read(), ser.read()), byteorder='big', signed=False)
                            SUM2 = int.from_bytes(pack('cc', ser.read(), ser.read()), byteorder='big', signed=False)
                            TH = int.from_bytes(pack('cc', ser.read(), ser.read()), byteorder='big', signed=False)
                            crc = int.from_bytes(pack('c', ser.read()), byteorder='big', signed=False)
                            site = {"avg": avg*16, 
                                    "light_ad": light_ad,
                                    "SUM0": SUM0*256,
                                    "SUM2": SUM2*256,
                                    "TH": TH*256,
                                    "diff": SUM2*256 - SUM0*256,
                                    }
                            with open('result.csv', 'a', newline='') as csvfile:
                                fieldnames = ['avg', 'light_ad', 'SUM0', 'SUM2', 'TH', 'diff']
                                writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
                                writer.writerow(site)
                            print("平均值：{avg}，光敏值：{light_ad}，SUM0值：{SUM0}，SUM1值：{SUM2}，TH值：{TH}，差值：{diff}".format(**site))
        await asyncio.sleep(delay)

def main():
    print('this message is from main function')
    global ser
    global packet_pir_delay
    global packet_load_radar_parameter
    global packet_Light_threshold3
    global packet_Light_threshold4
    global packet_save_upssa0
    
    ser=serial.Serial("com6", 115200, timeout=0.5)
    print(ser.port)

    ser.close()
    ser.open()
    ser.set_buffer_size(rx_size = 1024, tx_size = 1024)
    
    # packet_pir_delay
    packet_pir_delay = bytearray()
    packet_pir_delay.append(0x55)
    packet_pir_delay.append(0xAA)
    packet_pir_delay.append(0x00)
    packet_pir_delay.append(0x06)
    packet_pir_delay.append(0x00)
    packet_pir_delay.append(0x05)
    packet_pir_delay.append(0x66)
    packet_pir_delay.append(0x04)
    packet_pir_delay.append(0x00)
    packet_pir_delay.append(0x01)
    packet_pir_delay.append(0x01)   # 32s
    packet_pir_delay.append(0x76)
    
    # packet_load_radar_parameter
    packet_load_radar_parameter = bytearray()
    packet_load_radar_parameter.append(0x55)
    packet_load_radar_parameter.append(0xAA)
    packet_load_radar_parameter.append(0x00)
    packet_load_radar_parameter.append(0x06)
    packet_load_radar_parameter.append(0x00)
    packet_load_radar_parameter.append(0x05)
    packet_load_radar_parameter.append(0x69)
    packet_load_radar_parameter.append(0x04)
    packet_load_radar_parameter.append(0x00)
    packet_load_radar_parameter.append(0x01)
    packet_load_radar_parameter.append(0x00)   # 2.5m
    packet_load_radar_parameter.append(0x78)
    
    # Light_threshold3
    packet_Light_threshold3 = bytearray()
    packet_Light_threshold3.append(0x55)
    packet_Light_threshold3.append(0xAA)
    packet_Light_threshold3.append(0x00)
    packet_Light_threshold3.append(0x06)
    packet_Light_threshold3.append(0x00)
    packet_Light_threshold3.append(0x08)
    packet_Light_threshold3.append(0xA7)
    packet_Light_threshold3.append(0x02)
    packet_Light_threshold3.append(0x00)
    packet_Light_threshold3.append(0x04)
    packet_Light_threshold3.append(0x00)        #4000
    packet_Light_threshold3.append(0x00)
    packet_Light_threshold3.append(0x0F)
    packet_Light_threshold3.append(0xA0)    
    packet_Light_threshold3.append(0x69)
    
    # Light_threshold4
    packet_Light_threshold4 = bytearray()
    packet_Light_threshold4.append(0x55)
    packet_Light_threshold4.append(0xAA)
    packet_Light_threshold4.append(0x00)
    packet_Light_threshold4.append(0x06)
    packet_Light_threshold4.append(0x00)
    packet_Light_threshold4.append(0x08)
    packet_Light_threshold4.append(0xA8)
    packet_Light_threshold4.append(0x02)
    packet_Light_threshold4.append(0x00)
    packet_Light_threshold4.append(0x04)
    packet_Light_threshold4.append(0x00)        #3800
    packet_Light_threshold4.append(0x00)
    packet_Light_threshold4.append(0x0E)
    packet_Light_threshold4.append(0xD8)    
    packet_Light_threshold4.append(0xA1)
    
    # packet_save_upssa0
    packet_save_upssa0 = bytearray()
    packet_save_upssa0.append(0x55)
    packet_save_upssa0.append(0xAA)
    packet_save_upssa0.append(0x00)
    packet_save_upssa0.append(0x06)
    packet_save_upssa0.append(0x00)
    packet_save_upssa0.append(0x05)
    packet_save_upssa0.append(0xA9)
    packet_save_upssa0.append(0x04)
    packet_save_upssa0.append(0x00)
    packet_save_upssa0.append(0x01)
    packet_save_upssa0.append(0x01)   # save
    packet_save_upssa0.append(0xB9)    
    
    try:
        asyncio.run(main_co())
    except KeyboardInterrupt:
        ser.close()
        print('Bye-Bye!!!')

if __name__ == '__main__':
    main()
