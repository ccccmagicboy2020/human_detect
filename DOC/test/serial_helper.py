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
    core.end()
    core.add_tab("DP测试")
    core.add_text("这里是数据点测试面板")
    core.add_button("Apply1", label="Apply", tip="delete record")
    core.add_spacing(count=50, name="spacing1")
    core.add_button("Apply2", label="Apply", tip="delete record") 
    core.end()
    core.add_tab("工厂操作")
    core.add_text("这里是工厂操作人员使用的面板")
    core.end()
    core.add_tab("固件更新")
    core.add_text("这里用于现场固件更新面板")

  

core.start_dearpygui(primary_window='main')


