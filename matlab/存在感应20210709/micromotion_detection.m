function [micromotion_vote] = micromotion_detection(data, secnum, xhz, pf, N, pro_N, PAD, offset, rr_threshold)

%{
Function Name: micromotion_detection
Description: 微动检测
Input:
	data: 原始数据
	secnum: 累积时间
	xhz: 去除频点数量
	pf: 工频频点
	N: CFAR窗口大小
	pro_N: CFAR保护单元大小
	PAD: CFAR虚警概率
	offset: 门限偏置
	rr_threshold: 呼吸频率门限
Output: None
Return:
	micromotion_vote: 微动检测判定结果（布尔值）
%}

micromotion_vote = 0; % 默认为无人

w = window(@hamming, length(data)); % 窗函数
X = data .* w; % 加窗
Y = fft(X, length(data)); % FFT
AP_double = abs(Y)/ length(data); % 双边谱
AP_single = AP_double(1: length(data)/ 2); % 单边谱
AP_single(2: end) = 2* AP_single(2: end); % 计算单边谱幅度并去除零频放大效应
AP_single_ANF = remove_pf(AP_single, secnum, xhz, pf); % 去除工频及其谐波周围xHz频点
slice = AP_single_ANF(1: 50* secnum); % 取前50Hz频段
slice_flip = flip(slice, 1); % 上下翻转
P = [slice_flip; slice]; % 合成一个矩阵
xc = P.'; % 转置
[index, XT] = cfar_ca(xc, N, pro_N, PAD); % CA-CFAR
xxcc = 10.* log(abs(xc)/ max(abs(xc))+ 1)./ log(10); % 数据对数归一化
XXTT = 10.* log(abs(XT)/ max(abs(XT))+ 1)./ log(10); % 门限对数归一化

% 调参绘图
figure(6)
plot(xxcc, 'b')
hold on
plot(index, offset+ XXTT, 'r')
hold on

% 微动判定
for i = 50* secnum+ 2 : (50+ rr_threshold)* secnum- 1
	if xxcc(i) > offset + XXTT(i- (N+ pro_N)/ 2)
		micromotion_vote = 1;
		plot(length(index), max(offset+ XXTT), 'p', 'MarkerSize', 30, 'MarkerEdgeColor', 'r', 'MarkerFaceColor', 'r')
		break
	end
end

hold off
title('微动检测')

end