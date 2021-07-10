function [time_vote_instant] = time_detection_instant(data_instant, win_size_time_instant, stride_time_instant, time_times_instant, time_add_instant)

%{
Function Name: time_detection_instant
Description: ���ݴ��ڱ�׼���ʱ���ж����
Input: 
	data_instant: ԭʼ����
	win_size_time_instant: ʱ�򴰳�
	stride_time_instant: ʱ�򲽳�
	time_times_instant: ʱ��˷�����
	time_add_instant: ʱ��ӷ�����
Output: None
Return:
	time_vote_instant: ʱ���ж����������ֵ��
%}

time_instant = zeros((length(data_instant)- win_size_time_instant) / stride_time_instant+ 1, 1); % ���㴰����
for i = 1: length(time_instant)
	time_instant(i, 1) = std(data_instant((i- 1)* stride_time_instant+ 1: (i- 1)* stride_time_instant+ win_size_time_instant)); % ���ڱ�׼��
end
time_vote_instant = max(time_instant) > min(min(time_instant)* time_times_instant, min(time_instant)+ time_add_instant); % ���ݻ������ݵ������С��׼�����ʱ���ж�

end