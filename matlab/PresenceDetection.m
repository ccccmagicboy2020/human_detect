close all;
clearvars;
clear;
clc;

% �رղ�ɾ����ռ�ö˿�
if ~isempty(instrfind)
		fclose(instrfind);
		delete(instrfind);
end
% �˿�����
s = serial('com31'); % �������ж˿ڶ���
set(s, 'BaudRate', 115200, 'StopBits', 1, 'Parity', 'none', 'DataBits', 8, 'InputBufferSize', 100251, 'ReadAsyncMode', 'continuous'); % ���� ������ ֹͣλ У�鷽ʽ ����λ ���뻺���С���ֽڣ� �첽��ȡ
% set(s, 'BaudRate', 128000, 'StopBits', 1, 'Parity', 'none', 'DataBits', 8, 'InputBufferSize', 10251, 'ReadAsyncMode', 'continuous'); % ���� ������ ֹͣλ У�鷽ʽ ����λ ���뻺���С���ֽڣ� �첽��ȡ
fopen(s); % �򿪴���

sample_rate = 2048; % ʵ�ʲ�����
div = 8; % ��Ƶ��
fs = sample_rate / div; % ���������
T = 1 / fs; % �������

% ȫ�ֱ���Ԥ��ֵ
secnum = 10; % ����Ԥ�������ݳ��ȣ�ʱ�䣩
cnt = 1; % ������ʼֵ
presence_count = 0; % ���ڼ���
presence_flag = 10; % ��������
move = 0; % ��������ʼֵ
lastcolor = 'g'; % ��ʼ��ɫ

% �����ع�
data = zeros(sample_rate, secnum);

