function [freq_vote, respirationfreq_vote] = freq_detection(data, div, win_size_freq, stride_freq, time_accum, xhz, pf, freq_times, respiration_times)

%{
Function Name: freq_detection
Description: 根据窗内均值返回频域判定结果
Input: 
	data: 原始数据
	div: 分频数
	win_size_freq: 频域窗长
	stride_freq: 频域步长
	time_accum: 累积时间
	xhz: 去除频点数量
	pf: 工频频点
	freq_times: 频域乘法门限
	respiration_times: 呼吸频域乘法门限
Output: None
Return: 
	freq_vote: 频域判定结果（布尔值）
	respirationfreq_vote: 0.5Hz内频谱过门限判定结果（布尔值）
%}

fs = 2000; % 采样率
w = window(@hamming, length(data)); % 窗函数
X = data.* w; % 加窗
X = fft(X, length(data)); % FFT
AP_double = abs(X)/ length(data); % 双边谱
AP_single = AP_double(1: length(data)/ 2); % 单边谱
AP_single(2: end) = 2* AP_single(2: end); % 计算单边谱幅度并去除零频放大效应
AP_single_ANF = remove_pf(AP_single, time_accum, xhz, pf); % 去除工频及其谐波周围2Hz频点
freq = zeros((length(AP_single_ANF)- win_size_freq)/ stride_freq+ 1, 1); % 计算窗数量
for i = 1: length(freq)
	freq(i, 1) = sum(abs(AP_single_ANF((i- 1)* stride_freq+ 1: (i- 1)* stride_freq+ win_size_freq))) / win_size_freq;
end
freq_vote = max(freq) > min(freq)* freq_times; % 根据滑窗数据的最大最小均值进行频域判定
data_czt = czt(data.* w, length(data), exp(-1i* 2* pi* (0.5- 0.1)/ (length(data)* fs/ div)), exp(1i* 2* pi* 0.1/ (fs/ div))); % Chirp Z-transform
respirationfreq_max = max(abs(data_czt))/ (length(data)/ 2); % 0.5Hz内频谱最大值
respirationfreq_mean = mean(abs(data_czt))/ (length(data)/ 2); % 0.5Hz内频谱均值
respirationfreq_vote = (respirationfreq_max > min(freq)* respiration_times) || (respirationfreq_mean > min(freq)* respiration_times* 0.618); % 根据0.5Hz内的最大值均值进行频域判定

figure(4)
AP_single_x = 0: fs/ div/ length(data): (length(AP_single)- 1)* fs/ div/ length(data);
plot(AP_single_x, AP_single)
xlabel('Frequency (Hz)')
ylabel('Amplitude')
title('信号频域')
figure(5)
plot(freq, 'LineWidth', 2)
hold on
plot(min(freq)* freq_times* ones(length(freq)), 'r*')
hold on
plot(respirationfreq_max* ones(length(freq)), 'b', 'LineWidth', 2)
hold on
plot(respirationfreq_mean* ones(length(freq)), 'b', 'LineWidth', 2)
hold on
plot(min(freq)* respiration_times* ones(length(freq)), 'r', 'LineWidth', 2)
hold on
plot(min(freq)* respiration_times* 0.618* ones(length(freq)), 'r', 'LineWidth', 2)
hold off
title('信号频域检测')

end