
%addpath(genpath('/usr/local/matlab/2014a/toolbox/straightpcode/STRAIGHTVpcode'));
addpath(genpath('/usr/local/matlab/2014a/toolbox/STRAIGHTV40_006b/STRAIGHTV40_006b'));

% 输入 000001 表示 wave/000001.wav
base_name = ddd
wav_name = strcat(base_name,'.wav');
disp(['path_wav: wave/', wav_name])

x=wavread(fullfile('wave', wav_name));
    
cmp_name=strcat(base_name,'.cmp');

disp(cmp_name)

writecmp(x, fullfile('cmp', cmp_name));


%x=wavread('sample.wav');
%writecmp(x,'test.cmp');

exit();


