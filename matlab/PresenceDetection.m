close all;
clearvars;
clear;
clc;

% 关闭并删除已占用端口
if ~isempty(instrfind)
		fclose(instrfind);
		delete(instrfind);
end
% 端口配置
s = serial('com31'); % 创建串行端口对象
set(s, 'BaudRate', 115200, 'StopBits', 1, 'Parity', 'none', 'DataBits', 8, 'InputBufferSize', 100251, 'ReadAsyncMode', 'continuous'); % 配置 波特率 停止位 校验方式 数据位 输入缓存大小（字节） 异步读取
% set(s, 'BaudRate', 128000, 'StopBits', 1, 'Parity', 'none', 'DataBits', 8, 'InputBufferSize', 10251, 'ReadAsyncMode', 'continuous'); % 配置 波特率 停止位 校验方式 数据位 输入缓存大小（字节） 异步读取
fopen(s); % 打开串口

sample_rate = 2048; % 实际采样率
div = 8; % 分频数
fs = sample_rate / div; % 处理采样率
T = 1 / fs; % 采样间隔

% 全局变量预设值
secnum = 10; % 配置预处理数据长度（时间）
cnt = 1; % 计数初始值
presence_count = 0; % 存在计数
presence_flag = 10; % 存在门限
move = 0; % 大动作检测初始值
lastcolor = 'g'; % 初始颜色

% 数据重构
data = zeros(sample_rate, secnum);

