function [result] = add_hamming(data)
%add_hamming 给数据加汉宁窗
%   此处显示详细说明
    w = window(@hamming, length(data)); % 窗函数
    result = data .* w; % 加窗
end


