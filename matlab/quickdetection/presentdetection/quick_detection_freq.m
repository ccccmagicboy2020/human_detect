function [freq_vote] = quick_detection_freq(data, win_size, stride, time_accum, xhz)

%{
Function Name: quick_detection_freq
Description: ���ݴ��ھ�ֵ����Ƶ���ж����
Input: 
	data: һά����
	win_size: ����
	stride: ����
Output: None
Return: 
	time_vote: Ƶ���ж����������ֵ��
%}

w = window(@hamming, length(data)); % ������
X = data .* w; % �Ӵ�
Y = fft(X, length(data)); % FFT
AP_double = abs(Y)/ length(data); % ˫����
AP_single = AP_double(1: length(data)/ 2); % ������
AP_single(2: end) = 2* AP_single(2: end); % ���㵥���׷��Ȳ�ȥ����Ƶ�Ŵ�ЧӦ
AP_single_ANF = remove_pf(AP_single, time_accum, xhz); % ȥ����Ƶ����г����Χ2HzƵ��
quick_freq = zeros((length(AP_single_ANF)- win_size)/ stride+ 1, 1); % 1�뻬�� ����������sample_rate/2 ����Ϊ0.4��
for i = 1: length(quick_freq)
	quick_freq(i, 1) = sum(abs(AP_single_ANF((i- 1)* stride+ 1: (i- 1)* stride+ win_size))) / win_size;
end
freq_vote = max(quick_freq) > min(quick_freq)* 40; % ���ݻ������ݵ������С��ֵ����Ƶ���ж�

end