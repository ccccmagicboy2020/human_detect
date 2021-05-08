close all
clearvars
clear
clc

% 关闭并删除已占用端口
if ~isempty(instrfind)
		fclose(instrfind);
		delete(instrfind);
end
% 端口配置
s = serial('com3'); % 创建串行端口对象
set(s, 'BaudRate', 115200, 'StopBits', 1, 'Parity', 'none', 'DataBits', 8, 'InputBufferSize', 2562, 'ReadAsyncMode', 'continuous') % 配置 波特率 停止位 校验方式 数据位 输入缓存大小（字节） 异步读取
fopen(s) % 打开串口

while(1)
	%% 快检测
	sample_rate = 512; % 串口包数据量
	div = 1; % 分频数
	data = zeros(sample_rate, 1); % 数据构建
    secnum = 8; % 配置预处理数据长度（时间）
	time_accum = 2048/ sample_rate* secnum; % 累积次数
	data_accum = zeros(sample_rate* time_accum, 1); % 累积数据
	count = 0; % 累积计数
	win_size_time = sample_rate* 4; % 时域窗长
	stride_time = sample_rate* 4/ 2; % 时域步长
	time_times = 3.8; % 时域乘法门限
	time_add = 35; % 时域加法门限
	win_size_freq = sample_rate/ 2; % 频域窗长
	stride_freq = 408/ 4; % 频域步长
	xhz = 2; % 去除频点数量
	pf = 50; % 工频频点
	freq_times = 4; % 频域乘法门限
	respiration_times = 16.5; % 呼吸频域乘法门限
	colorflag = 'g'; % 初始为绿色
	while(1)
		% 数据获取
		data_current = fgetl(s); % fgerl - 读取文件中的行 以字符向量形式返回 并删除换行符
		strlen = length(data_current);
		if strlen ~= 2562
			continue
		end
		i = 1;
		if(data_current(i) == 0x42)
			for index = 1: 512
				if data_current(i+ 5+ 5*index- 4) == 0x20
					qian = double(data_current(i+ 5*index+ 1- 4)- 0x30);
					bai = double(data_current(i+ 5*index+ 2- 4)- 0x30);
					shi = double(data_current(i+ 5*index+ 3- 4)- 0x30);
					ge = double(data_current(i+ 5*index+ 4- 4)- 0x30);
					data(index, 1) = qian*1000 + bai*100 + shi*10 + ge*1;
				end
			end
		end
		% 数据处理
		data_sample = data(1:div:end, 1); % 原始数据抽样
		num = sample_rate / div; % 原始数据抽样后的总数
		raw_data = reshape(data_sample, num, 1); % 原始数据降维
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
		data_accum_MF_sample = data_accum_MF(1:4:end, 1); % 数据抽样
		% 时频判定
		quick_detection_result = quick_detection(data_accum_MF, data_accum_MF_sample, win_size_time, stride_time, time_times, time_add, win_size_freq, stride_freq, secnum, xhz, pf, freq_times, respiration_times);
		if quick_detection_result
			colorflag = 'r';
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
		% 跳出快检测
		if colorflag == 'r'
			pause(0.001)
			break
		end
	end
	%% 存在保持检测
	sample_rate = 512; % 原始采样率
	div = 8; % 分频数
    secnum = 16; % 配置预处理数据长度（时间）
    time_accum = 2048/ sample_rate* secnum; % 累积次数
	data = zeros(sample_rate, time_accum); % 数据构建
	cnt = 1; % 计数初始值
	% 大动作检测
	win_size_time = sample_rate* 4/ div; % 时域窗长
	stride_time = sample_rate* 4/ div/ 2; % 时域步长
	time_times = 3.8; % 时域乘法门限
	time_add = 35; % 时域加法门限
	win_size_freq = sample_rate* 4/ div/ 2; % 频域窗长
	stride_freq = 64; % 频域步长
	xhz = 2; % 去除频点数量
	pf = 50; % 工频频点
	freq_times = 4; % 频域乘法门限
    respiration_times = 16.5; % 呼吸频域乘法门限
	bigmotion_colorflag = 'g'; % 初始为绿色
	% 微动检测
	% xhz = 2; % 去除频点数量
	% pf = 50; % 工频频点
	N = 300; % CFAR窗口大小
	pro_N = 200; % CFAR保护单元大小
	PAD = 10^(-8); % 虚警概率
	offsetmax = 0.38; % 门限偏置（较大）
	offsetmin = 0.33; % 门限偏置（较小）
	rr_threshold = 0.5; % 呼吸频率截取范围
	micromotion_colorflag = 'g'; % 初始为绿色
	% 延迟预设
	delay_time = 64; % 延迟时间
	delay_time_num = fix(delay_time* 2/ secnum); % 确认无人次数
	% 其它
	respirationfreq_num = fix(delay_time* 2 / secnum* 0.125); % 0.5Hz频段过门限允许次数
	while(1)
		% 数据获取
		data_current = fgetl(s); % fgerl - 读取文件中的行 以字符向量形式返回 并删除换行符
		strlen = length(data_current);
		if strlen ~= 2562
			continue
		end
		i = 1;
		if(data_current(i) == 0x42)
			cntt = mod(cnt, 32); % 取余
			if cntt == 0
				cntt = 32;
			end
			for index = 1: 512
				if data_current(i+ 5+ 5*index- 4) == 0x20
					qian = double(data_current(i+ 5*index+ 1- 4)- 0x30);
					bai = double(data_current(i+ 5*index+ 2- 4)- 0x30);
					shi = double(data_current(i+ 5*index+ 3- 4)- 0x30);
					ge = double(data_current(i+ 5*index+ 4- 4)- 0x30);
					if cnt <= time_accum
						data(index, cnt) = qian*1000 + bai*100 + shi*10 + ge*1;
					else
						data(index, time_accum- 32+ cntt) = qian*1000 + bai*100 + shi*10 + ge*1;
					end
				end
			end		
			% 数据处理
			if cnt >= time_accum && mod(cnt, 32) == 0
				data_Samples = data(1:div:end, :); % 采样率
				num = sample_rate / div * time_accum; % 数据抽样后的总数         
				raw_data = reshape(data_Samples, num, 1); % 原始数据降维
				raw_data_MF = raw_data - mean(raw_data); % 均值滤波
				% 大动作检测
				bigmotion_time_vote = time_detection(raw_data_MF, win_size_time, stride_time, time_times, time_add); % 时域判定
				[bigmotion_freq_vote, respirationfreq_vote] = freq_detection(raw_data_MF, win_size_freq, stride_freq, secnum, xhz, pf, freq_times, respiration_times); % 频域判定
				if bigmotion_time_vote && bigmotion_freq_vote
					bigmotion_colorflag = 'r';
				else
					bigmotion_colorflag = 'g';
				end
				% 微动检测
				if bigmotion_freq_vote
					offset = offsetmax;
				else
					offset = offsetmin;
				end
				micromotion_detection_result = micromotion_detection(raw_data_MF, secnum, xhz, pf, N, pro_N, PAD, offset, rr_threshold);
				if micromotion_detection_result
					micromotion_colorflag = 'r';
				else
					micromotion_colorflag = 'g';
				end
				% 存在保持检测判定
                if bigmotion_colorflag == 'r'
					colorflag = 'r';
					respirationfreq_num = fix(delay_time* 2 / 16* 0.125); % 重置次数
					delay_time_num = fix(delay_time* 2 / 16); % 重置次数
				elseif micromotion_colorflag == 'r'
					colorflag = 'y';
					respirationfreq_num = fix(delay_time* 2 / 16* 0.125); % 重置次数
					delay_time_num = fix(delay_time* 2 / 16); % 重置次数
				elseif not(bigmotion_time_vote) && not(bigmotion_freq_vote) && respirationfreq_vote
					colorflag = 'y';
					respirationfreq_num = fix(delay_time* 2 / 16* 0.125); % 重置次数
					delay_time_num = fix(delay_time* 2 / 16); % 重置次数
				elseif not(bigmotion_time_vote) && bigmotion_freq_vote && respirationfreq_vote
					respirationfreq_num = respirationfreq_num - 1;
					if(respirationfreq_num < 0) % 64秒允许最多1次呼吸频谱检测有人，3分钟允许最多3次呼吸频谱检测有人，5分钟允许最多5次呼吸频谱检测有人，10分钟允许最多10次呼吸频谱检测有人
                        colorflag = 'y';
						respirationfreq_num = fix(delay_time* 2 / 16* 0.125); % 重置次数
                        delay_time_num = fix(delay_time* 2 / 16); % 重置次数                        
				    else
						delay_time_num = delay_time_num - 1;
						if delay_time_num == 0
							colorflag = 'g';
						end
					end							
				else
					delay_time_num = delay_time_num - 1;
                    if delay_time_num == 0
                        colorflag = 'g';
                    end
                end
				data(:, 1: time_accum- 32) = data(:, time_accum- 31: time_accum); % 数据滑窗
				% 图形显示
				figure(1)
				alpha = 0: pi/20: 2*pi; % 角度[0, 2*pi]
				R = 2; % 半径
				xx = R* cos(alpha);
				yy = R* sin(alpha);
				plot(xx, yy, '-')
				axis equal
				fill(xx, yy, colorflag); % 颜色填充
				% 跳出存在保持检测
				if colorflag == 'g'
					pause(0.001)
					break
				end
			end
			cnt = cnt + 1;
		end				
	end
end