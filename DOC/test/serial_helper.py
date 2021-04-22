from dearpygui.core import *
from dearpygui.simple import *
from pylink import jlink
import os

# show the dearpygui logger
show_logger()
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
jlk.exec_command('device=HC32F46X')

 
with window("Tutorial"):
    set_main_window_size(600,440)
    add_tab_bar("bezier_test")
    add_additional_font('NotoSerifCJKjp-Medium.otf', 30, glyph_ranges='chinese_full')
     
    x1, y1 = [100,100]
    x2, y2 = [100,200]
    x3, y3 = [200,200]
    x4, y4 = [200,100]
     
    add_tab("第一个栏目")
    add_simple_plot("Simpleplot1", value=[0.3, 0.9, 0.5, 0.3], height=300)
    end()
     
    add_tab("第二个栏目")
    add_simple_plot("Simpleplot2", value=[0.3, 0.9, 2.5, 8.9], overlay="Overlaying", height=180, histogram=True)
     
    end()
    
def on_render(sender, data):
    delta_time = str(round(get_delta_time(), 4))
    total_time = str(round(get_total_time(), 4))
    set_value("delta_time", delta_time)
    set_value("total_time", total_time)

with window("Tutoria2"):
    add_text("应用运行的总时间: ")
    add_same_line()
    add_label_text("##total_time_text", source="total_time")
    add_text("应用刷新的时间差: ")
    add_same_line()
    add_label_text("##delta_time_text", source="delta_time")
    
set_render_callback(callback=on_render)
 
def print_me(sender, data):
    log_debug(f"菜单项: {sender}")


with window("Tutorial3"):
    with menu_bar("Main Menu Bar"):
        with menu("文件"):
            add_menu_item("保存", callback=print_me)
            add_menu_item("另存为", callback=print_me)

            with menu("设置"):
                add_menu_item("设置项 1", callback=print_me)
                add_menu_item("设置项 2", callback=print_me)

        add_menu_item("帮助", callback=print_me)

        with menu("控件列表"):
            add_checkbox("选择", callback=print_me)
            add_button("点击", callback=print_me)
            add_color_picker4("选择颜色", callback=print_me)

start_dearpygui()