while(1)
    data_corrent = fgetl(s); % fgerl - ��ȡ�ļ��е��� ���ַ�������ʽ���� ��ɾ�����з�
    strlen = length(data_corrent);
    if strlen ~= 10249
        continue;
    end
    i = 1;
    if(data_corrent(i) == 0x42 && data_corrent(i+ 1) == 0x65 && data_corrent(i+ 2) == 0x67 && data_corrent(i+ 3) == 0x69 && data_corrent(i+ 4) == 0x6E)
        cntt = mod(cnt, 5); % ȡ��
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

        % ���ݴ���
        if cnt >= secnum && mod(cnt, 5) == 0
			% ���ݳ���
            data_Samples = data(1:2:end, :); % 1024
            data_Samples = data_Samples(1:2:end, :); % 512
			data_Samples = data_Samples(1:2:end, :); % 256
            num = sample_rate / div * secnum; % ���ݳ����������         
			raw_data = reshape(data_Samples, num, 1);
            raw_data_MF = raw_data - mean(raw_data); % ��ֵ�˲�
			% ʱ��ͼ
			timeStamps = 0: T: (num- 1)*T; % ʱ���
            timeStamps = timeStamps.';
			figure(1)
            plot(timeStamps, raw_data_MF)
            title('Signal Corrupted with Zero-Mean Random Noise')
            xlabel('t(sec)')
            ylabel('|raw data MF|')
			% Ƶ�����			
			win_size = length(raw_data_MF);
			w = window(@hamming, win_size); % ������
			X = raw_data_MF .* w; % �Ӵ�
			Y = fft(X, num); % FFT
			AP_double = abs(Y)/ num; % ˫����
			AP_single = AP_double(1: num/ 2); % ������
			AP_single(2: end- 1) = 2*AP_single(2: end- 1); % ���㵥���׷��Ȳ�ȥ����Ƶ�Ŵ�ЧӦ
			f = fs*(0: (num/ 2)- 1)/ num;
			% ���Ƶ�����           
			figure(2)
			plot(f, AP_single)
			title('Single-Sided Amplitude Spectrum of raw data MF')
			xlabel('f(Hz)')
			ylabel('|AP_single(f)|')
									
			% ȥ����Ƶ����
			peak_50 = 0;
			peak_100_1 = 0;
			peak_100_2 = 0;
			% �˳�50Hz�����ź�
			[pks, locs] = findpeaks(AP_single(401:600), f(401:600)); % 50Hz������ֵ
            [M, I] = max(pks);
            peak_50 = locs(I(1));			
			% �˳�100Hz�����ź�
            [pks, locs] = findpeaks(AP_single(901:1100), f(901:1100)); % 100Hz������ֵ           
            [M, I] = sort(pks, 'descend');
            if M(1) > 0.6
                peak_100_1 = locs(I(1));
                peak_100_2 = locs(I(2));
            end     
            % ����Ӧ�ݲ���    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
			raw_data_MF_ANF = raw_data_MF;
            if peak_100_1 > 0
                Size_t = size(timeStamps, 1);
                Signal_noise = raw_data_MF.';
                % FIR�˲�������
                M_FIR = 6; % ����FIR�˲�������
                lamda = 0.99; % ������������
                Signal_Len = Size_t; % �����ź����ݵĸ���
                I = eye(M_FIR); % ���ɶ�Ӧ�ĵ�λ����
                c = 0.01; % С���� ��֤����P������
                y_out = zeros(Signal_Len, 1);
                Eta_out = zeros(Signal_Len, 1);
                w_out = zeros(Signal_Len, M_FIR);
                for i = 1: Signal_Len
                    % ��������
                    if i == 1 % ����ǵ�һ�ν���
                        P_last = I/c;
                        w_last = zeros(M_FIR, 1);
                    end
                    d = Signal_noise(i); % �����µ������ź�
                    Interference = [sin(2*pi*peak_50*(i- 1)/fs)
                        cos(2*pi*peak_50*(i- 1)/fs)
						sin(2*pi*peak_100_1*(i- 1)/fs)
						cos(2*pi*peak_100_1*(i- 1)/fs)
						sin(2*pi*peak_100_2*(i- 1)/fs)
						cos(2*pi*peak_100_2*(i- 1)/fs)]; % �����µ��ź�ʸ��
                    % �㷨����
                    K = (P_last * Interference) / (lamda + Interference'* P_last * Interference); % ��������ʸ��
                    y = Interference' * w_last; % ����FIR�˲������
                    Eta = d - y; % ������Ƶ����
                    w = w_last + K * Eta; % �����˲���ϵ��ʸ��
                    P = (I - K * Interference') * P_last/lamda; % ���������ؾ���
                    % ��������
                    P_last = P;
                    w_last = w;
                    % �˲�����洢
                    y_out(i) = y;
                    Eta_out(i) = Eta;
                    w_out(i,:) = w';
                end           
                raw_data_MF_ANF = Eta_out; % �������ź�                
            end
			% Ƶ�����
            win_size = length(raw_data_MF_ANF);
            w = window(@hamming, win_size); % ������
            X = raw_data_MF_ANF .* w; % �Ӵ�
            Y = fft(X, num);
            AP_double = abs(Y/ num);
            AP_single = AP_double(1: num/2);
            AP_single(2: end- 1) = 2*AP_single(2: end- 1);
            f = fs*(0: (num/2)- 1)/ num;
            % ���Ƶ�����
            figure(3)
            plot(f, AP_single)
            title('Single-Sided Amplitude Spectrum of raw data MF ANF')
            xlabel('f(Hz)')
            ylabel('|AP_single(f)|')
			
            % ���ڸ�Ӧ
            colorflag = 'g'; % û��ʱ��ʾ��ɫ			
            % ������Ŀ��
            slice_100 = AP_single(1: 1000); % 100Hz���ⲻҪ��
			slice_100_flip = flip(slice_100, 1); % ���·�ת 
			P = [slice_100_flip; slice_100]; % �ϳ�һ������
			xc = P.'; % ת��
			% CA-CFAR
			N = 100; % ���ڴ�С
            pro_N = 50; % ������Ԫ
            PAD = 10^(-8); % �龯����
            [index, XT] = cfar_ca(xc, N, pro_N, PAD);	% @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@		
			slice_100_CFAR = P .* XT;
			slice_100_CFAR = slice_100_CFAR(1: length(slice_100_CFAR)/ 2);
			% ����
            winnum = floor(length(slice_100_CFAR)/ 100);
            stepmean = zeros(1, winnum- 1);			
            % ����ȡ���ֵ��Сֵ
            for i = 1: winnum- 1
                stepmean(1, i) = mean(slice_100_CFAR((i- 1)*100+ 1:(i- 1)*100+ 200)); % ÿ200��Ƶ��ȡ��ֵ
            end
            minstep = min(stepmean);
            maxstep = max(stepmean);
            TH = 7*minstep;
            figure(4)
            if(lastcolor == 'g' && maxstep > 7*minstep) % ���˵����� �״δ���
                maxnum = max([max(stepmean) 5*minstep]);
                plot(length(stepmean), maxnum, 'p', 'MarkerSize', 30, ...
                    'MarkerEdgeColor', 'r', ...
                    'MarkerFaceColor', 'r');
                presence_count = presence_count + 1;
                TH = 7*minstep;
                hold on;
            end
            if((lastcolor == 'r' && maxstep > 5*minstep)||...
			(lastcolor=='g' && maxstep > 7*minstep && presence_count > 2)) % ���ڽϴ�����ϴ��˶����� ���δ����������
                colorflag = 'r';
                move = 1;
                presence_flag = 0;
                presence_count = 0; % ��0
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
            title('�������');
            threshold = ones(length(stepmean))*TH;
            plot(threshold, 'r');
			hold off;
            
            % ΢��̽��
            % ֻ����֤����Ŀ���ǰ���½����ж�
            if(lastcolor == 'r') % ��Ŀ��                
                slice_50 = AP_single(1: 500); % ȡǰ50HzƵ��
                slice_50_flip = flip(slice_50, 1); % ���·�ת
                P = [slice_50_flip; slice_50]; % �ϳ�һ������
                xc = P.'; % ת��
				% CA-CFAR
                N = 100; % ���ڴ�С
                pro_N = 50; % ������Ԫ
                PAD = 10^(-8); % �龯����
                [index, XT] = cfar_ca(xc, N, pro_N, PAD);
                figure(5);
                xxcc = 10.* log(abs(xc)/ max(abs(xc))+ 1)./ log(10);
                XXTT = 10.* log(abs(XT)/ max(abs(XT))+ 1)./ log(10);
                offset = 0.4;
                offsetmin = 0.2;
                % ȡ��ֵ
                [pks, locs] = findpeaks(xxcc(index(1): index(end)), index);
                C = zeros(1, length(locs));
                for i = 1: length(locs)
                    C(1, i) = find(index == locs(i));
                end
                for i = 1: length(locs)- 8
                    if locs(i) > 500 && locs(i) < 530 % ��Ƶֱͨ�� ����Ҳ���� ��Ժ����˶�
                        if pks(i) > offsetmin+ XXTT(C(i))
                            colorflag = 'r';
                            presence_flag = 0;
                            plot(max(index), max(offsetmin+ XXTT), 'p', 'MarkerSize', 30, ...
                                'MarkerEdgeColor', 'r', ...
                                'MarkerFaceColor', 'r');
                            hold on;
                            offset = offsetmin; % ��ͼʱ������ʵ����                           
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
                                pks(i+ 8) > offset+ XXTT(C(i+ 8)) % һ���̲��Ǵ̣������̴�������
                            if locs(i) < 550 && locs(i+ 8) > 550 % �ԳƵĶ���̲���
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
                title('΢�����');
				plot(index, offset+ XXTT, 'r')
				hold off;				
            end
			
			if colorflag == 'r' % ���˴��� ���к���Ƶ�ʼ��
				% iceemdanȥ��
				X = raw_data_MF_ANF;
				Nstd = 0.2*std(X);
				[allmode, its] = iceemdan(X, Nstd, 50, 1000, 1);
				[m, n] = size(allmode);
				% �鿴�ֽ�Ч��
				k = 4;
				figure(6)			
				for i = 1: k
					subplot(k, 1, i)
					plot(allmode(i, :));
				end
				% ��ȡ��ģ����
				partmode = allmode(k: end, :);
				raw_data_MF_ANF_iceemdan = sum(partmode, 1).';
				figure(7)			
				plot(timeStamps, raw_data_MF_ANF_iceemdan)
				title('Signal after iceemdan')
				xlabel('t(sec)')
				ylabel('X(t)')
				% Ƶ�����			
				X = raw_data_MF_ANF_iceemdan;
				win_size = length(X);
				w = window(@hamming, win_size); % ������
				X = X .* w; % �Ӵ�
				% FFT
				Y = fft(X, num);
				AP_double = abs(Y)/ num;
				AP_single = AP_double(1: num/ 2);
				AP_single(2: end- 1) = 2*AP_single(2: end- 1); % ���㵥���׷��Ȳ�ȥ����Ƶ�Ŵ�ЧӦ
				f = fs*(0: (num/ 2)- 1)/ num;
				% ���Ƶ�����           
				figure(8)
				plot(f, AP_single)
				title('Single-Sided Amplitude Spectrum of X(t)')
				xlabel('f(Hz)')
				ylabel('|AP_single(f)|')
				xlim([0.1 1])
				
				% MUSIC
				X = raw_data_MF_ANF_iceemdan;
				signalnum = 2; % �ź�����Ч�ɷֵ�����
				signal = X;
				% ����������źŵ�����ؾ�����M=1000;
				M = 1000;
				R = corrmtx(signal, M- 1);
				Rxx = R'* R;
				[V, D] = eig(Rxx); % ����ؾ���������ֵ�ֽ�
	   
				% ����MUSIC�㷨�е�ɨ�躯��P����ȷ����Ч�ɷֵ�Ƶ��
				omega = 2*pi*(0.05:0.005:3)/fs;
				l = length(omega);
				P = zeros(l, 1); % ɨ�躯��
				for i = 1: l
					deno = 0;
					a = exp(-1i*omega(i).*(0: M-1));
					for j=1: M- signalnum
						deno = deno+ abs((a*V(:, j)))^(2)/D(j, j);
					end
					P(i) = 1 / deno;
				end
			
				% ����ɨ�躯��ͼ
				figure(9)
				ff = omega* 60/ (2*pi)*fs;
				plot(ff, P);
				title('Respiratory rate(MUSIC)')
				xlabel('Respiratory rate(min)')
				ylabel('Amplitude')
				
				[pks, locs] = findpeaks(P(:),ff(:));
				[M, I] = max(pks);
				RR = locs(I); % ����ȡ����Ƶ��
				RR_str = num2str(RR);
				string = ['\leftarrow ����Ƶ��' RR_str '��/����'];
				text(RR, M, string, 'Color', 'red', 'FontSize', 26);
			end
                   
            if colorflag == 'g' && presence_flag < 10 % ��������������������
                presence_flag = presence_flag + 1;
            end
            if presence_flag >= 2 % ȷ��������
                colorflag = 'g';
                presence_flag = 10;
            else
                colorflag = 'r';
            end
            figure(10);
            alpha = 0: pi/20 :2*pi; % �Ƕ�[0, 2*pi]
            R = 2; % �뾶
            xx = R*cos(alpha);
            yy = R*sin(alpha);
            plot(xx, yy, '-');
            axis equal
            fill(xx, yy, colorflag); % �ú�ɫ���
            data(:, 1:secnum- 5) = data(:, 6: secnum);
            lastcolor = colorflag;
        end
        cnt = cnt+ 1;
    end
end