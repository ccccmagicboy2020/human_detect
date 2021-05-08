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
fopen(s) % �򿪴���

while(1)
	%% ����
	sample_rate = 512; % ���ڰ�������
	div = 1; % ��Ƶ��
	data = zeros(sample_rate, 1); % ���ݹ���
    secnum = 8; % ����Ԥ�������ݳ��ȣ�ʱ�䣩
	time_accum = 2048/ sample_rate* secnum; % �ۻ�����
	data_accum = zeros(sample_rate* time_accum, 1); % �ۻ�����
	count = 0; % �ۻ�����
	win_size_time = sample_rate* 4; % ʱ�򴰳�
	stride_time = sample_rate* 4/ 2; % ʱ�򲽳�
	time_times = 3.8; % ʱ��˷�����
	time_add = 35; % ʱ��ӷ�����
	win_size_freq = sample_rate/ 2; % Ƶ�򴰳�
	stride_freq = 408/ 4; % Ƶ�򲽳�
	xhz = 2; % ȥ��Ƶ������
	pf = 50; % ��ƵƵ��
	freq_times = 4; % Ƶ��˷�����
	respiration_times = 16.5; % ����Ƶ��˷�����
	colorflag = 'g'; % ��ʼΪ��ɫ
	while(1)
		% ���ݻ�ȡ
		data_current = fgetl(s); % fgerl - ��ȡ�ļ��е��� ���ַ�������ʽ���� ��ɾ�����з�
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
		% ���ݴ���
		data_sample = data(1:div:end, 1); % ԭʼ���ݳ���
		num = sample_rate / div; % ԭʼ���ݳ����������
		raw_data = reshape(data_sample, num, 1); % ԭʼ���ݽ�ά
		% �����ۻ�����
		if count < time_accum
			data_accum(sample_rate* count+ 1: sample_rate* (count+ 1), 1) = raw_data;		
			count = count + 1;
			continue
		end
		% �������ݻ���
		data_accum(1: sample_rate* (time_accum- 1), 1) = data_accum(sample_rate+ 1: end, 1);
		data_accum(sample_rate* (time_accum- 1)+ 1: end, 1) = raw_data;
		% ��ֵ�˲�
		data_accum_MF = data_accum - mean(data_accum);
		data_accum_MF_sample = data_accum_MF(1:4:end, 1); % ���ݳ���
		% ʱƵ�ж�
		quick_detection_result = quick_detection(data_accum_MF, data_accum_MF_sample, win_size_time, stride_time, time_times, time_add, win_size_freq, stride_freq, secnum, xhz, pf, freq_times, respiration_times);
		if quick_detection_result
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
		% ��������
		if colorflag == 'r'
			pause(0.001)
			break
		end
	end
	%% ���ڱ��ּ��
	sample_rate = 512; % ԭʼ������
	div = 8; % ��Ƶ��
    secnum = 16; % ����Ԥ�������ݳ��ȣ�ʱ�䣩
    time_accum = 2048/ sample_rate* secnum; % �ۻ�����
	data = zeros(sample_rate, time_accum); % ���ݹ���
	cnt = 1; % ������ʼֵ
	% �������
	win_size_time = sample_rate* 4/ div; % ʱ�򴰳�
	stride_time = sample_rate* 4/ div/ 2; % ʱ�򲽳�
	time_times = 3.8; % ʱ��˷�����
	time_add = 35; % ʱ��ӷ�����
	win_size_freq = sample_rate* 4/ div/ 2; % Ƶ�򴰳�
	stride_freq = 64; % Ƶ�򲽳�
	xhz = 2; % ȥ��Ƶ������
	pf = 50; % ��ƵƵ��
	freq_times = 4; % Ƶ��˷�����
    respiration_times = 16.5; % ����Ƶ��˷�����
	bigmotion_colorflag = 'g'; % ��ʼΪ��ɫ
	% ΢�����
	% xhz = 2; % ȥ��Ƶ������
	% pf = 50; % ��ƵƵ��
	N = 300; % CFAR���ڴ�С
	pro_N = 200; % CFAR������Ԫ��С
	PAD = 10^(-8); % �龯����
	offsetmax = 0.38; % ����ƫ�ã��ϴ�
	offsetmin = 0.33; % ����ƫ�ã���С��
	rr_threshold = 0.5; % ����Ƶ�ʽ�ȡ��Χ
	micromotion_colorflag = 'g'; % ��ʼΪ��ɫ
	% �ӳ�Ԥ��
	delay_time = 64; % �ӳ�ʱ��
	delay_time_num = fix(delay_time* 2/ secnum); % ȷ�����˴���
	% ����
	respirationfreq_num = fix(delay_time* 2 / secnum* 0.125); % 0.5HzƵ�ι������������
	while(1)
		% ���ݻ�ȡ
		data_current = fgetl(s); % fgerl - ��ȡ�ļ��е��� ���ַ�������ʽ���� ��ɾ�����з�
		strlen = length(data_current);
		if strlen ~= 2562
			continue
		end
		i = 1;
		if(data_current(i) == 0x42)
			cntt = mod(cnt, 32); % ȡ��
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
			% ���ݴ���
			if cnt >= time_accum && mod(cnt, 32) == 0
				data_Samples = data(1:div:end, :); % ������
				num = sample_rate / div * time_accum; % ���ݳ����������         
				raw_data = reshape(data_Samples, num, 1); % ԭʼ���ݽ�ά
				raw_data_MF = raw_data - mean(raw_data); % ��ֵ�˲�
				% �������
				bigmotion_time_vote = time_detection(raw_data_MF, win_size_time, stride_time, time_times, time_add); % ʱ���ж�
				[bigmotion_freq_vote, respirationfreq_vote] = freq_detection(raw_data_MF, win_size_freq, stride_freq, secnum, xhz, pf, freq_times, respiration_times); % Ƶ���ж�
				if bigmotion_time_vote && bigmotion_freq_vote
					bigmotion_colorflag = 'r';
				else
					bigmotion_colorflag = 'g';
				end
				% ΢�����
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
				% ���ڱ��ּ���ж�
                if bigmotion_colorflag == 'r'
					colorflag = 'r';
					respirationfreq_num = fix(delay_time* 2 / 16* 0.125); % ���ô���
					delay_time_num = fix(delay_time* 2 / 16); % ���ô���
				elseif micromotion_colorflag == 'r'
					colorflag = 'y';
					respirationfreq_num = fix(delay_time* 2 / 16* 0.125); % ���ô���
					delay_time_num = fix(delay_time* 2 / 16); % ���ô���
				elseif not(bigmotion_time_vote) && not(bigmotion_freq_vote) && respirationfreq_vote
					colorflag = 'y';
					respirationfreq_num = fix(delay_time* 2 / 16* 0.125); % ���ô���
					delay_time_num = fix(delay_time* 2 / 16); % ���ô���
				elseif not(bigmotion_time_vote) && bigmotion_freq_vote && respirationfreq_vote
					respirationfreq_num = respirationfreq_num - 1;
					if(respirationfreq_num < 0) % 64���������1�κ���Ƶ�׼�����ˣ�3�����������3�κ���Ƶ�׼�����ˣ�5�����������5�κ���Ƶ�׼�����ˣ�10�����������10�κ���Ƶ�׼������
                        colorflag = 'y';
						respirationfreq_num = fix(delay_time* 2 / 16* 0.125); % ���ô���
                        delay_time_num = fix(delay_time* 2 / 16); % ���ô���                        
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
				data(:, 1: time_accum- 32) = data(:, time_accum- 31: time_accum); % ���ݻ���
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
			end
			cnt = cnt + 1;
		end				
	end
end