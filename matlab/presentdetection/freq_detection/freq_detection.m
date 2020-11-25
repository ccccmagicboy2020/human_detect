function [freq_vote, respirationfreq_vote] = freq_detection(data, win_size_freq, stride_freq, time_accum, xhz, freq_times, respiration_times)

%{
Function Name: freq_detection
Description: ���ݴ��ھ�ֵ����Ƶ���ж����
Input: 
	data: һά����
	win_size_freq: Ƶ�򴰳�
	stride_freq: Ƶ�򲽳�
	time_accum: �ۻ�ʱ��
	xhz: ȥ��Ƶ������
	freq_times: Ƶ��˷�����
	respiration_times: ����Ƶ��˷�����
Output: None
Return: 
	freq_vote: Ƶ���ж����������ֵ��
	respirationfreq_vote: 0.5Hz��Ƶ�׹������ж����������ֵ��
%}

w = window(@hamming, length(data)); % ������
dlmwrite('w.txt', w', 'delimiter', ',', 'newline', 'pc', 'precision', '%5.3f')
X = data .* w; % �Ӵ�
dlmwrite('X.txt', X', 'delimiter', ',', 'newline', 'pc', 'precision', '%5.3f')
Y = fft(X, length(data)); % FFT
YY = abs(Y);
figure;
YY_FIX = csvread('result.csv');
plot(YY_FIX)
figure;
plot(YY)


AP_double = abs(Y)/ length(data); % ˫����
AP_single = AP_double(1: length(data)/ 2); % ������
AP_single(2: end) = 2* AP_single(2: end); % ���㵥���׷��Ȳ�ȥ����Ƶ�Ŵ�ЧӦ
AP_single_ANF = remove_pf(AP_single, time_accum, xhz); % ȥ����Ƶ����г����Χ2HzƵ��
freq = zeros((length(AP_single_ANF)- win_size_freq)/ stride_freq+ 1, 1); % ���㴰����
for i = 1: length(freq)
	freq(i, 1) = sum(abs(AP_single_ANF((i- 1)* stride_freq+ 1: (i- 1)* stride_freq+ win_size_freq))) / win_size_freq;
end
freq_vote = max(freq) > min(freq)* freq_times; % ���ݻ������ݵ������С��ֵ����Ƶ���ж�
respirationfreq_max = max(AP_single_ANF(1: (time_accum* 0.5+ 1))); % 0.5Hz��Ƶ�����ֵ
respirationfreq_mean = mean(AP_single_ANF(1: (time_accum* 0.5+ 1))); % 0.5Hz��Ƶ�׾�ֵ
respirationfreq_vote = (respirationfreq_max > min(freq)* respiration_times) || (respirationfreq_mean > min(freq)* respiration_times* 0.618); % ����0.5Hz�ڵ����ֵ��ֵ����Ƶ���ж�

end