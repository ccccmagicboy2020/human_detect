close all
clearvars
clear
clc

% �رղ�ɾ����ռ�ö˿�
if ~isempty(instrfind)
		fclose(instrfind);
		delete(instrfind);
end
% �˿�����
s = serial('com3'); % �������ж˿ڶ���
set(s, 'BaudRate', 115200, 'StopBits', 1, 'Parity', 'none', 'DataBits', 8, 'InputBufferSize', 2562, 'ReadAsyncMode', 'continuous') % ���� ������ ֹͣλ У�鷽ʽ ����λ ���뻺���С���ֽڣ� �첽��ȡ
fopen(s); % �򿪴���

fs = 2000; % ������
data_package = 512; % ���ڰ�������
result_output_index = 1; % ����������
result_output = cell(8192, 2); % �������ռ�

while(1)
	%% ����
	div = 4; % ��Ƶ��
	secnum = 8.192; % ����Ԥ�������ݳ��ȣ�ʱ�䣩
	time_accum = fs/ data_package* secnum; % �ۻ�����
	data = zeros(data_package, 1); % ���ݹ���
	data_accum = zeros(data_package* time_accum, 1); % �ۻ�����
	count = 0; % �ۻ�����
	win_size_time = data_package* 4; % ʱ�򴰳�
	stride_time = data_package* 4/ 2; % ʱ�򲽳�
	time_times = 3.8; % ʱ��˷�����
	time_add = 35; % ʱ��ӷ�����
	win_size_freq = data_package* 4/ div/ 2; % Ƶ�򴰳�
	stride_freq = 82; % Ƶ�򲽳�
	xhz_b = 6; % ȥ��Ƶ������
	pf = 50; % ��ƵƵ��
	freq_times = 4; % Ƶ��˷�����
	respiration_times = 28; % ����Ƶ��˷�����
	while(1)
		% ���ݻ�ȡ
		data_current = fgetl(s); % fgerl - ��ȡ�ļ��е��� ���ַ�������ʽ���� ��ɾ�����з�
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
		% �����ۻ�����
		if count < time_accum
			data_accum(data_package* count+ 1: data_package* (count+ 1), 1) = data;		
			count = count + 1;
			continue
		end
		% �������ݻ���
		data_accum(1: data_package* (time_accum- 1), 1) = data_accum(data_package+ 1: end, 1);
		data_accum(data_package* (time_accum- 1)+ 1: end, 1) = data;
		% ���ݴ���
		raw_data = data_accum; % ԭʼ����
		raw_data_MF = raw_data - mean(raw_data); % ��ֵ�˲�
		raw_data_MF_LP = filter(lowpass_filter(2000, 250, 300), raw_data_MF); % ��ͨ�˲�
		raw_data_MF_LP_DS = raw_data_MF_LP(1:div:end, :); % ���ݽ�����
		% ʱƵ�ж�
		quick_detection_result = quick_detection(raw_data_MF, raw_data_MF_LP_DS, div, win_size_time, stride_time, time_times, time_add, win_size_freq, stride_freq, fix(secnum), xhz_b, pf, freq_times, respiration_times);
		if quick_detection_result
			colorflag = 'r';
			output_state = '��1';
			filename = strcat(num2str(output_state), '_', datestr(now, 30), '.mat');
			dlmwrite(filename, data_accum)
			% filename = strcat(num2str(output_state), '_', datestr(now, 30), '.xls');
			% xlswrite(filename, data_accum)
		else
			colorflag = 'g';
			output_state = '��0';
		end
		% ����������
