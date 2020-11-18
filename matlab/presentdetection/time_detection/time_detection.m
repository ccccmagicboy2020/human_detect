function [time_vote] = time_detection(data, win_size_time, stride_time, time_times, time_add)

%{
Function Name: time_detection
Description: ���ݴ��ڱ�׼���ʱ���ж����
Input: 
	data: һά����
	win_size_time: ʱ�򴰳�
	stride_time: ʱ�򲽳�
	time_times: ʱ��˷�����
	time_add: ʱ��ӷ�����
Output: None
Return:
	time_vote: ʱ���ж����������ֵ��
%}

time = zeros((length(data)- win_size_time) / stride_time+ 1, 1); % ���㴰����
for i = 1: length(time)
	time(i, 1) = std(data((i- 1)* stride_time+ 1: (i- 1)* stride_time+ win_size_time)); % ���ڱ�׼��
end

temp = std_cv(data(1: 100));

time_vote = max(time) > min(min(time)* time_times, min(time)+ time_add); % ���ݻ������ݵ������С��׼�����ʱ���ж�

end