function [quick_detection_result] = quick_detection(data, data_sample, win_size_time, stride_time, time_times, time_add, win_size_freq, stride_freq, time_accum, xhz, freq_times, respiration_times)

%{
Function Name: quick_detection
Description: 快检测
Input:
	data: 一维数组
	data_sample: 一维数组抽样
	win_size_time: 时域窗长
	stride_time: 时域步长
	time_times: 时域乘法门限
	time_add: 时域加法门限
	win_size_freq: 频域窗长
	stride_freq: 频域步长
	time_accum: 累积时间
	xhz: 去除频点数量
	freq_times: 频域乘法门限
	respiration_times: 呼吸频域乘法门限
Output: None
Return: 
	quick_detection_result: 快检测判定结果（布尔值）
%}

time_vote = time_detection(data, win_size_time, stride_time, time_times, time_add); % 时域判定
if time_vote
	[freq_vote, respirationfreq_vote] = freq_detection(data_sample, win_size_freq, stride_freq, time_accum, xhz, freq_times, respiration_times); % 频域判定
	if freq_vote
		quick_detection_result = 1;
	else
		quick_detection_result = 0;
	end
else
	quick_detection_result = 0;
end

end