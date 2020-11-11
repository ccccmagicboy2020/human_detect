function [res] = auto_trip_50hz(src, fs, lamda, peaks)
    persistent mid;
    
    len = size(src', 1);
    len2 = size(peaks', 1);
    M_FIR = len2 * 2;
    mid = zeros(M_FIR, 1);
    I = eye(M_FIR); % 生成对应的单位矩阵
    c = 0.01; % 小正数 保证矩阵P非奇异
    %y_out = zeros(len, 1);
    Eta_out = zeros(len, 1);
    %w_out = zeros(len, M_FIR);
    P_last = I/c;
    w_last = zeros(M_FIR, 1);
    
    for i = 1: len
        d = src(i); % 输入新的期望信号
        for j = 1: len2
            mid(2*j - 1, 1) = sin(2*pi*peaks(j)*(i- 1)/fs);
            mid(2*j, 1) = cos(2*pi*peaks(j)*(i- 1)/fs);
        end
        Interference = mid;
        K = (P_last * Interference) / (lamda + Interference'* P_last * Interference); % 计算增益矢量
        y = Interference' * w_last; % 计算FIR滤波器输出
        Eta = d - y; % 计算估计的误差
        w = w_last + K * Eta; % 计算滤波器系数矢量
        P = (I - K * Interference') * P_last/lamda; % 计算误差相关矩阵
        % 变量更替
        P_last = P;
        w_last = w;
        % 滤波结果存储
        Eta_out(i) = Eta;                    
    end
    res = Eta_out;
end