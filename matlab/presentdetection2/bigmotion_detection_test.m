%% �������

% �رղ�ɾ����ռ�ö˿�
if ~isempty(instrfind)
		fclose(instrfind);
		delete(instrfind);
end
% �˿�����
s = serial('com3'); % �������ж˿ڶ���
set(s, 'BaudRate', 115200, 'StopBits', 1, 'Parity', 'none', 'DataBits', 8, 'InputBufferSize', 10250, 'ReadAsyncMode', 'continuous'); % ���� ������ ֹͣλ У�鷽ʽ ����λ ���뻺���С���ֽڣ� �첽��ȡ
fopen(s); % �򿪴���

% ����Ԥ��
sample_rate = 2048; % ԭʼ������
div = 8; % ��Ƶ��
secnum = 16; % ����Ԥ�������ݳ��ȣ�ʱ�䣩
data = zeros(sample_rate, secnum); % ���ݹ���
cnt = 1; % ������ʼֵ
win_size_time = sample_rate/ div; % ʱ�򴰳�
stride_time = sample_rate/ div/ 2; % ʱ�򲽳�
time_times = 5; % ʱ��˷�����
time_add = 50; % ʱ��ӷ�����
win_size_freq = sample_rate/ div/ 2; % Ƶ�򴰳�
stride_freq = 64; % Ƶ�򲽳�
xhz = 2; % ȥ��Ƶ������
freq_times = 40; % Ƶ��˷�����
respiration_times = 22; % ����Ƶ��˷�����
delay_time = 32; % �ӳ�ʱ��
delay_time_num = fix(delay_time* 2/ secnum); % ȷ�����˴���
colorflag = 'g'; % ��ʼΪ��ɫ

while(1)
	% ���ݻ�ȡ
	data_current = fgetl(s); % fgerl - ��ȡ�ļ��е��� ���ַ�������ʽ���� ��ɾ�����з�
	strlen = length(data_current);
	if strlen ~= 10249
		continue
	end
	i = 1;
	if(data_current(i) == 0x42 && data_current(i+ 1) == 0x65 && data_current(i+ 2) == 0x67 && data_current(i+ 3) == 0x69 && data_current(i+ 4) == 0x6E)
		cntt = mod(cnt, 8); % ȡ��
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
		% ���ݴ���
		if cnt >= secnum && mod(cnt, 8) == 0
			data_sample = data(1:div:end, :); % ������
			num = sample_rate / div * secnum; % ���ݳ����������         
			raw_data = reshape(data_sample, num, 1); % ԭʼ���ݽ�ά
			raw_data_MF = raw_data - mean(raw_data); % ��ֵ�˲�
			% �������
			bigmotion_time_vote = time_detection(raw_data_MF, win_size_time, stride_time, time_times, time_add); % ʱ���ж�
			[bigmotion_freq_vote, respirationfreq_vote] = freq_detection(raw_data_MF, win_size_freq, stride_freq, secnum, xhz, freq_times, respiration_times); % Ƶ���ж�
			if bigmotion_time_vote && bigmotion_freq_vote
				colorflag = 'r';
				delay_time_num = fix(delay_time* 2/ secnum); % ���ô���
			else
				delay_time_num = delay_time_num - 1;
				if delay_time_num == 0
					colorflag = 'g';
				else
					colorflag = 'r';
				end
			end
			data(:, 1: secnum- 8) = data(:, secnum- 7: secnum); % ���ݻ���       
		end
		cnt = cnt+ 1;
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