function [time_vote] = time_detection(data, win_size_time, stride_time, time_times, time_add)

%{
Function Name: time_detection
Description: ���ݴ��ڱ�׼���ʱ���ж����
Input: 
	data: ԭʼ����
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
time_vote = max(time) > min(min(time)* time_times, min(time)+ time_add); % ���ݻ������ݵ������С��׼�����ʱ���ж�

figure(2)
plot(data)
title('�ź�ʱ����')

figure(3)
plot(time, 'LineWidth', 2)
hold on
plot(min(time)* time_times* ones(length(time)), 'r', 'LineWidth', 2)
hold on
plot((min(time)+ time_add)* ones(length(time)), 'b', 'LineWidth', 2)
hold off
title('�ź�ʱ����')

end