while(1)
    data_corrent = fgetl(s); % fgerl - 读取文件中的行 以字符向量形式返回 并删除换行符
    strlen = length(data_corrent);
    if strlen ~= 10249
        continue;
    end
    i = 1;
    if(data_corrent(i) == 0x42 && data_corrent(i+ 1) == 0x65 && data_corrent(i+ 2) == 0x67 && data_corrent(i+ 3) == 0x69 && data_corrent(i+ 4) == 0x6E)
        cntt = mod(cnt, 5); % 取余
        if cntt == 0
            cntt = 5;
        end
        for index = 1: 2048
            if data_corrent(i+ 5+ 5*index) == 0x20
                qian = double(data_corrent(i+ 5*index+ 1)- 0x30);
                bai = double(data_corrent(i+ 5*index+ 2)- 0x30);
                shi = double(data_corrent(i+ 5*index+ 3)- 0x30);
                ge = double(data_corrent(i+ 5*index+ 4)- 0x30);
                if cnt <= secnum
                    data(index, cnt) = qian*1000 + bai*100 + shi*10 + ge*1;
                else
                    data(index, secnum- 5+ cntt) = qian*1000 + bai*100 + shi*10 + ge*1;
                end
            end
        end

        % 数据处理
        if cnt >= secnum && mod(cnt, 5) == 0
			% 数据抽样
            data_Samples = data(1:2:end, :); % 1024
            data_Samples = data_Samples(1:2:end, :); % 512
			data_Samples = data_Samples(1:2:end, :); % 256
            num = sample_rate / div * secnum; % 数据抽样后的总数         
			raw_data = reshape(data_Samples, num, 1);
            raw_data_MF = raw_data - mean(raw_data); % 均值滤波
			% 时域图
			timeStamps = 0: T: (num- 1)*T; % 时间戳
            timeStamps = timeStamps.';
			figure(1)
            plot(timeStamps, raw_data_MF)
            title('Signal Corrupted with Zero-Mean Random Noise')
            xlabel('t(sec)')
            ylabel('|raw data MF|')
			% 频域分析			
			win_size = length(raw_data_MF);
			w = window(@hamming, win_size); % 窗函数
			X = raw_data_MF .* w; % 加窗
			Y = fft(X, num); % FFT
			AP_double = abs(Y)/ num; % 双边谱
			AP_single = AP_double(1: num/ 2); % 单边谱
			AP_single(2: end- 1) = 2*AP_single(2: end- 1); % 计算单边谱幅度并去除零频放大效应
			f = fs*(0: (num/ 2)- 1)/ num;
			% 绘制单边谱           
			figure(2)
			plot(f, AP_single)
			title('Single-Sided Amplitude Spectrum of raw data MF')
			xlabel('f(Hz)')
			ylabel('|AP_single(f)|')
									
			% 去除工频干扰
			peak_50 = 0;
			peak_100_1 = 0;
			peak_100_2 = 0;
			% 滤除50Hz干扰信号
			[pks, locs] = findpeaks(AP_single(401:600), f(401:600)); % 50Hz附近峰值
            [M, I] = max(pks);
            peak_50 = locs(I(1));			
			% 滤除100Hz干扰信号
            [pks, locs] = findpeaks(AP_single(901:1100), f(901:1100)); % 100Hz附近峰值           
            [M, I] = sort(pks, 'descend');
            if M(1) > 0.6
                peak_100_1 = locs(I(1));
                peak_100_2 = locs(I(2));
            end     
            % 自适应陷波器    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
			raw_data_MF_ANF = raw_data_MF;
            if peak_100_1 > 0
                Size_t = size(timeStamps, 1);
                Signal_noise = raw_data_MF.';
                % FIR滤波器配置
                M_FIR = 6; % 定义FIR滤波器阶数
                lamda = 0.99; % 定义遗忘因子
                Signal_Len = Size_t; % 定义信号数据的个数
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
                    d = Signal_noise(i); % 输入新的期望信号
                    Interference = [sin(2*pi*peak_50*(i- 1)/fs)
                        cos(2*pi*peak_50*(i- 1)/fs)
						sin(2*pi*peak_100_1*(i- 1)/fs)
						cos(2*pi*peak_100_1*(i- 1)/fs)
						sin(2*pi*peak_100_2*(i- 1)/fs)
						cos(2*pi*peak_100_2*(i- 1)/fs)]; % 输入新的信号矢量
                    % 算法正体
                    K = (P_last * Interference) / (lamda + Interference'* P_last * Interference); % 计算增益矢量
                    y = Interference' * w_last; % 计算FIR滤波器输出
                    Eta = d - y; % 计算估计的误差
                    w = w_last + K * Eta; % 计算滤波器系数矢量
                    P = (I - K * Interference') * P_last/lamda; % 计算误差相关矩阵
                    % 变量更替
                    P_last = P;
                    w_last = w;
                    % 滤波结果存储
                    y_out(i) = y;
                    Eta_out(i) = Eta;
                    w_out(i,:) = w';
                end           
                raw_data_MF_ANF = Eta_out; % 对消后信号                
            end
			% 频域分析
            win_size = length(raw_data_MF_ANF);
            w = window(@hamming, win_size); % 窗函数
            X = raw_data_MF_ANF .* w; % 加窗
            Y = fft(X, num);
            AP_double = abs(Y/ num);
            AP_single = AP_double(1: num/2);
            AP_single(2: end- 1) = 2*AP_single(2: end- 1);
            f = fs*(0: (num/2)- 1)/ num;
            % 绘制单边谱
            figure(3)
            plot(f, AP_single)
            title('Single-Sided Amplitude Spectrum of raw data MF ANF')
            xlabel('f(Hz)')
            ylabel('|AP_single(f)|')
			
            % 存在感应
            colorflag = 'g'; % 没人时显示绿色			
            % 检测大动作目标
            slice_100 = AP_single(1: 1000); % 100Hz以外不要了
			slice_100_flip = flip(slice_100, 1); % 上下翻转 
			P = [slice_100_flip; slice_100]; % 合成一个矩阵
			xc = P.'; % 转置
			% CA-CFAR
			N = 100; % 窗口大小
            pro_N = 50; % 保护单元
            PAD = 10^(-8); % 虚警概率
            [index, XT] = cfar_ca(xc, N, pro_N, PAD);	% @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@		
			slice_100_CFAR = P .* XT;
			slice_100_CFAR = slice_100_CFAR(1: length(slice_100_CFAR)/ 2);
			% 滑窗
            winnum = floor(length(slice_100_CFAR)/ 100);
            stepmean = zeros(1, winnum- 1);			
            % 滑动取最大值最小值
            for i = 1: winnum- 1
                stepmean(1, i) = mean(slice_100_CFAR((i- 1)*100+ 1:(i- 1)*100+ 200)); % 每200个频点取均值
            end
            minstep = min(stepmean);
            maxstep = max(stepmean);
            TH = 7*minstep;
            figure(4)
            if(lastcolor == 'g' && maxstep > 7*minstep) % 无人到有人 首次触发
                maxnum = max([max(stepmean) 5*minstep]);
                plot(length(stepmean), maxnum, 'p', 'MarkerSize', 30, ...
                    'MarkerEdgeColor', 'r', ...
                    'MarkerFaceColor', 'r');
                presence_count = presence_count + 1;
                TH = 7*minstep;
                hold on;
            end
            if((lastcolor == 'r' && maxstep > 5*minstep)||...
			(lastcolor=='g' && maxstep > 7*minstep && presence_count > 2)) % 存在较大动作或较大运动物体 初次触发门限提高
                colorflag = 'r';
                move = 1;
                presence_flag = 0;
                presence_count = 0; % 归0
                maxnum = max([max(stepmean) 5*minstep]);
                plot(length(stepmean), maxnum, 'p', 'MarkerSize', 30,...
                    'MarkerEdgeColor', 'r',...
                    'MarkerFaceColor', 'r');
                if(lastcolor == 'r')
                    TH = 5*minstep;
                elseif(lastcolor == 'g')
                    TH = 7*minstep;
                end                
                hold on;
            end
            plot(stepmean, 'b');
			hold on;
            title('大动作检测');
            threshold = ones(length(stepmean))*TH;
            plot(threshold, 'r');
			hold off;
            
            % 微动探测
            % 只有已证明有目标的前提下进行判断
            if(lastcolor == 'r') % 有目标                
                slice_50 = AP_single(1: 500); % 取前50Hz频段
                slice_50_flip = flip(slice_50, 1); % 上下翻转
                P = [slice_50_flip; slice_50]; % 合成一个矩阵
                xc = P.'; % 转置
				% CA-CFAR
                N = 100; % 窗口大小
                pro_N = 50; % 保护单元
                PAD = 10^(-8); % 虚警概率
                [index, XT] = cfar_ca(xc, N, pro_N, PAD);
                figure(5);
                xxcc = 10.* log(abs(xc)/ max(abs(xc))+ 1)./ log(10);
                XXTT = 10.* log(abs(XT)/ max(abs(XT))+ 1)./ log(10);
                offset = 0.4;
                offsetmin = 0.2;
                % 取峰值
                [pks, locs] = findpeaks(xxcc(index(1): index(end)), index);
                C = zeros(1, length(locs));
                for i = 1: length(locs)
                    C(1, i) = find(index == locs(i));
                end
                for i = 1: length(locs)- 8
                    if locs(i) > 500 && locs(i) < 530 % 低频直通车 门限也降低 针对呼吸运动
                        if pks(i) > offsetmin+ XXTT(C(i))
                            colorflag = 'r';
                            presence_flag = 0;
                            plot(max(index), max(offsetmin+ XXTT), 'p', 'MarkerSize', 30, ...
                                'MarkerEdgeColor', 'r', ...
                                'MarkerFaceColor', 'r');
                            hold on;
                            offset = offsetmin; % 绘图时绘制真实门限                           
                            break;
                        end
                    end
                    if pks(i) > offset+ XXTT(C(i))                       
                        if pks(i+ 1) > offset+ XXTT(C(i+ 1)) ||...
                                pks(i+ 2) > offset+ XXTT(C(i+ 2)) ||...
                                pks(i+ 3) > offset+ XXTT(C(i+ 3)) ||...
                                pks(i+ 4) > offset+ XXTT(C(i+ 4)) ||...
                                pks(i+ 5) > offset+ XXTT(C(i+ 5)) ||...
                                pks(i+ 6) > offset+ XXTT(C(i+ 6)) ||...
                                pks(i+ 7) > offset+ XXTT(C(i+ 7)) ||...
                                pks(i+ 8) > offset+ XXTT(C(i+ 8)) % 一根刺不是刺，连续刺穿才算数
                            if locs(i) < 550 && locs(i+ 8) > 550 % 对称的多个刺不行
                                continue;
                            end
                            colorflag = 'r';
                            presence_flag = 0;
                            plot(max(index), max(offset+ XXTT), 'p', 'MarkerSize', 30,...
                                'MarkerEdgeColor', 'r',...
                                'MarkerFaceColor', 'r');
                            hold on;                          
                            break;
                        end                       
                    end
                end
                plot(xxcc, 'b');
				hold on;
                title('微动检测');
				plot(index, offset+ XXTT, 'r')
				hold off;				
            end
			
			if colorflag == 'r' % 有人存在 进行呼吸频率检测
				% iceemdan去噪
				X = raw_data_MF_ANF;
				Nstd = 0.2*std(X);
				[allmode, its] = iceemdan(X, Nstd, 50, 1000, 1);
				[m, n] = size(allmode);
				% 查看分解效果
				k = 4;
				figure(6)			
				for i = 1: k
					subplot(k, 1, i)
					plot(allmode(i, :));
				end
				% 获取低模数据
				partmode = allmode(k: end, :);
				raw_data_MF_ANF_iceemdan = sum(partmode, 1).';
				figure(7)			
				plot(timeStamps, raw_data_MF_ANF_iceemdan)
				title('Signal after iceemdan')
				xlabel('t(sec)')
				ylabel('X(t)')
				% 频域分析			
				X = raw_data_MF_ANF_iceemdan;
				win_size = length(X);
				w = window(@hamming, win_size); % 窗函数
				X = X .* w; % 加窗
				% FFT
				Y = fft(X, num);
				AP_double = abs(Y)/ num;
				AP_single = AP_double(1: num/ 2);
				AP_single(2: end- 1) = 2*AP_single(2: end- 1); % 计算单边谱幅度并去除零频放大效应
				f = fs*(0: (num/ 2)- 1)/ num;
				% 绘制单边谱           
				figure(8)
				plot(f, AP_single)
				title('Single-Sided Amplitude Spectrum of X(t)')
				xlabel('f(Hz)')
				ylabel('|AP_single(f)|')
				xlim([0.1 1])
				
				% MUSIC
				X = raw_data_MF_ANF_iceemdan;
				signalnum = 2; % 信号中有效成分的数量
				signal = X;
				% 计算加噪声信号的自相关矩阵，令M=1000;
				M = 1000;
				R = corrmtx(signal, M- 1);
				Rxx = R'* R;
				[V, D] = eig(Rxx); % 对相关矩阵作特征值分解
	   
				% 计算MUSIC算法中的扫描函数P，以确定有效成分的频率
				omega = 2*pi*(0.05:0.005:3)/fs;
				l = length(omega);
				P = zeros(l, 1); % 扫描函数
				for i = 1: l
					deno = 0;
					a = exp(-1i*omega(i).*(0: M-1));
					for j=1: M- signalnum
						deno = deno+ abs((a*V(:, j)))^(2)/D(j, j);
					end
					P(i) = 1 / deno;
				end
			
				% 绘制扫描函数图
				figure(9)
				ff = omega* 60/ (2*pi)*fs;
				plot(ff, P);
				title('Respiratory rate(MUSIC)')
				xlabel('Respiratory rate(min)')
				ylabel('Amplitude')
				
				[pks, locs] = findpeaks(P(:),ff(:));
				[M, I] = max(pks);
				RR = locs(I); % 初步取呼吸频率
				RR_str = num2str(RR);
				string = ['\leftarrow 呼吸频率' RR_str '次/分钟'];
				text(RR, M, string, 'Color', 'red', 'FontSize', 26);
			end
                   
            if colorflag == 'g' && presence_flag < 10 % 刚满足无人条件，慎重
                presence_flag = presence_flag + 1;
            end
            if presence_flag >= 2 % 确认无人了
                colorflag = 'g';
                presence_flag = 10;
            else
                colorflag = 'r';
            end
            figure(10);
            alpha = 0: pi/20 :2*pi; % 角度[0, 2*pi]
            R = 2; % 半径
            xx = R*cos(alpha);
            yy = R*sin(alpha);
            plot(xx, yy, '-');
            axis equal
            fill(xx, yy, colorflag); % 用红色填充
            data(:, 1:secnum- 5) = data(:, 6: secnum);
            lastcolor = colorflag;
        end
        cnt = cnt+ 1;
    end
end