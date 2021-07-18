%%%
p0 = process('test-p0.csv', 200, 1.02)
p1 = process('test-p1.csv', 200, 1.02)
p2 = process('test-p2.csv', 200, 1.02)
p3 = process('test-p3.csv', 200, 1.02)
p4 = process('test-p4.csv', 600, 1.02)
p5 = process('test-p5.csv', 600, 1.02)
fid=fopen('result.txt','w');
fprintf(fid,'p0: %.3f\r\n', p0);
fprintf(fid,'p1: %.3f\r\n', p1);
fprintf(fid,'p2: %.3f\r\n', p2);
fprintf(fid,'p3: %.3f\r\n', p3);
fprintf(fid,'p4: %.3f\r\n', p4);
fprintf(fid,'p5: %.3f\r\n', p5);
fclose(fid);
p0_raw = load('test-p0.mat', 'value')
p1_raw = load('test-p1.mat', 'value')
p2_raw = load('test-p2.mat', 'value')
p3_raw = load('test-p3.mat', 'value')
p4_raw = load('test-p4.mat', 'value')
p5_raw = load('test-p5.mat', 'value')
