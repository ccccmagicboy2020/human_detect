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
fopen(s); % 打开串口

fs = 2000; % 采样率
data_package = 512; % 串口包数据量
result_output_index = 1; % 结果输出索引
result_output = cell(8192, 2); % 结果缓存空间

while(1)
	%% 快检测
	div = 4; % 分频数
	secnum = 8.192; % 配置预处理数据长度（时间）
	time_accum = fs/ data_package* secnum; % 累积次数
	data = zeros(data_package, 1); % 数据构建
	data_accum = zeros(data_package* time_accum, 1); % 累积数据
	count = 0; % 累积计数
	win_size_time = data_package* 4; % 时域窗长
	stride_time = data_package* 4/ 2; % 时域步长
	time_times = 3.8; % 时域乘法门限
	time_add = 35; % 时域加法门限
	win_size_freq = data_package* 4/ div/ 2; % 频域窗长
	stride_freq = 82; % 频域步长
	xhz_b = 6; % 去除频点数量
	pf = 50; % 工频频点
	freq_times = 4; % 频域乘法门限
	respiration_times = 28; % 呼吸频域乘法门限
	while(1)
		% 数据获取
		data_current = fgetl(s); % fgerl - 读取文件中的行 以字符向量形式返回 并删除换行符
		strlen = length(data_current);
		if strlen ~= 2562
			continue
		end
		i = 1;
		if(data_current(i) == 'B')
			for index = 1: 512
				if data_current(i+ 5+ 5*index- 4) == ' '
					qian = double(data_current(i+ 5*index+ 1- 4)- '0');
					bai = double(data_current(i+ 5*index+ 2- 4)- '0');
					shi = double(data_current(i+ 5*index+ 3- 4)- '0');
					ge = double(data_current(i+ 5*index+ 4- 4)- '0');
					data(index, 1) = qian*1000 + bai*100 + shi*10 + ge*1;
				end
			end
		end
		% 填满累积数据
		if count < time_accum
			data_accum(data_package* count+ 1: data_package* (count+ 1), 1) = data;		
			count = count + 1;
			continue
		end
		% 新入数据滑窗
		data_accum(1: data_package* (time_accum- 1), 1) = data_accum(data_package+ 1: end, 1);
		data_accum(data_package* (time_accum- 1)+ 1: end, 1) = data;
		% 数据处理
		raw_data = data_accum; % 原始数据
		raw_data_MF = raw_data - mean(raw_data); % 均值滤波
		raw_data_MF_LP = filter(lowpass_filter(2000, 250, 300), raw_data_MF); % 低通滤波
		raw_data_MF_LP_DS = raw_data_MF_LP(1:div:end, :); % 数据降采样
		% 时频判定
		quick_detection_result = quick_detection(raw_data_MF, raw_data_MF_LP_DS, div, win_size_time, stride_time, time_times, time_add, win_size_freq, stride_freq, fix(secnum), xhz_b, pf, freq_times, respiration_times);
		if quick_detection_result
			colorflag = 'r';
			output_state = '快1';
			filename = strcat(num2str(output_state), '_', datestr(now, 30), '.mat');
			dlmwrite(filename, data_accum)
			% filename = strcat(num2str(output_state), '_', datestr(now, 30), '.xls');
			% xlswrite(filename, data_accum)
		else
			colorflag = 'g';
			output_state = '快0';
		end
		% 输出结果保存
