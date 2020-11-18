function [time_vote] = quick_detection_time(data, win_size, stride)

%{
Function Name: quick_detection_time
Description: 根据窗内标准差返回时域判定结果
Input: 
	data: 一维数组
	win_size: 窗长
	stride: 步长
Output: None
Return:
	time_vote: 时域判定结果（布尔值）
%}

quick_time = zeros((length(data)- win_size) / stride+ 1, 1); % 计算窗数量
for i = 1: length(quick_time)
	quick_time(i, 1) = std(data((i- 1)* stride+ 1: (i- 1)* stride+ win_size)); % 窗内标准差
end
time_vote = max(quick_time) > min(min(quick_time)* 5, min(quick_time)+ 50); % 根据滑窗数据的最大最小标准差进行时域判定

end