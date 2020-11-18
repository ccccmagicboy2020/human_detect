function [time_vote] = time_detection(data, win_size_time, stride_time, time_times, time_add)

%{
Function Name: time_detection
Description: 根据窗内标准差返回时域判定结果
Input: 
	data: 一维数组
	win_size_time: 时域窗长
	stride_time: 时域步长
	time_times: 时域乘法门限
	time_add: 时域加法门限
Output: None
Return:
	time_vote: 时域判定结果（布尔值）
%}

time = zeros((length(data)- win_size_time) / stride_time+ 1, 1); % 计算窗数量
for i = 1: length(time)
	time(i, 1) = std(data((i- 1)* stride_time+ 1: (i- 1)* stride_time+ win_size_time)); % 窗内标准差
end

temp = std_cv(data(1: 100));

time_vote = max(time) > min(min(time)* time_times, min(time)+ time_add); % 根据滑窗数据的最大最小标准差进行时域判定

end