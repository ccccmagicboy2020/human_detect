function freq_vote = quickfreq_detection(data_accum_MF,sample_rate)%�����˳�Ƶ������
%QUICKTIME_DETECTION �˴���ʾ�йش˺�����ժҪ
%   �˴���ʾ��ϸ˵��
	% Ƶ������
	win_size = length(data_accum_MF); % ����
	w = window(@hamming, win_size); % ������
	X = data_accum_MF .* w; % �Ӵ�
	Y = fft(X, length(data_accum_MF)); % FFT
	AP_double = abs(Y)/ length(data_accum_MF); % ˫����
	AP_single = AP_double(1: length(data_accum_MF)/ 2); % ������
	AP_single(2: end) = 2*AP_single(2: end); % ���㵥���׷��Ȳ�ȥ����Ƶ�Ŵ�ЧӦ
	% ȥ����Ƶ����
% 	AP_single_ANF = zeros(length(AP_single)- 640, 1); % ȥ����Ƶ����г����Χ2HzƵ��
	part_1 = AP_single(1: 16);
	part_3 = AP_single(400* fix(length(AP_single)/ 400)+ 16+ 1: length(AP_single));
	part_2 = zeros(400- 2*16, fix(length(AP_single)/ 400));
	for i  = 1: fix(length(AP_single)/ 400)
		part_2(:, i) = AP_single(400*(i- 1)+ 16+ 1: 400*i- 16);
	end
	part_2 = reshape(part_2, numel(part_2), 1);
	AP_single_ANF = [part_1
					 part_2
					 part_3];
	quick_freq = zeros((length(AP_single_ANF)- (sample_rate/ 2))/ 408+ 1, 1); % 1�뻬�� ����������sample_rate/2 ����Ϊ0.4��
	for i = 1: length(quick_freq)
		quick_freq(i, 1) = sum(abs(AP_single_ANF((i- 1)* 408+ 1: (i- 1)* 408+ (sample_rate/ 2)))) / (sample_rate/ 2);
	end	
	freq_vote = max(quick_freq) > min(quick_freq)* 40; % ���ݻ������ݵ������С��ֵ����Ƶ���ж�
    figure(2)
	plot(quick_freq)
	hold on
	plot(min(quick_freq)* 40* ones(1, length(quick_freq)))
	hold off
	% ʱ��
	cur_date = date;
	cur_time = fix(clock);
	str = sprintf('%s %.2d:%.2d:%.2d\n', cur_date, cur_time(4), cur_time(5), cur_time(6));
	disp(str);
    
end

