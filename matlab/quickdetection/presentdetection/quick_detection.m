%% ���Ӧ

% �رղ�ɾ����ռ�ö˿�
if ~isempty(instrfind)
		fclose(instrfind);
		delete(instrfind);
end
% �˿�����
s = serial('com31'); % �������ж˿ڶ���
%set(s, 'BaudRate', 115200, 'StopBits', 1, 'Parity', 'none', 'DataBits', 8, 'InputBufferSize', 10250, 'ReadAsyncMode', 'continuous'); % ���� ������ ֹͣλ У�鷽ʽ ����λ ���뻺���С���ֽڣ� �첽��ȡ
set(s, 'BaudRate', 128000, 'StopBits', 1, 'Parity', 'none', 'DataBits', 8, 'InputBufferSize', 10250, 'ReadAsyncMode', 'continuous'); % ���� ������ ֹͣλ У�鷽ʽ ����λ ���뻺���С���ֽڣ� �첽��ȡ
fopen(s); % �򿪴���

% ����Ԥ��
sample_rate = 2048; % ԭʼ������
div = 1; % ��Ƶ��
fs = sample_rate / div; % ʵ�ʲ�����
T = 1 / fs; % ʵ�ʲ������
data = zeros(sample_rate, 1); % ���ݹ���
time_accum = 8; % �ۻ�ʱ��
data_accum = zeros(sample_rate* time_accum, 1); % �ۻ�����
count = 0; % �ۻ�����
win_size_time = sample_rate; % ʱ�򴰳�
stride_time = sample_rate/ 2; % ʱ�򲽳�
win_size_freq = sample_rate/ 2; % Ƶ�򴰳�
stride_freq = 408; % Ƶ�򲽳�
xhz = 2; % ȥ��Ƶ������
colorflag = 'g'; % ��ʼΪ��ɫ

while(1)
	data_current = fgetl(s); % fgerl - ��ȡ�ļ��е��� ���ַ�������ʽ���� ��ɾ�����з�
	strlen = length(data_current);
	if strlen ~= 10250
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
	% ���ݴ���
	data_Samples = data(1:div:end, 1); % ԭʼ���ݳ���
	num = sample_rate / div; % ԭʼ���ݳ����������
	raw_data = reshape(data_Samples, num, 1); % ԭʼ���ݽ�ά
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
	% ʱ���ж�
	time_vote = quick_detection_time(data_accum_MF, win_size_time, stride_time);
	% Ƶ���ж�
	freq_vote = quick_detection_freq(data_accum_MF, win_size_freq, stride_freq, time_accum, xhz);
	% �����ж�
	if time_vote && freq_vote
		colorflag = 'r';
	end
% ͼ����ʾ
if colorflag == 'r'
	figure(10)
	alpha = 0: pi/20: 2*pi; % �Ƕ�[0, 2*pi]
	R = 2; % �뾶
	xx = R* cos(alpha);
	yy = R* sin(alpha);
	plot(xx, yy, '-')
	axis equal
	fill(xx, yy, colorflag); % �ú�ɫ���
	break	
end
end