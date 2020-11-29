function [quick_detection_result] = quick_detection(data, data_sample, win_size_time, stride_time, time_times, time_add, win_size_freq, stride_freq, time_accum, xhz, freq_times, respiration_times)

%{
Function Name: quick_detection
Description: ����
Input:
	data: һά����
	data_sample: һά�������
	win_size_time: ʱ�򴰳�
	stride_time: ʱ�򲽳�
	time_times: ʱ��˷�����
	time_add: ʱ��ӷ�����
	win_size_freq: Ƶ�򴰳�
	stride_freq: Ƶ�򲽳�
	time_accum: �ۻ�ʱ��
	xhz: ȥ��Ƶ������
	freq_times: Ƶ��˷�����
	respiration_times: ����Ƶ��˷�����
Output: None
Return: 
	quick_detection_result: �����ж����������ֵ��
%}

time_vote = time_detection(data, win_size_time, stride_time, time_times, time_add); % ʱ���ж�
if time_vote
	[freq_vote, respirationfreq_vote] = freq_detection(data_sample, win_size_freq, stride_freq, time_accum, xhz, freq_times, respiration_times); % Ƶ���ж�
	if freq_vote
		quick_detection_result = 1;
	else
		quick_detection_result = 0;
	end
else
	quick_detection_result = 0;
end

end