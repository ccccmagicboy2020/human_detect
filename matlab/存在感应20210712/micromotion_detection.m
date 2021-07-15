function [micromotion_vote] = micromotion_detection(data, secnum, xhz, pf, N, pro_N, PAD, offset, rr_threshold)

%{
Function Name: micromotion_detection
Description: ΢�����
Input:
	data: ԭʼ����
	secnum: �ۻ�ʱ��
	xhz: ȥ��Ƶ������
	pf: ��ƵƵ��
	N: CFAR���ڴ�С
	pro_N: CFAR������Ԫ��С
	PAD: CFAR�龯����
	offset: ����ƫ��
	rr_threshold: ����Ƶ������
Output: None
Return:
	micromotion_vote: ΢������ж����������ֵ��
%}

micromotion_vote = 0; % Ĭ��Ϊ����

w = window(@hamming, length(data)); % ������
X = data .* w; % �Ӵ�
Y = fft(X, length(data)); % FFT
AP_double = abs(Y)/ length(data); % ˫����
AP_single = AP_double(1: length(data)/ 2); % ������
AP_single(2: end) = 2* AP_single(2: end); % ���㵥���׷��Ȳ�ȥ����Ƶ�Ŵ�ЧӦ
AP_single_ANF = remove_pf(AP_single, secnum, xhz, pf); % ȥ����Ƶ����г����ΧxHzƵ��
slice = AP_single_ANF(1: 50* secnum); % ȡǰ50HzƵ��
slice_flip = flip(slice, 1); % ���·�ת
P = [slice_flip; slice]; % �ϳ�һ������
xc = P.'; % ת��
[index, XT] = cfar_ca(xc, N, pro_N, PAD); % CA-CFAR
xxcc = 10.* log(abs(xc)/ max(abs(xc))+ 1)./ log(10); % ���ݶ�����һ��
XXTT = 10.* log(abs(XT)/ max(abs(XT))+ 1)./ log(10); % ���޶�����һ��

% ���λ�ͼ
figure(6)
plot(xxcc, 'b')
hold on
plot(index, offset+ XXTT, 'r')
hold on

% ΢���ж�
for i = 50* secnum+ 2 : (50+ rr_threshold)* secnum- 1
	if xxcc(i) > offset + XXTT(i- (N+ pro_N)/ 2)
		micromotion_vote = 1;
		plot(length(index), max(offset+ XXTT), 'p', 'MarkerSize', 30, 'MarkerEdgeColor', 'r', 'MarkerFaceColor', 'r')
		break
	end
end

hold off
title('΢�����')

end