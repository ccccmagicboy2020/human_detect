%%%
function [result] = process(file_name, min_peak_distance, scale)
figure('name', file_name)
a = csvread(file_name,0,0);
value = a(:,2);
timestamp = a(:,1);	%Ê±¼ä´Á
save(replace(file_name, '.csv', ''), 'value', 'timestamp')
[pks, locs] = findpeaks(value, 'MinPeakDistance', min_peak_distance, 'MinPeakHeight', mean(value));
hold on;
set(gcf,'Position',[100 100 1024 768]);
set(gca,'Position',[.15 .15 .80 .80]);
plot(value, 'b', 'linewidth',1.5);
plot(locs, pks, "^r");
plot([min(timestamp), max(timestamp)],[mean(value),mean(value)],'--')
plot([min(timestamp), max(timestamp)],[max(value),max(value)],'--')
plot([min(timestamp), max(timestamp)],[median(value),median(value)],'--')
plot([min(timestamp), max(timestamp)],[max(value)*scale,max(value)*scale],'-', 'linewidth',1.5)
legend(file_name, 'peak', 'mean', 'max', 'median', 'result');
text(locs+1,pks,num2str(pks))
hold off;
axis tight;
saveas(gcf, replace(file_name, '.csv', '.png'));
result = max(value)*scale