% 		result_output{result_output_index, 1} = datestr(now);
% 		result_output{result_output_index, 2} = output_state;
% 		result_output_index = result_output_index + 1;
		% ͼ����ʾ
		figure(1)
		alpha = 0: pi/20: 2*pi; % �Ƕ�[0, 2*pi]
		R = 2; % �뾶
		xx = R* cos(alpha);
		yy = R* sin(alpha);
		plot(xx, yy, '-')
		axis equal
		fill(xx, yy, colorflag); % ��ɫ���
		% ��������
		if colorflag == 'r'
			pause(0.001)
			break
		end
	end
	%% ���ڱ��ּ��
	div = 8; % ��Ƶ��
    secnum = 16.384; % ����Ԥ�������ݳ��ȣ�ʱ�䣩
    time_accum = fs/ data_package* secnum; % �ۻ�����
	data = zeros(data_package, time_accum); % ���ݹ���
	cnt = 1; % ������ʼֵ
	N_sigma = 3.2; % ���ޱ���
	nbins = 100; % bin����
	N_interval = 100; % �������
	% �������
	win_size_time = data_package* 4/ div; % ʱ�򴰳�
	stride_time = data_package* 4/ div/ 2; % ʱ�򲽳�
	time_times = 3.8; % ʱ��˷�����
	time_add = 35; % ʱ��ӷ�����
	win_size_freq = data_package* 4/ div/ 2; % Ƶ�򴰳�
	stride_freq = 44; % Ƶ�򲽳�
	xhz_b = 8; % ȥ��Ƶ������
	pf = 50; % ��ƵƵ��
	freq_times = 4; % Ƶ��˷�����
    respiration_times = 30; % ����Ƶ��˷�����
	% ΢�����
	xhz_m = 4; % ȥ��Ƶ������
	% pf = 50; % ��ƵƵ��
	N = 300; % CFAR���ڴ�С
	pro_N = 200; % CFAR������Ԫ��С
	PAD = 10^(-8); % �龯����
	offsetmax = 0.45; % ����ƫ�ã��ϴ�
	offsetmin = 0.45; % ����ƫ�ã���С��
	rr_threshold = 0.5; % ����Ƶ�ʽ�ȡ��Χ
	% ��������Ƶ�ʼ��
	m = 1024; % �任����
	% �ӳ�Ԥ��
	delay_time = 32; % �ӳ�ʱ��
	delay_time_num = round(delay_time* 2/ secnum); % ȷ�����˴���
	% ����Ԥ��
	respirationfreq_num = round(delay_time* 2 / secnum* 0.125); % 0.5HzƵ�ι������������
	% ��ʱ���
	div_instant = 4; % ��Ƶ��
	win_size_time_instant = data_package/ div_instant; % ʱ�򴰳�
	stride_time_instant = data_package/ div_instant/ 2; % ʱ�򲽳�
	time_times_instant = 3.8; % ʱ��˷�����
	time_add_instant = 35; % ʱ��ӷ�����
	while(1)
		% ���ݻ�ȡ
		data_current = fgetl(s); % fgerl - ��ȡ�ļ��е��� ���ַ�������ʽ���� ��ɾ�����з�
		strlen = length(data_current);
		if strlen ~= 2562
			continue
		end
		i = 1;
		if(data_current(i) == 'B')
			cntt = mod(cnt, 32); % ȡ��
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
			% ���ݴ���
			% ���ڱ����ж�
			if cnt >= time_accum && mod(cnt, 32) == 0
				raw_data = reshape(data, data_package* time_accum, 1); % ԭʼ����
				raw_data_MF = raw_data - mean(raw_data); % ��ֵ�˲�
				raw_data_MF_LP = filter(lowpass_filter(2000, 125, 150), raw_data_MF); % ��ͨ�˲�
				raw_data_MF_LP_DS = raw_data_MF_LP(1:div:end, :); % ���ݽ�����
				raw_data_MF_LP_DS_RO = remove_outliers(raw_data_MF_LP_DS, N_sigma, nbins, N_interval); % ȥ����Ⱥ��
				% �������
				bigmotion_time_vote = time_detection(raw_data_MF_LP_DS_RO, win_size_time, stride_time, time_times, time_add); % ʱ���ж�
				[bigmotion_freq_vote, respirationfreq_vote] = freq_detection(raw_data_MF_LP_DS_RO, div, win_size_freq, stride_freq, fix(secnum), xhz_b, pf, freq_times, respiration_times); % Ƶ���ж�
				% ΢�����
				if bigmotion_freq_vote
					offset = offsetmax;
				else
					offset = offsetmin;
				end
				micromotion_vote = micromotion_detection(raw_data_MF_LP_DS_RO, fix(secnum), xhz_m, pf, N, pro_N, PAD, offset, rr_threshold);
				% ��������Ƶ�ʼ��
				[respiratory_rate, heart_rate] = respiratory_heart_rate_monitoring(raw_data_MF_LP_DS_RO, m, respirationfreq_vote, micromotion_vote);
				% ���ڱ��ּ���ж�
                if bigmotion_time_vote && bigmotion_freq_vote
					colorflag = 'r';
					output_state = '��1';
% 					filename = strcat(num2str(output_state), '_', datestr(now, 30), '.mat');
% 					dlmwrite(filename, data_accum)
					% filename = strcat(num2str(output_state), '_', datestr(now, 30), '.xls');
					% xlswrite(filename, data_accum)
					respirationfreq_num = fix(delay_time* 2 / 16* 0.125); % ���ô���
					delay_time_num = fix(delay_time* 2 / 16); % ���ô���
				elseif micromotion_vote
					colorflag = 'y';
					output_state = '΢1';
