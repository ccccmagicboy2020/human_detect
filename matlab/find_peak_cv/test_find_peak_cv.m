%y_value = dlmread('value.txt', ',');
y_value = dlmread('AP_single_401_600.txt', '\t');
[peaks,index] = find_peak_cv(y_value);
hold on;
grid on;
plot(y_value, 'r', 'linewidth', 2);
plot(index, peaks, 'b*')
hold off;
dlmwrite('index.txt', index, 'delimiter', ',', 'newline', 'pc', 'precision', '%d');
dlmwrite('peaks.txt', peaks, 'delimiter', ',', 'newline', 'pc', 'precision', '%5.3f');%index




