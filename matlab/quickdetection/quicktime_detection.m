function [time_vote] = quicktime_detection(data_accum_MF,sample_rate)
%QUICKTIME_DETECTION 此处显示有关此函数的摘要
%   此处显示详细说明
% 时域数据
	quick_time = zeros((length(data_accum_MF)- sample_rate) / (sample_rate/ 2)+ 1, 1); % 1秒滑窗 数据量等于sample_rate 步长为0.5秒
	for i = 1: length(quick_time)
		quick_time(i, 1) = std(data_accum_MF((i- 1)* (sample_rate/ 2)+ 1: (i- 1)* (sample_rate/ 2)+ sample_rate));
    end
    time_vote = max(quick_time) > min(min(quick_time)* 5, min(quick_time)+ 50); % 根据滑窗数据的最大最小标准差进行时域判定

    figure(1)
	plot(quick_time)
	hold on
	plot(min(min(quick_time)* 5, min(quick_time)+ 50)* ones(1, length(quick_time)))
	hold off
    
end

