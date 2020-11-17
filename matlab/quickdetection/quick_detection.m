function vote = quick_detection(data_accum_MF,sample_rate)%补充过滤频点数组
%QUICKTIME_DETECTION 此处显示有关此函数的摘要
%   此处显示详细说明
% 时域数据
time_vote=quicktime_detection(data_accum_MF,sample_rate);
% 频域处理
freq_vote = quickfreq_detection(data_accum_MF,sample_rate);
if time_vote && freq_vote
    vote=1;
else
    vote=0;
end
end

