%% 大动作检测

% 关闭并删除已占用端口
if ~isempty(instrfind)
		fclose(instrfind);
		delete(instrfind);
end
% 端口配置
s = serial('com3'); % 创建串行端口对象
set(s, 'BaudRate', 115200, 'StopBits', 1, 'Parity', 'none', 'DataBits', 8, 'InputBufferSize', 10250, 'ReadAsyncMode', 'continuous'); % 配置 波特率 停止位 校验方式 数据位 输入缓存大小（字节） 异步读取
fopen(s); % 打开串口

% 参数预设
sample_rate = 2048; % 原始采样率
div = 8; % 分频数
secnum = 16; % 配置预处理数据长度（时间）
data = zeros(sample_rate, secnum); % 数据构建
cnt = 1; % 计数初始值
win_size_time = sample_rate/ div; % 时域窗长
stride_time = sample_rate/ div/ 2; % 时域步长
time_times = 5; % 时域乘法门限
time_add = 50; % 时域加法门限
win_size_freq = sample_rate/ div/ 2; % 频域窗长
stride_freq = 64; % 频域步长
xhz = 2; % 去除频点数量
freq_times = 40; % 频域乘法门限
respiration_times = 22; % 呼吸频域乘法门限
delay_time = 32; % 延迟时间
delay_time_num = fix(delay_time* 2/ secnum); % 确认无人次数
colorflag = 'g'; % 初始为绿色

while(1)
	% 数据获取
	data_current = fgetl(s); % fgerl - 读取文件中的行 以字符向量形式返回 并删除换行符
	strlen = length(data_current);
	if strlen ~= 10249
		continue
	end
	i = 1;
	if(data_current(i) == 0x42 && data_current(i+ 1) == 0x65 && data_current(i+ 2) == 0x67 && data_current(i+ 3) == 0x69 && data_current(i+ 4) == 0x6E)
		cntt = mod(cnt, 8); % 取余
		if cntt == 0
			cntt = 8;
		end
		for index = 1: 2048
			if data_current(i+ 5+ 5*index) == 0x20
				qian = double(data_current(i+ 5*index+ 1)- 0x30);
				bai = double(data_current(i+ 5*index+ 2)- 0x30);
				shi = double(data_current(i+ 5*index+ 3)- 0x30);
				ge = double(data_current(i+ 5*index+ 4)- 0x30);
				if cnt <= secnum
					data(index, cnt) = qian*1000 + bai*100 + shi*10 + ge*1;
				else
					data(index, secnum- 8+ cntt) = qian*1000 + bai*100 + shi*10 + ge*1;
				end
			end
		end		
		% 数据处理
		if cnt >= secnum && mod(cnt, 8) == 0
			data_Samples = data(1:div:end, :); % 采样率
			num = sample_rate / div * secnum; % 数据抽样后的总数         
			raw_data = reshape(data_Samples, num, 1); % 原始数据降维
			raw_data_MF = raw_data - mean(raw_data); % 均值滤波
			% 大动作检测
			bigmotion_time_vote = time_detection(raw_data_MF, win_size_time, stride_time, time_times, time_add); % 时域判定
			[bigmotion_freq_vote, respirationfreq_vote] = freq_detection(raw_data_MF, win_size_freq, stride_freq, secnum, xhz, freq_times, respiration_times); % 频域判定
			if bigmotion_time_vote && bigmotion_freq_vote
				colorflag = 'r';
				delay_time_num = fix(delay_time* 2/ secnum); % 重置次数
			else
				delay_time_num = delay_time_num - 1;
				if delay_time_num == 0
					colorflag = 'g';
				else
					colorflag = 'r';
				end
			end
			data(:, 1: secnum- 8) = data(:, secnum- 7: secnum); % 数据滑窗       
		end
		cnt = cnt+ 1;
	end
	% 图形显示
	figure(1)
	alpha = 0: pi/20: 2*pi; % 角度[0, 2*pi]
	R = 2; % 半径
	xx = R* cos(alpha);
	yy = R* sin(alpha);
	plot(xx, yy, '-')
	axis equal
	fill(xx, yy, colorflag); % 颜色填充
end