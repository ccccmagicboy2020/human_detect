function [vote_result] = quick_detection(data, win_size_time, stride_time, time_times, time_add, win_size_freq, stride_freq, time_accum, xhz, freq_times)

%{
Function Name: vote
Description: 根据时频域投票给出最终判定结果
Input:
	data: 一维数组
	win_size_time: 时域窗长
	stride_time: 时域步长
	time_times: 时域乘法门限
	time_add: 时域加法门限
	win_size_freq: 频域窗长
	stride_freq: 频域步长
	time_accum: 累积时间
	xhz: 去除频点数量
	freq_times: 频域乘法门限
Output: None
Return: 
	vote_result: 最终判定结果（布尔值）
%}

time_vote = time_detection(data, win_size_time, stride_time, time_times, time_add); % 时域判定
freq_vote = freq_detection(data, win_size_freq, stride_freq, time_accum, xhz, freq_times); % 频域判定
vote_result = time_vote && freq_vote;

end