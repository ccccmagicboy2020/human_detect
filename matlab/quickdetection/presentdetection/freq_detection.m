function [freq_vote] = quick_detection_freq(data, win_size, stride, time_accum, xhz)

%{
Function Name: quick_detection_freq
Description: 根据窗内均值返回频域判定结果
Input: 
	data: 一维数组
	win_size: 窗长
	stride: 步长
Output: None
Return: 
	time_vote: 频域判定结果（布尔值）
%}

w = window(@hamming, length(data)); % 窗函数
X = data .* w; % 加窗
Y = fft(X, length(data)); % FFT
AP_double = abs(Y)/ length(data); % 双边谱
AP_single = AP_double(1: length(data)/ 2); % 单边谱
AP_single(2: end) = 2* AP_single(2: end); % 计算单边谱幅度并去除零频放大效应
AP_single_ANF = remove_pf(AP_single, time_accum, xhz); % 去除工频及其谐波周围2Hz频点
quick_freq = zeros((length(AP_single_ANF)- win_size)/ stride+ 1, 1); % 1秒滑窗 数据量等于sample_rate/2 步长为0.4秒
for i = 1: length(quick_freq)
	quick_freq(i, 1) = sum(abs(AP_single_ANF((i- 1)* stride+ 1: (i- 1)* stride+ win_size))) / win_size;
end
freq_vote = max(quick_freq) > min(quick_freq)* 40; % 根据滑窗数据的最大最小均值进行频域判定

end