% 		result_output{result_output_index, 1} = datestr(now);
% 		result_output{result_output_index, 2} = output_state;
% 		result_output_index = result_output_index + 1;
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
	div = 8; % 分频数
    secnum = 16.384; % 配置预处理数据长度（时间）
    time_accum = fs/ data_package* secnum; % 累积次数
	data = zeros(data_package, time_accum); % 数据构建
	cnt = 1; % 计数初始值
	N_sigma = 3.2; % 门限倍数
	nbins = 100; % bin数量
	N_interval = 100; % 间隔长度
	% 大动作检测
	win_size_time = data_package* 4/ div; % 时域窗长
	stride_time = data_package* 4/ div/ 2; % 时域步长
	time_times = 3.8; % 时域乘法门限
	time_add = 35; % 时域加法门限
	win_size_freq = data_package* 4/ div/ 2; % 频域窗长
	stride_freq = 44; % 频域步长
	xhz_b = 8; % 去除频点数量
	pf = 50; % 工频频点
	freq_times = 4; % 频域乘法门限
    respiration_times = 30; % 呼吸频域乘法门限
	% 微动检测
	xhz_m = 4; % 去除频点数量
	% pf = 50; % 工频频点
	N = 300; % CFAR窗口大小
	pro_N = 200; % CFAR保护单元大小
	PAD = 10^(-8); % 虚警概率
	offsetmax = 0.45; % 门限偏置（较大）
	offsetmin = 0.45; % 门限偏置（较小）
	rr_threshold = 0.5; % 呼吸频率截取范围
	% 呼吸心跳频率检测
	m = 1024; % 变换长度
	% 延迟预设
	delay_time = 32; % 延迟时间
	delay_time_num = round(delay_time* 2/ secnum); % 确认无人次数
	% 次数预设
	respirationfreq_num = round(delay_time* 2 / secnum* 0.125); % 0.5Hz频段过门限允许次数
	% 即时检测
	div_instant = 4; % 分频数
	win_size_time_instant = data_package/ div_instant; % 时域窗长
	stride_time_instant = data_package/ div_instant/ 2; % 时域步长
	time_times_instant = 3.8; % 时域乘法门限
	time_add_instant = 35; % 时域加法门限
	while(1)
		% 数据获取
		data_current = fgetl(s); % fgerl - 读取文件中的行 以字符向量形式返回 并删除换行符
		strlen = length(data_current);
		if strlen ~= 2562
			continue
		end
		i = 1;
		if(data_current(i) == 'B')
			cntt = mod(cnt, 32); % 取余
			if cntt == 0
				cntt = 32;
			end
			for index = 1: 512
				if data_current(i+ 5+ 5*index- 4) == ' '
					qian = double(data_current(i+ 5*index+ 1- 4)- '0');
					bai = double(data_current(i+ 5*index+ 2- 4)- '0');
					shi = double(data_current(i+ 5*index+ 3- 4)- '0');
					ge = double(data_current(i+ 5*index+ 4- 4)- '0');
					if cnt <= time_accum
						data(index, cnt) = qian*1000 + bai*100 + shi*10 + ge*1;
					else
						data(index, time_accum- 32+ cntt) = qian*1000 + bai*100 + shi*10 + ge*1;
					end
				end
			end
			% 数据处理
			% 存在保持判定
			if cnt >= time_accum && mod(cnt, 32) == 0
				raw_data = reshape(data, data_package* time_accum, 1); % 原始数据
				raw_data_MF = raw_data - mean(raw_data); % 均值滤波
				raw_data_MF_LP = filter(lowpass_filter(2000, 125, 150), raw_data_MF); % 低通滤波
				raw_data_MF_LP_DS = raw_data_MF_LP(1:div:end, :); % 数据降采样
				raw_data_MF_LP_DS_RO = remove_outliers(raw_data_MF_LP_DS, N_sigma, nbins, N_interval); % 去除离群点
				% 大动作检测
				bigmotion_time_vote = time_detection(raw_data_MF_LP_DS_RO, win_size_time, stride_time, time_times, time_add); % 时域判定
				[bigmotion_freq_vote, respirationfreq_vote] = freq_detection(raw_data_MF_LP_DS_RO, div, win_size_freq, stride_freq, fix(secnum), xhz_b, pf, freq_times, respiration_times); % 频域判定
				% 微动检测
				if bigmotion_freq_vote
					offset = offsetmax;
				else
					offset = offsetmin;
				end
				micromotion_vote = micromotion_detection(raw_data_MF_LP_DS_RO, fix(secnum), xhz_m, pf, N, pro_N, PAD, offset, rr_threshold);
				% 呼吸心跳频率检测
				[respiratory_rate, heart_rate] = respiratory_heart_rate_monitoring(raw_data_MF_LP_DS_RO, m, respirationfreq_vote, micromotion_vote);
				% 存在保持检测判定
                if bigmotion_time_vote && bigmotion_freq_vote
					colorflag = 'r';
					output_state = '大1';
% 					filename = strcat(num2str(output_state), '_', datestr(now, 30), '.mat');
% 					dlmwrite(filename, data_accum)
					% filename = strcat(num2str(output_state), '_', datestr(now, 30), '.xls');
					% xlswrite(filename, data_accum)
					respirationfreq_num = fix(delay_time* 2 / 16* 0.125); % 重置次数
					delay_time_num = fix(delay_time* 2 / 16); % 重置次数
				elseif micromotion_vote
					colorflag = 'y';
					output_state = '微1';
