function [time_vote] = quicktime_detection(data_accum_MF,sample_rate)
%QUICKTIME_DETECTION �˴���ʾ�йش˺�����ժҪ
%   �˴���ʾ��ϸ˵��
% ʱ������
	quick_time = zeros((length(data_accum_MF)- sample_rate) / (sample_rate/ 2)+ 1, 1); % 1�뻬�� ����������sample_rate ����Ϊ0.5��
	for i = 1: length(quick_time)
		quick_time(i, 1) = std(data_accum_MF((i- 1)* (sample_rate/ 2)+ 1: (i- 1)* (sample_rate/ 2)+ sample_rate));
    end
    time_vote = max(quick_time) > min(min(quick_time)* 5, min(quick_time)+ 50); % ���ݻ������ݵ������С��׼�����ʱ���ж�

    figure(1)
	plot(quick_time)
	hold on
	plot(min(min(quick_time)* 5, min(quick_time)+ 50)* ones(1, length(quick_time)))
	hold off
    
end