% 					filename = strcat(num2str(output_state), '_', datestr(now, 30), '.mat');
% 					dlmwrite(filename, data_accum)
					% filename = strcat(num2str(output_state), '_', datestr(now, 30), '.xls');
					% xlswrite(filename, data_accum)
					respirationfreq_num = fix(delay_time* 2 / 16* 0.125); % ���ô���
					delay_time_num = fix(delay_time* 2 / 16); % ���ô���
				elseif not(bigmotion_time_vote) && not(bigmotion_freq_vote) && respirationfreq_vote
					colorflag = 'y';
					output_state = '΢1';
% 					filename = strcat(num2str(output_state), '_', datestr(now, 30), '.mat');
% 					dlmwrite(filename, data_accum)
					% filename = strcat(num2str(output_state), '_', datestr(now, 30), '.xls');
					% xlswrite(filename, data_accum)
					respirationfreq_num = fix(delay_time* 2 / 16* 0.125); % ���ô���
					delay_time_num = fix(delay_time* 2 / 16); % ���ô���
				elseif not(bigmotion_time_vote) && bigmotion_freq_vote && respirationfreq_vote
					respirationfreq_num = respirationfreq_num - 1;
					if(respirationfreq_num < 0) % 64���������1�κ���Ƶ�׼�����ˣ�3�����������3�κ���Ƶ�׼�����ˣ�5�����������5�κ���Ƶ�׼�����ˣ�10�����������10�κ���Ƶ�׼������
                        colorflag = 'y';
						output_state = '΢1';
% 						filename = strcat(num2str(output_state), '_', datestr(now, 30), '.mat');
% 						dlmwrite(filename, data_accum)
						% filename = strcat(num2str(output_state), '_', datestr(now, 30), '.xls');
						% xlswrite(filename, data_accum)
						respirationfreq_num = fix(delay_time* 2 / 16* 0.125); % ���ô���
                        delay_time_num = fix(delay_time* 2 / 16); % ���ô���                        
				    else
						delay_time_num = delay_time_num - 1;
						if delay_time_num == 0
							colorflag = 'g';
							output_state = '����';
						end
					end
				else
					delay_time_num = delay_time_num - 1;
					if result_output{result_output_index- 1, 2} == '��1' | result_output{result_output_index- 1, 2} == '��1' | result_output{result_output_index- 1, 2} == '��0'
						output_state = '��0';
					elseif result_output{result_output_index- 1, 2} == '��1' | result_output{result_output_index- 1, 2} == '΢1' | result_output{result_output_index- 1, 2} == '΢0'
						output_state = '΢0';
					end
					if delay_time_num == 0
                        colorflag = 'g';
						output_state = '����';
					end
                end
				% ����������
				result_output{result_output_index, 1} = datestr(now);
				result_output{result_output_index, 2} = output_state;
				result_output_index = result_output_index + 1;
				% ���ݻ���
				data(:, 1: time_accum- 32) = data(:, time_accum- 31: time_accum);
				% ͼ����ʾ
				figure(1)
				alpha = 0: pi/20: 2*pi; % �Ƕ�[0, 2*pi]
				R = 2; % �뾶
				xx = R* cos(alpha);
				yy = R* sin(alpha);
				plot(xx, yy, '-')
				axis equal
				fill(xx, yy, colorflag); % ��ɫ���
				% �������ڱ��ּ��
				if colorflag == 'g'
					pause(0.001)
					break
				end
			else
				% ��ʱ���
				if colorflag == 'y'
					if cnt <= time_accum
						raw_data_instant = data(:, cnt); % ��ʱ����
					else
						raw_data_instant = data(:, time_accum- 32+ cntt); % ��ʱ����
					end
					raw_data_MF_instant = raw_data_instant - mean(raw_data_instant); % ��ֵ�˲�
					[time_vote_instant] = time_detection_instant(raw_data_MF_instant, win_size_time_instant, stride_time_instant, time_times_instant, time_add_instant); % ʱ���ж�
					if time_vote_instant
						colorflag = 'r';
					end
					% ͼ����ʾ
					figure(1)
					alpha = 0: pi/20: 2*pi; % �Ƕ�[0, 2*pi]
					R = 2; % �뾶
					xx = R* cos(alpha);
					yy = R* sin(alpha);
					plot(xx, yy, '-')
					axis equal
					fill(xx, yy, colorflag); % ��ɫ���
				end
			end
			cnt = cnt + 1;
		end				
	end
end

% fir filterʵ�ֹ���
% �˲���ϵ�� fir_coef
% �˲������� fir_ord = fir_ord - 1
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