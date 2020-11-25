function [data_remove_pf] = remove_pf(data, time_accum, xhz)

%{
Function Name: remove_pf
Description: 暴力去除工频及其谐波周围xHz频点
Input: 
	data: 一维数组
	time_accum: 累积时间
	xhz: 去除频点数量
Output: None
Return: 
	data_remove_pf: 去除工频干扰后的数据
%}

part_1 = data(1: xhz* time_accum);
part_3 = data(50* time_accum* fix(length(data)/ (50* time_accum))+ xhz* time_accum+ 1: length(data));
part_2 = zeros(50* time_accum- 2* xhz* time_accum, fix(length(data)/ (50* time_accum)));
for i  = 1: fix(length(data)/ (50* time_accum))
	part_2(:, i) = data(50* time_accum* (i- 1)+ xhz* time_accum+ 1: 50* time_accum* i- xhz* time_accum);
end
part_2 = reshape(part_2, numel(part_2), 1);
data_remove_pf = [part_1
				  part_2
				  part_3];

end