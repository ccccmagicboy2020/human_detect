from dearpygui import core,simple
from pylink import jlink
import os

# show the dearpygui logger
#core.show_logger()
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
#jlk.exec_command('usb')
#jlk.exec_command('si 1')
#jlk.exec_command('speed 25000')
jlk.exec_command('device HC32F46X')
#jlk.exec_command('vcom enable')
jlk.set_tif(jlink.enums.JLinkInterfaces.SWD)
jlk.connect(chip_name='HC32F46X', speed=50000)
print(jlk.target_connected())

def apply_text_multiplier(sender, data):
    font_multiplier = core.get_value("Font Size Multiplier")
    core.set_global_font_scale(font_multiplier)

def print_me(sender, data):
    core.log_debug(f"Menu Item: {sender}")

def apply_theme(sender, data):
    theme = core.get_value("Themes")
    core.set_theme(theme)

with simple.window("main"):
    core.set_main_window_size(1024, 768)
    core.add_additional_font('font.TTF', 24, glyph_ranges='chinese_full')
    core.set_main_window_pos(100, 100)
    core.set_main_window_resizable(False)
    core.set_main_window_title('serial helper for phosense')

    with simple.menu_bar("Main Menu Bar"):
        with simple.menu("文件"):
            core.add_menu_item("保存", callback=print_me)
        with simple.menu("帮助"):
            core.add_menu_item("关于", callback=print_me)

    core.add_tab_bar("test")
     
    core.add_tab("基本设置")
    core.add_text("选择界面主题")
    themes = ["Dark", "Light", "Classic", "Dark 2", "Grey", "Dark Grey", "Cherry", "Purple", "Gold", "Red"]
    core.add_combo("Themes", items=themes, default_value="Gold", callback=apply_theme)
    core.set_theme("Gold")
    core.add_slider_float("Font Size Multiplier", default_value=1.0, min_value=0.8, max_value=2.0,
                 callback=apply_text_multiplier)    
    core.end()
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
    core.end()
    core.add_tab("工厂操作")
    core.add_text("这里是工厂操作人员使用的面板")
    core.end()
    core.add_tab("固件更新")
    core.add_text("这里用于现场固件更新面板")

  

core.start_dearpygui(primary_window='main')


