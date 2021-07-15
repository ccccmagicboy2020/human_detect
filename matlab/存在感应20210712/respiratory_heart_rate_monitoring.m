function [respiratory_rate, heart_rate] = respiratory_heart_rate_monitoring(data, m, respirationfreq_vote, micromotion_vote)

%{
Function Name: respiratoryrate_monitoring
Description: 呼吸、心跳速率检测
Input: 
	data: 原始数据
	m: 变换长度
	respirationfreq_vote: 0.5Hz内频谱过门限判定结果（布尔值）
	micromotion_vote: 微动检测判定结果（布尔值）
Output: None
Return: 
	respiratory_rate: 呼吸速率
	heart_rate: 心跳速率
%}

fs = 250; % 采样率
fn = (0: m- 1)'/ m;

% 呼吸速率检测
f1 = 0.1; % 细化呼吸频率段起始点
f2 = 0.5; % 细化呼吸频率段终止点
w_br = exp(-1i* 2* pi* (f2- f1)/ (m* fs)); % 螺旋轮廓点之间的比率
a_br = exp(1i* 2* pi* f1/ fs); % 螺旋轮廓起始点
z_br = czt(data, m, w_br, a_br); % Chirp Z-transform
fz_br = (f2- f1)* fn+ f1;
[pks_br, locs_br] = findpeaks(abs(z_br)); % 遍历所有峰
[maxVal_br, maxIndex_br] = max(pks_br); % 获取最大峰
respiratory_rate = ((f2- f1)* (locs_br(maxIndex_br(1))- 1)/ m + f1); % 呼吸次数/秒

figure(7)
plot(fz_br, abs(z_br))
title('Respiratory rate')
xlabel('Respiratory rate (min)')
ylabel('Amplitude')
rr = num2str(respiratory_rate* 60);
string_rr = ['\leftarrow' rr '次(分钟)'];
if respirationfreq_vote || micromotion_vote
	text(respiratory_rate, maxVal_br, string_rr, 'Color', 'red', 'FontSize', 15);
else
	text(respiratory_rate, maxVal_br, '不符合检测条件', 'Color', 'red', 'FontSize', 15);
end

figure(8)
data_br_lowpass = filter(lowpass_filter(250, 18, 20), data); % 低通滤波
t_br = 0: 1/ fs: (length(data_br_lowpass)- 1)* (1/ fs);
plot(t_br, data_br_lowpass)
title('Respiratory waveform')
xlabel('Time (s)')
ylabel('Amplitude')
if not(respirationfreq_vote) && not(micromotion_vote)
	text(t_br(length(t_br)/ 2), data_br_lowpass(length(data_br_lowpass)/ 2), '不符合绘制条件', 'Color', 'red', 'FontSize', 15);
end

% 去除呼吸谐波干扰
respiratory_harmonic_3 = respiratory_rate* 3;
respiratory_harmonic_4 = respiratory_rate* 4;
respiratory_harmonic_5 = respiratory_rate* 5;

% FIR滤波器配置
Signal_noise = data.';
M_FIR = 6; % 定义FIR滤波器阶数
lamda = 0.99; % 定义遗忘因子
Signal_Len = length(data); % 定义信号数据的个数
I = eye(M_FIR); % 生成对应的单位矩阵
c = 0.01; % 小正数 保证矩阵P非奇异
y_out = zeros(Signal_Len, 1);
Eta_out = zeros(Signal_Len, 1);
w_out = zeros(Signal_Len, M_FIR);
for i = 1: Signal_Len
	% 输入数据
	if i == 1 % 如果是第一次进入
		P_last = I/c;
		w_last = zeros(M_FIR, 1);
	end
	d = Signal_noise(i);
	Interference = [sin(2*pi*respiratory_harmonic_3*(i- 1)/fs)
	cos(2*pi*respiratory_harmonic_3*(i- 1)/fs)
	sin(2*pi*respiratory_harmonic_4*(i- 1)/fs)
	cos(2*pi*respiratory_harmonic_4*(i- 1)/fs)
	sin(2*pi*respiratory_harmonic_5*(i- 1)/fs)
	cos(2*pi*respiratory_harmonic_5*(i- 1)/fs)]; % 输入新的信号矢量
	% 算法正体
	K = (P_last * Interference) / (lamda + Interference' * P_last * Interference); % 计算增益矢量
	y = Interference' * w_last; % 计算FIR滤波器输出
	Eta = d - y; % 计算估计的误差
	w = w_last + K * Eta; % 计算滤波器系数矢量
	P = (I - K * Interference') * P_last / lamda; % 计算误差相关矩阵
	% 变量更替
	P_last = P;
	w_last = w;
	% 滤波结果存储
	y_out(i) = y;
	Eta_out(i) = Eta;
	w_out(i,:) = w';
end           
data_hr = Eta_out; % 对消后信号

% 心跳速率检测
f3 = 0.8; % 细化心跳频率段起始点
f4 = 2; % 细化心跳频率段终止点
w_hr = exp(-1i* 2* pi* (f4- f3)/ (m* fs)); % 螺旋轮廓点之间的比率
a_hr = exp(1i* 2* pi* f3/ fs); % 螺旋轮廓起始点
z_hr = czt(data_hr, m, w_hr, a_hr); % Chirp Z-transform
fz_hr = (f4- f3)* fn+ f3;
[pks_hr, locs_hr] = findpeaks(abs(z_hr)); % 遍历所有峰
[maxVal_hr, maxIndex_hr] = max(pks_hr); % 获取最大峰
heart_rate = ((f4- f3)* (locs_hr(maxIndex_hr(1))- 1)/ m + f3); % 心跳次数/秒

figure(9)
plot(fz_hr, abs(z_hr))
title('Heart rate')
xlabel('Heart rate (min)')
ylabel('Amplitude')
hr = num2str(heart_rate* 60);
string_hr = ['\leftarrow' hr '次(分钟)'];
if respirationfreq_vote || micromotion_vote
	text(heart_rate, maxVal_hr, string_hr, 'Color', 'red', 'FontSize', 15);
else
	text(heart_rate, maxVal_hr, '不符合检测条件', 'Color', 'red', 'FontSize', 15);
end

figure(10)
data_hr_bandpass = filter(bandpass_filter(250, 0.5, 0.8, 2, 2.5), data_hr);
t_hr = 0: 1/ fs: (length(data_hr_bandpass)- 1)* (1/ fs);
plot(t_hr, data_hr_bandpass)
title('Heartbeat waveform')
xlabel('Time (s)')
ylabel('Amplitude')
if not(respirationfreq_vote) && not(micromotion_vote)
	text(t_hr(length(t_hr)/ 2), data_hr_bandpass(length(data_hr_bandpass)/ 2), '不符合绘制条件', 'Color', 'red', 'FontSize', 15);
end

end