function [vote_result] = quick_detection(data, win_size_time, stride_time, time_times, time_add, win_size_freq, stride_freq, time_accum, xhz, freq_times)

%{
Function Name: vote
Description: ����ʱƵ��ͶƱ���������ж����
Input:
	data: һά����
	win_size_time: ʱ�򴰳�
	stride_time: ʱ�򲽳�
	time_times: ʱ��˷�����
	time_add: ʱ��ӷ�����
	win_size_freq: Ƶ�򴰳�
	stride_freq: Ƶ�򲽳�
	time_accum: �ۻ�ʱ��
	xhz: ȥ��Ƶ������
	freq_times: Ƶ��˷�����
Output: None
Return: 
	vote_result: �����ж����������ֵ��
%}

time_vote = time_detection(data, win_size_time, stride_time, time_times, time_add); % ʱ���ж�
freq_vote = freq_detection(data, win_size_freq, stride_freq, time_accum, xhz, freq_times); % Ƶ���ж�
vote_result = time_vote && freq_vote;

end