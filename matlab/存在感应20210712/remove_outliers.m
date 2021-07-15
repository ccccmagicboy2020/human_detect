function [data_remove_outliers] = remove_outliers(data, N_sigma, nbins, N_interval)

%{
Function Name: remove_outliers
Description: ȥ����Ⱥ��
Input: 
	data: ԭʼ����
	N_sigma: ���ޱ���
	nbins: bin����
	N_interval: �������
Output: None
Return: 
	data_remove_outliers: ȥ����Ⱥ��������
%}

data = data - mean(data); % ��ֵ�˲�
data_remove_outliers = data; % ��ֵ
flag = 1; % Ĭ��ȥ����Ⱥ��
std_upper = std(data)* N_sigma; % ��Ⱥ���ж�������
std_lower = -std_upper; % ��Ⱥ���ж�������
% ������Ⱥ��
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
% ��Ⱥ��ͳ�Ʒֲ�
[N_hist, edges] = histcounts(k, nbins);
% ��Ⱥ��ͳ������
maxN = max(N_hist);
kk = find(N_hist > max(maxN* 0.12, 2));
kk1 = diff(kk); % 1�ײ��
for i = 1: length(kk1)- 1
    if (kk1(i) >= 1 && kk1(i+ 1) >= 1 && ((kk1(i)+ kk1(i+ 1)) <= 4) && (kk1(i)+ kk1(i+ 1)) >= 2) % ����
        flag = 0;
        break
    end
end
if flag % ���ڸ������޴���
	% ��Ը��ŷ�ֵ���в�ֵƽ������
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