function [freq_vote, respirationfreq_vote] = freq_detection(data, div, win_size_freq, stride_freq, time_accum, xhz, pf, freq_times, respiration_times)

%{
Function Name: freq_detection
Description: ���ݴ��ھ�ֵ����Ƶ���ж����
Input: 
	data: ԭʼ����
	div: ��Ƶ��
	win_size_freq: Ƶ�򴰳�
	stride_freq: Ƶ�򲽳�
	time_accum: �ۻ�ʱ��
	xhz: ȥ��Ƶ������
	pf: ��ƵƵ��
	freq_times: Ƶ��˷�����
	respiration_times: ����Ƶ��˷�����
Output: None
Return: 
	freq_vote: Ƶ���ж����������ֵ��
	respirationfreq_vote: 0.5Hz��Ƶ�׹������ж����������ֵ��
%}

fs = 2000; % ������
w = window(@hamming, length(data)); % ������
X = data.* w; % �Ӵ�
X = fft(X, length(data)); % FFT
AP_double = abs(X)/ length(data); % ˫����
AP_single = AP_double(1: length(data)/ 2); % ������
AP_single(2: end) = 2* AP_single(2: end); % ���㵥���׷��Ȳ�ȥ����Ƶ�Ŵ�ЧӦ
AP_single_ANF = remove_pf(AP_single, time_accum, xhz, pf); % ȥ����Ƶ����г����Χ2HzƵ��
freq = zeros((length(AP_single_ANF)- win_size_freq)/ stride_freq+ 1, 1); % ���㴰����
for i = 1: length(freq)
	freq(i, 1) = sum(abs(AP_single_ANF((i- 1)* stride_freq+ 1: (i- 1)* stride_freq+ win_size_freq))) / win_size_freq;
end
freq_vote = max(freq) > min(freq)* freq_times; % ���ݻ������ݵ������С��ֵ����Ƶ���ж�
data_czt = czt(data.* w, length(data), exp(-1i* 2* pi* (0.5- 0.1)/ (length(data)* fs/ div)), exp(1i* 2* pi* 0.1/ (fs/ div))); % Chirp Z-transform
respirationfreq_max = max(abs(data_czt))/ (length(data)/ 2); % 0.5Hz��Ƶ�����ֵ
respirationfreq_mean = mean(abs(data_czt))/ (length(data)/ 2); % 0.5Hz��Ƶ�׾�ֵ
respirationfreq_vote = (respirationfreq_max > min(freq)* respiration_times) || (respirationfreq_mean > min(freq)* respiration_times* 0.618); % ����0.5Hz�ڵ����ֵ��ֵ����Ƶ���ж�

figure(4)
AP_single_x = 0: fs/ div/ length(data): (length(AP_single)- 1)* fs/ div/ length(data);
plot(AP_single_x, AP_single)
xlabel('Frequency (Hz)')
ylabel('Amplitude')
title('�ź�Ƶ��')
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
title('�ź�Ƶ����')

end