function [index, XT] = cfar_ca(xc, N, pro_N, PAD)
alpha = N.* (PAD.^(-1./N)- 1); % 门限系数
index = 1+ N/2+ pro_N/2: length(xc)- N/2- pro_N/2; % 中间那一部分
XT = zeros(1, length(index)); % 门限
for i = index
    cell_left = xc(1, i- N/2- pro_N/2: i- pro_N/2- 1);
    cell_right = xc(1, i+ pro_N/2+ 1: i+ N/2+ pro_N/2);
    Z = (sum(cell_left)+ sum(cell_right))./ N;
    XT(1, i- N/2- pro_N/2) = Z.* alpha;
end
end