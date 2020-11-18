function [time_vote] = quick_detection_time(data, win_size, stride)

%{
Function Name: quick_detection_time
Description: ���ݴ��ڱ�׼���ʱ���ж����
Input: 
	data: һά����
	win_size: ����
	stride: ����
Output: None
Return:
	time_vote: ʱ���ж����������ֵ��
%}

quick_time = zeros((length(data)- win_size) / stride+ 1, 1); % ���㴰����
for i = 1: length(quick_time)
	quick_time(i, 1) = std(data((i- 1)* stride+ 1: (i- 1)* stride+ win_size)); % ���ڱ�׼��
end
time_vote = max(quick_time) > min(min(quick_time)* 5, min(quick_time)+ 50); % ���ݻ������ݵ������С��׼�����ʱ���ж�

end