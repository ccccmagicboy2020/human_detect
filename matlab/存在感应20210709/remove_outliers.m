function [data_remove_outliers] = remove_outliers(data, N_sigma, nbins, N_interval)

%{
Function Name: remove_outliers
Description: 去除离群点
Input: 
	data: 原始数据
	N_sigma: 门限倍数
	nbins: bin数量
	N_interval: 间隔长度
Output: None
Return: 
	data_remove_outliers: 去除离群点后的数据
%}

data = data - mean(data); % 均值滤波
data_remove_outliers = data; % 赋值
flag = 1; % 默认去除离群点
std_upper = std(data)* N_sigma; % 离群点判定上门限
std_lower = -std_upper; % 离群点判定下门限
% 查找离群点
k = find(data>std_upper | data<std_lower);
if isempty(k)
    flag = 0;
    return
end
if k(1) ~= 1
    k = [1; k];
end
if k(end) ~= length(data)
    k = [k; length(data)];
end
% 离群点统计分布
[N_hist, edges] = histcounts(k, nbins);
% 离群点统计门限
maxN = max(N_hist);
kk = find(N_hist > max(maxN* 0.12, 2));
kk1 = diff(kk); % 1阶差分
for i = 1: length(kk1)- 1
    if (kk1(i) >= 1 && kk1(i+ 1) >= 1 && ((kk1(i)+ kk1(i+ 1)) <= 4) && (kk1(i)+ kk1(i+ 1)) >= 2) % 大动作
        flag = 0;
        break
    end
end
if flag % 存在干扰且无大动作
	% 针对干扰峰值进行插值平滑处理
	for i = 1: length(k)
		if(k(i) >= N_interval+ 1 && k(i) <= length(data)- N_interval)
        data_remove_outliers(k(i)) = (data(k(i)- N_interval)+ data(k(i)+ N_interval))/ 2;
		elseif(k(i) < N_interval+ 1)
			data_remove_outliers(k(i)) = (data(1)+ data(k(i)+ N_interval))/ 2;
		elseif(k(i) > length(data)- N_interval)
			data_remove_outliers(k(i)) = (data(k(i)- N_interval)+ data(end))/ 2;
		end
	end
end

end