function freq_vote = quickfreq_detection(data_accum_MF,sample_rate)%补充滤除频点输入
%QUICKTIME_DETECTION 此处显示有关此函数的摘要
%   此处显示详细说明
	% 频域数据
	win_size = length(data_accum_MF); % 窗长
	w = window(@hamming, win_size); % 窗函数
	X = data_accum_MF .* w; % 加窗
	Y = fft(X, length(data_accum_MF)); % FFT
	AP_double = abs(Y)/ length(data_accum_MF); % 双边谱
	AP_single = AP_double(1: length(data_accum_MF)/ 2); % 单边谱
	AP_single(2: end) = 2*AP_single(2: end); % 计算单边谱幅度并去除零频放大效应
	% 去除工频干扰
% 	AP_single_ANF = zeros(length(AP_single)- 640, 1); % 去除工频及其谐波周围2Hz频点
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
	quick_freq = zeros((length(AP_single_ANF)- (sample_rate/ 2))/ 408+ 1, 1); % 1秒滑窗 数据量等于sample_rate/2 步长为0.4秒
	for i = 1: length(quick_freq)
		quick_freq(i, 1) = sum(abs(AP_single_ANF((i- 1)* 408+ 1: (i- 1)* 408+ (sample_rate/ 2)))) / (sample_rate/ 2);
	end	
	freq_vote = max(quick_freq) > min(quick_freq)* 40; % 根据滑窗数据的最大最小均值进行频域判定
    figure(2)
	plot(quick_freq)
	hold on
	plot(min(quick_freq)* 40* ones(1, length(quick_freq)))
	hold off
	% 时间
	cur_date = date;
	cur_time = fix(clock);
	str = sprintf('%s %.2d:%.2d:%.2d\n', cur_date, cur_time(4), cur_time(5), cur_time(6));
	disp(str);
    
end

