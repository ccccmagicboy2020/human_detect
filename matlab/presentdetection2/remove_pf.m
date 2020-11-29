function [data_remove_pf] = remove_pf(data, time_accum, xhz, freq50or60)

%{
Function Name: remove_pf
Description: ����ȥ����Ƶ����г����ΧxHzƵ��
Input: 
	data: һά����
	time_accum: �ۻ�ʱ��
	xhz: ȥ��Ƶ������
    freq50or60:��Ƶ����Ƶ��ֵ
Output: None
Return: 
	data_remove_pf: ȥ����Ƶ���ź������
%}

part_1 = data(1: xhz* time_accum);
part_3 = data(freq50or60* time_accum* fix(length(data)/ (freq50or60* time_accum))+ xhz* time_accum+ 1: length(data));
part_2 = zeros(freq50or60* time_accum- 2* xhz* time_accum, fix(length(data)/ (freq50or60* time_accum)));
for i  = 1: fix(length(data)/ (freq50or60* time_accum))
	part_2(:, i) = data(freq50or60* time_accum* (i- 1)+ xhz* time_accum+ 1: freq50or60* time_accum* i- xhz* time_accum);
end
part_2 = reshape(part_2, numel(part_2), 1);
data_remove_pf = [part_1
				  part_2
				  part_3];

end