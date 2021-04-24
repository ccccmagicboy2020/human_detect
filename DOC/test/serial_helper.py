from dearpygui import core,simple
from pylink import jlink
import os
import asyncio
import serial
import threading

class myThread (threading.Thread):
    def __init__(self, threadID, name, counter):
        threading.Thread.__init__(self)
        self.threadID = threadID
        self.name = name
        self.counter = counter
    def run(self):
        print ("开始线程：" + self.name)
        async_process()
        print ("退出线程：" + self.name)

def apply_text_multiplier(sender, data):
    font_multiplier = core.get_value("Font Size Multiplier")
    core.set_global_font_scale(font_multiplier)

def print_me(sender, data):
    core.log_debug(f"Menu Item: {sender}")

def apply_theme(sender, data):
    theme = core.get_value("Themes")
    core.set_theme(theme)

def init_jlink():
    global jlk
    #jlink initial
    dllpath = os.path.join(os.getcwd(), 'JLink_x64.dll')
    xmlpath = os.path.join(os.getcwd(), 'JLinkDevices.xml')
    dev_xml = 'JLinkDevicesXMLPath ' + xmlpath
    print(dllpath)
    print(dev_xml)
    jlk = jlink.JLink(lib=jlink.library.Library(dllpath=dllpath))
    jlk.open()
    print(jlk.product_name)
    print(jlk.connected())
    print(jlk.update_firmware())
    print(jlk.target_connected())
    jlk.exec_command(dev_xml)
    jlk.exec_command('device HC32F46X')
    jlk.set_tif(jlink.enums.JLinkInterfaces.SWD)
    jlk.connect(chip_name='HC32F46X', speed=50000)
    print(jlk.target_connected())

def exit_handle():
    global thread1
    global stop_thread_flag
    stop_thread_flag = True
    thread1.join()
    ser.close()
    jlk.close()
    print('Bye-Bye!!!')

