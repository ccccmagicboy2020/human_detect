function [freq_vote, respirationfreq_vote] = freq_detection(data, win_size_freq, stride_freq, time_accum, xhz, freq_times, respiration_times)

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
	respiration_times: 呼吸频域乘法门限
Output: None
Return: 
	freq_vote: 频域判定结果（布尔值）
	respirationfreq_vote: 0.5Hz内频谱过门限判定结果（布尔值）
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
freq = zeros((length(AP_single_ANF)- win_size_freq)/ stride_freq+ 1, 1); % 计算窗数量
for i = 1: length(freq)
	freq(i, 1) = sum(abs(AP_single_ANF((i- 1)* stride_freq+ 1: (i- 1)* stride_freq+ win_size_freq))) / win_size_freq;
end
freq_vote = max(freq) > min(freq)* freq_times; % 根据滑窗数据的最大最小均值进行频域判定
respirationfreq_max = max(AP_single_ANF(1: (time_accum* 0.5+ 1))); % 0.5Hz内频谱最大值
respirationfreq_mean = mean(AP_single_ANF(1: (time_accum* 0.5+ 1))); % 0.5Hz内频谱均值
respirationfreq_vote = (respirationfreq_max > min(freq)* respiration_times) || (respirationfreq_mean > min(freq)* respiration_times* 0.618); % 根据0.5Hz内的最大值均值进行频域判定

end