% 					filename = strcat(num2str(output_state), '_', datestr(now, 30), '.mat');
% 					dlmwrite(filename, data_accum)
					% filename = strcat(num2str(output_state), '_', datestr(now, 30), '.xls');
					% xlswrite(filename, data_accum)
					respirationfreq_num = fix(delay_time* 2 / 16* 0.125); % 重置次数
					delay_time_num = fix(delay_time* 2 / 16); % 重置次数
				elseif not(bigmotion_time_vote) && not(bigmotion_freq_vote) && respirationfreq_vote
					colorflag = 'y';
					output_state = '微1';
% 					filename = strcat(num2str(output_state), '_', datestr(now, 30), '.mat');
% 					dlmwrite(filename, data_accum)
					% filename = strcat(num2str(output_state), '_', datestr(now, 30), '.xls');
					% xlswrite(filename, data_accum)
					respirationfreq_num = fix(delay_time* 2 / 16* 0.125); % 重置次数
					delay_time_num = fix(delay_time* 2 / 16); % 重置次数
				elseif not(bigmotion_time_vote) && bigmotion_freq_vote && respirationfreq_vote
					respirationfreq_num = respirationfreq_num - 1;
					if(respirationfreq_num < 0) % 64秒允许最多1次呼吸频谱检测有人，3分钟允许最多3次呼吸频谱检测有人，5分钟允许最多5次呼吸频谱检测有人，10分钟允许最多10次呼吸频谱检测有人
                        colorflag = 'y';
						output_state = '微1';
% 						filename = strcat(num2str(output_state), '_', datestr(now, 30), '.mat');
% 						dlmwrite(filename, data_accum)
						% filename = strcat(num2str(output_state), '_', datestr(now, 30), '.xls');
						% xlswrite(filename, data_accum)
						respirationfreq_num = fix(delay_time* 2 / 16* 0.125); % 重置次数
                        delay_time_num = fix(delay_time* 2 / 16); % 重置次数                        
				    else
						delay_time_num = delay_time_num - 1;
						if delay_time_num == 0
							colorflag = 'g';
							output_state = '无人';
						end
					end
				else
					delay_time_num = delay_time_num - 1;
					if result_output{result_output_index- 1, 2} == '快1' | result_output{result_output_index- 1, 2} == '大1' | result_output{result_output_index- 1, 2} == '大0'
						output_state = '大0';
					elseif result_output{result_output_index- 1, 2} == '快1' | result_output{result_output_index- 1, 2} == '微1' | result_output{result_output_index- 1, 2} == '微0'
						output_state = '微0';
					end
					if delay_time_num == 0
                        colorflag = 'g';
						output_state = '无人';
					end
                end
				% 输出结果保存
				result_output{result_output_index, 1} = datestr(now);
				result_output{result_output_index, 2} = output_state;
				result_output_index = result_output_index + 1;
				% 数据滑窗
				data(:, 1: time_accum- 32) = data(:, time_accum- 31: time_accum);
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
			else
				% 即时检测
				if colorflag == 'y'
					if cnt <= time_accum
						raw_data_instant = data(:, cnt); % 即时数据
					else
						raw_data_instant = data(:, time_accum- 32+ cntt); % 即时数据
					end
					raw_data_MF_instant = raw_data_instant - mean(raw_data_instant); % 均值滤波
					[time_vote_instant] = time_detection_instant(raw_data_MF_instant, win_size_time_instant, stride_time_instant, time_times_instant, time_add_instant); % 时域判定
					if time_vote_instant
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
				end
			end
			cnt = cnt + 1;
		end				
	end
end

% fir filter实现过程
% 滤波器系数 fir_coef
% 滤波器阶数 fir_ord = fir_ord - 1
% data_filter = zeros(data_length, 1);
% for i = 1: fir_ord
     % for j = 1: i
        % data_filter(i, 1) = data_filter(i, 1)+ fir_coef(j)* data(i- j+ 1);
    % end
% end
% for i = fir_ord+ 1: data_length
    % for j = 1: fir_ord+ 1
        % data_filter(i, 1) = data_filter(i, 1)+ fir_coef(j)* data(i- j+ 1);
    % end
% end