def init_ui():
    #dearpygui initial
    # show the dearpygui logger
    #core.show_logger()

    with simple.window("main"):
        core.set_main_window_size(1024, 768)
        core.add_additional_font('font.TTF', 24, glyph_ranges='chinese_full')
        core.set_main_window_pos(100, 100)
        core.set_main_window_resizable(False)
        core.set_main_window_title('serial helper for phosense')
        core.set_exit_callback(callback=exit_handle)

        with simple.menu_bar("Main Menu Bar"):
            with simple.menu("文件"):
                core.add_menu_item("保存", callback=print_me)
                core.add_menu_item("退出", callback=print_me)
            with simple.menu("帮助"):
                core.add_menu_item("关于", callback=print_me)

        core.add_tab_bar("test")
        
        core.add_tab("基本设置")
        core.add_text("选择界面主题")
        themes = ["Dark", "Light", "Classic", "Dark 2", "Grey", "Dark Grey", "Cherry", "Purple", "Gold", "Red"]
        core.add_combo("Themes", items=themes, default_value="Gold", callback=apply_theme)
        core.set_theme("Gold")
        core.add_text("选择字体大小")
        core.add_slider_float("Font Size Multiplier", default_value=1.0, min_value=0.8, max_value=2.0,
                    callback=apply_text_multiplier)    
        core.end()
    #####################################################################    
        core.add_tab("DP测试")
        core.add_text("1. 感应延时")
        core.add_button("24s", label="24秒", tip="0x00")
        core.add_same_line(spacing=10)
        core.add_button("32s", label="32秒", tip="0x01")
        core.add_same_line(spacing=10)
        core.add_button("40s", label="40秒", tip="0x02")
        core.add_same_line(spacing=10)
        core.add_button("48s", label="48秒", tip="0x03")
        core.add_same_line(spacing=10)
        core.add_button("64s", label="64秒", tip="0x04")
        core.add_same_line(spacing=10)
        core.add_button("128s", label="128秒", tip="0x05")
        core.add_same_line(spacing=10)
        core.add_button("192s", label="192秒", tip="0x06")            
        core.add_spacing(count=10)
        core.add_text("4. 找我")
        core.add_button("findme", label="找我", tip="板载LED会闪动三次") 
        core.add_spacing(count=10)
        core.add_text("14-17. 光敏门限")
        core.add_slider_int("ambient_light_sensor_threshold1", label="光敏门限1", tip="环境光门限1", max_value=4096)
        core.add_slider_int("ambient_light_sensor_threshold2", label="光敏门限2", tip="环境光门限2", max_value=4096)
        core.add_slider_int("ambient_light_sensor_threshold3", label="光敏门限3", tip="环境光门限3", max_value=4096)
        core.add_slider_int("ambient_light_sensor_threshold4", label="光敏门限4", tip="环境光门限4", max_value=4096)
        core.add_spacing(count=10)
        core.add_text("18. 工厂操作")         
        core.add_button("reset_mcu", label="软复位MCU", tip="只MCU复位")
        core.add_spacing(count=10)
        core.add_text("19. 检测模式")         
        core.add_button("mode_normal", label="正常模式", tip="正常工作的状态：快慢检测结合")
        core.add_same_line(spacing=10)
        core.add_button("fast_only", label="只快检测", tip="只进行快速检测")
        core.add_same_line(spacing=10)
        core.add_button("slow_only", label="只慢检测", tip="只进行慢速带呼吸及心跳的检测")
        core.add_spacing(count=10)
        core.add_text("20. 一般命令")
        core.add_button("save_to_flash", label="保存所有参数到flash", tip="保存后可供下一次上电后使用")
        core.add_spacing(count=10)
        core.add_text("22. 学习")
        core.add_button("s1", label="策略1", tip="请确认背景无人后再学习背景1分钟")
        core.add_same_line(spacing=10)
        core.add_button("s2", label="策略2", tip="请确认背景无人后再学习背景2分钟")
        core.add_same_line(spacing=10)
        core.add_button("s3", label="策略3", tip="请确认背景无人后再学习背景3分钟")
        core.add_same_line(spacing=10)
        core.add_button("s4", label="策略4", tip="请确认背景无人后再学习背景4分钟")
        core.add_spacing(count=10)
        core.add_text("24. 加载预设参数")
        core.add_button("ceil_setup0", label="吸顶2.5米", tip="0x00")
        core.add_same_line(spacing=10)
        core.add_button("ceil_setup1", label="吸顶3.5米", tip="0x01")
        core.add_same_line(spacing=10)
        core.add_button("wall_setup0", label="壁挂5米", tip="0x02")
        core.add_same_line(spacing=10)
        core.add_button("wall_setup1", label="壁挂7米", tip="0x03")
        core.add_spacing(count=1)
        core.add_button("user_setup0", label="用户预存1", tip="0x04")
        core.add_same_line(spacing=10)
        core.add_button("user_setup1", label="用户预存2", tip="0x05")
        core.add_same_line(spacing=10)
        core.add_button("user_setup2", label="用户预存3", tip="0x06")   
        core.add_same_line(spacing=10)
        core.add_button("user_setup3", label="用户预存4", tip="0x07")          
        core.add_spacing(count=10)                 
        core.end()
    #####################################################################    
        core.add_tab("工厂操作")
        core.add_text("这里是工厂操作人员使用的面板")
        core.end()
        core.add_tab("固件更新")
        core.add_text("这里用于现场固件更新面板")

def init_uart():
    global ser
    ser=serial.Serial("com6", 115200, timeout=0.5)
    print(ser.port)

    ser.close()
    ser.open()
    ser.set_buffer_size(rx_size = 1024, tx_size = 1024)

async def send_command0(delay):
    global ser
    
    while True:
        print('this message is from send_command0 function')
        if (stop_thread_flag == True):
            break        
        await asyncio.sleep(delay)  # 阻塞直到协程sleep返回结果

async def rev_command0(delay):
    global ser
    
    while True:
        print('this message is from rev_command0 function')
        if (stop_thread_flag == True):
            break
        await asyncio.sleep(delay)  # 阻塞直到协程sleep返回结果  

def ui():
    core.start_dearpygui(primary_window='main')

def async_process():
    asyncio.run(main_co())

async def main_co():
    task1 = asyncio.create_task(send_command0(3))   # 3s
    task2 = asyncio.create_task(rev_command0(1))

    await task1
    await task2

def main():
    global ser
    global jlk
    global thread1
    global stop_thread_flag
    stop_thread_flag = False
    init_jlink()
    init_uart()
    init_ui()

    try:
        thread1 = myThread(1, "Thread-1", 1)
        thread1.start()
        ui()
    except KeyboardInterrupt:
        ser.close()
        jlk.close()
        print('Bye-Bye!!!')
        pass

if __name__ == '__main__':
    main()