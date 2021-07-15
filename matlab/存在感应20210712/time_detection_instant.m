function [time_vote_instant] = time_detection_instant(data_instant, win_size_time_instant, stride_time_instant, time_times_instant, time_add_instant)

%{
Function Name: time_detection_instant
Description: 根据窗内标准差返回时域判定结果
Input: 
	data_instant: 原始数据
	win_size_time_instant: 时域窗长
	stride_time_instant: 时域步长
	time_times_instant: 时域乘法门限
	time_add_instant: 时域加法门限
Output: None
Return:
	time_vote_instant: 时域判定结果（布尔值）
%}

time_instant = zeros((length(data_instant)- win_size_time_instant) / stride_time_instant+ 1, 1); % 计算窗数量
for i = 1: length(time_instant)
	time_instant(i, 1) = std(data_instant((i- 1)* stride_time_instant+ 1: (i- 1)* stride_time_instant+ win_size_time_instant)); % 窗内标准差
end
time_vote_instant = max(time_instant) > min(min(time_instant)* time_times_instant, min(time_instant)+ time_add_instant); % 根据滑窗数据的最大最小标准差进行时域判定

end