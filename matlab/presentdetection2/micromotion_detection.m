function [micromotion_detection_result] = micromotion_detection(data, secnum, xhz, N, pro_N, PAD, offset, rr_threshold)

%{
Function Name: micromotion_detection
Description: ΢�����
Input:
	data: һά����
	secnum: �ۻ�ʱ��
	xhz: ȥ��Ƶ������
	N: CFAR���ڴ�С
	pro_N: CFAR������Ԫ��С
	PAD: CFAR�龯����
	offset: ����ƫ��
	rr_threshold: ����Ƶ������
Output: None
Return:
	micromotion_detection_result: ΢������ж����������ֵ��
%}

micromotion_detection_result = 0; % Ĭ��Ϊ����

w = window(@hamming, length(data)); % ������
X = data .* w; % �Ӵ�
Y = fft(X, length(data)); % FFT
AP_double = abs(Y)/ length(data); % ˫����
AP_single = AP_double(1: length(data)/ 2); % ������
AP_single(2: end) = 2* AP_single(2: end); % ���㵥���׷��Ȳ�ȥ����Ƶ�Ŵ�ЧӦ
AP_single_ANF = remove_pf(AP_single, secnum, xhz,50); % ȥ����Ƶ����г����ΧxHzƵ��
slice = AP_single_ANF(1: 50* secnum); % ȡǰ50HzƵ��
slice_flip = flip(slice, 1); % ���·�ת
P = [slice_flip; slice]; % �ϳ�һ������
xc = P.'; % ת��
[index, XT] = cfar_ca(xc, N, pro_N, PAD); % CA-CFAR
xxcc = 10.* log(abs(xc)/ max(abs(xc))+ 1)./ log(10); % ���ݶ�����һ��
XXTT = 10.* log(abs(XT)/ max(abs(XT))+ 1)./ log(10); % ���޶�����һ��

figure(11)
plot(xxcc, 'b')
hold on
title('΢�����')
plot(index, offset+ XXTT, 'r')
hold on


% ΢���ж�
for i = 50* secnum : (50+ rr_threshold)* secnum
	if xxcc(i) > offset + XXTT(i- (N+ pro_N)/ 2)
		micromotion_detection_result = 1;   
        figure(11)
        plot(length(index),max(offset+ XXTT),'p','MarkerSize',30,...
                    'MarkerEdgeColor','r',...
                    'MarkerFaceColor','r');
        break
	end
end
figure(11);hold off

figure(12);
plot(data);
% xlabel('t (milliseconds)')
% ylabel('X(t)')
title('�ź�ʱ����');

% % ���λ�ͼ
% figure(1)
% plot(xxcc, 'b')
% hold on
% title('΢�����')
% plot(index, offset+ XXTT, 'r')
% hold off

end