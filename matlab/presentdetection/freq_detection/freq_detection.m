function [freq_vote] = freq_detection(data, win_size_freq, stride_freq, time_accum, xhz, freq_times)

%{
Function Name: freq_detection
Description: 根据窗内均值返回频域判定结果
Input: 
	data: 一维数组
	win_size_freq: 频域窗长
	stride_freq: 频域步长
	time_accum: 累积时间
	xhz: 去除频点数量
	freq_times: 频域乘法门限
Output: None
Return: 
	freq_vote: 频域判定结果（布尔值）
%}

w = window(@hamming, length(data)); % 窗函数
dlmwrite('w.txt', w', 'delimiter', ',', 'newline', 'pc', 'precision', '%5.3f')
X = data .* w; % 加窗
dlmwrite('X.txt', X', 'delimiter', ',', 'newline', 'pc', 'precision', '%5.3f')
Y = fft(X, length(data)); % FFT
YY = abs(Y);
figure;
YY_FIX = csvread('result.csv');
plot(YY_FIX)
figure;
plot(YY)


AP_double = abs(Y)/ length(data); % 双边谱
AP_single = AP_double(1: length(data)/ 2); % 单边谱
AP_single(2: end) = 2* AP_single(2: end); % 计算单边谱幅度并去除零频放大效应
AP_single_ANF = remove_pf(AP_single, time_accum, xhz); % 去除工频及其谐波周围2Hz频点
freq = zeros((length(AP_single_ANF)- win_size_freq)/ stride_freq+ 1, 1); % 1秒滑窗 数据量等于sample_rate/2 步长为0.4秒
for i = 1: length(freq)
	freq(i, 1) = sum(abs(AP_single_ANF((i- 1)* stride_freq+ 1: (i- 1)* stride_freq+ win_size_freq))) / win_size_freq;
end
freq_vote = max(freq) > min(freq)* freq_times; % 根据滑窗数据的最大最小均值进行频域判定

end