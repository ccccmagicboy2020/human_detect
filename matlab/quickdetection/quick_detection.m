function vote = quick_detection(data_accum_MF,sample_rate)%�������Ƶ������
%QUICKTIME_DETECTION �˴���ʾ�йش˺�����ժҪ
%   �˴���ʾ��ϸ˵��
% ʱ������
time_vote=quicktime_detection(data_accum_MF,sample_rate);
% Ƶ����
freq_vote = quickfreq_detection(data_accum_MF,sample_rate);
if time_vote && freq_vote
    vote=1;
else
    vote=0;
end
end

