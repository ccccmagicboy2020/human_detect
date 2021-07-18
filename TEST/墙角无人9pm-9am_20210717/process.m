%%%
function [result] = process(file_name, min_peak_distance, scale)
figure('name', file_name)
a = csvread(file_name,0,0);
p = a(:,2);
k = a(:,1);	%ʱ���
save(replace(file_name, '.csv', ''), 'p', 'k')
[pks, locs] = findpeaks(p, 'MinPeakDistance', min_peak_distance, 'MinPeakHeight', mean(p));
hold on;
set(gcf,'Position',[100 100 1024 768]);
set(gca,'Position',[.15 .15 .80 .80]);
plot(p, 'b', 'linewidth',1.5);
plot(locs, pks, "^r");
plot([min(k), max(k)],[mean(p),mean(p)],'--')
plot([min(k), max(k)],[max(p),max(p)],'--')
plot([min(k), max(k)],[median(p),median(p)],'--')
plot([min(k), max(k)],[max(p)*scale,max(p)*scale],'-', 'linewidth',1.5)
legend(file_name, 'peak', 'mean', 'max', 'median', 'result');
text(locs+1,pks,num2str(pks))
hold off;
axis tight;
saveas(gcf, replace(file_name, '.csv', '.png'));
result = max(p)*scale


