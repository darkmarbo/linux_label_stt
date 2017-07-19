
%addpath(genpath('/usr/local/matlab/2014a/toolbox/straightpcode/STRAIGHTVpcode'));
addpath(genpath('/usr/local/matlab/2014a/toolbox/STRAIGHTV40_006b/STRAIGHTV40_006b'));

files = dir('wave/*.wav')
num_file=length(files)
disp(['file_num_is:',num_file])
for i=1:num_file
    file_name = files(i).name
    
    x=wavread(fullfile('wave',file_name));
    pos=findstr(file_name,'.');
    name=file_name(1:pos-1);
    
    name_cmp=strcat(name,'.cmp');
    disp(name_cmp)
    writecmp(x,fullfile('cmp',name_cmp));

end

%x=wavread('sample.wav');
%writecmp(x,'test.cmp');

exit();


