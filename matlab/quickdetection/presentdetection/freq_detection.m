function [freq_vote] = freq_detection(data, win_size_freq, stride_freq, time_accum, xhz, freq_times)

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
Output: None
Return: 
	freq_vote: Ƶ���ж����������ֵ��
%}

w = window(@hamming, length(data)); % ������
X = data .* w; % �Ӵ�
Y = fft(X, length(data)); % FFT
AP_double = abs(Y)/ length(data); % ˫����
AP_single = AP_double(1: length(data)/ 2); % ������
AP_single(2: end) = 2* AP_single(2: end); % ���㵥���׷��Ȳ�ȥ����Ƶ�Ŵ�ЧӦ
AP_single_ANF = remove_pf(AP_single, time_accum, xhz); % ȥ����Ƶ����г����Χ2HzƵ��
freq = zeros((length(AP_single_ANF)- win_size_freq)/ stride_freq+ 1, 1); % 1�뻬�� ����������sample_rate/2 ����Ϊ0.4��
for i = 1: length(freq)
	freq(i, 1) = sum(abs(AP_single_ANF((i- 1)* stride_freq+ 1: (i- 1)* stride_freq+ win_size_freq))) / win_size_freq;
end
freq_vote = max(freq) > min(freq)* freq_times; % ���ݻ������ݵ������С��ֵ����Ƶ���ж�

end