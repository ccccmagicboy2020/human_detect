% function quick_detection_test
close all
clearvars
clc
% 关闭并删除已占用端口
if ~isempty(instrfind)
    fclose(instrfind);
    delete(instrfind);
end
% 端口配置
s = serial('com31'); % 创建串行端口对象
set(s, 'BaudRate', 115200, 'StopBits', 1, 'Parity', 'none', 'DataBits', 8, 'InputBufferSize', 10250, 'ReadAsyncMode', 'continuous'); % 配置 波特率 停止位 校验方式 数据位 输入缓存大小（字节） 异步读取
fopen(s); % 打开串口

% 参数预设
sample_rate = 2048; % 原始采样率
div = 1; % 分频数
fs = sample_rate / div; % 实际采样率
T = 1 / fs; % 实际采样间隔
data = zeros(sample_rate, 1); % 数据构建
time_accum = 8; % 累积时间
data_accum = zeros(sample_rate* time_accum, 1); % 累积数据
count = 0; % 累积计数
colorflag = 'g'; % 初始为绿色

while(1)
    data_current = fgetl(s); % fgerl - 读取文件中的行 以字符向量形式返回 并删除换行符
    strlen = length(data_current);
    if strlen ~= 10249
        continue;
    end
    i = 1;
    if(data_current(i) == 0x42 && data_current(i+ 1) == 0x65 && data_current(i+ 2) == 0x67 && data_current(i+ 3) == 0x69 && data_current(i+ 4) == 0x6E)
        for index = 1: 2048
            if data_current(i+ 5+ 5*index) == 0x20
                qian = double(data_current(i+ 5*index+ 1)- 0x30);
                bai = double(data_current(i+ 5*index+ 2)- 0x30);
                shi = double(data_current(i+ 5*index+ 3)- 0x30);
                ge = double(data_current(i+ 5*index+ 4)- 0x30);
                data(index, 1) = qian*1000 + bai*100 + shi*10 + ge*1;
            end
        end
    end
    % 数据处理
    data_Samples = data(1:div:end, 1); % 原始数据抽样
    num = sample_rate / div; % 原始数据抽样后的总数
    raw_data = reshape(data_Samples, num, 1); % 原始数据降维
    % 填满累积数据
    if count < time_accum
        data_accum(sample_rate* count+ 1: sample_rate* (count+ 1), 1) = raw_data;
        count = count + 1;
        continue
    end
    
    % 新入数据滑窗
    data_accum(1: sample_rate* (time_accum- 1), 1) = data_accum(sample_rate+ 1: end, 1);
    data_accum(sample_rate* (time_accum- 1)+ 1: end, 1) = raw_data;
    % 均值滤波
    data_accum_MF = data_accum - mean(data_accum);
    
    
    
    
    vote = quick_detection(data_accum_MF,sample_rate);%补充过滤频点数组
    
    
    if vote==1
        colorflag = 'r';
    end
    
    
    if colorflag == 'r'
        figure(10)
        alpha = 0: pi/20: 2*pi; % 角度[0, 2*pi]
        R = 2; % 半径
        xx = R* cos(alpha);
        yy = R* sin(alpha);
        plot(xx, yy, '-')
        axis equal
        fill(xx, yy, colorflag); % 用红色填充
        break
    end
end