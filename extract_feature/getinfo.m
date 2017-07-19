function [lsp,f0raw,gain]=getinfo(x)
lop=5;ord=24;
fs=16000;
[f0raw,ap,analysisParams]=exstraightsource(x,fs);
clear ap;
n3sgram = exstraightspec(x,f0raw,fs);
[m,n]=size(n3sgram);
n=min(n,length(f0raw));

n3sgram=n3sgram(:,1:lop:n);
f0raw=f0raw(1:lop:n);
[m,n]=size(n3sgram);

for i=1:n
    lsp(:,i)=poly2lsf(spec2lpc(n3sgram(:,i),ord));
end

gain=zeros(n,1);
for i=1:n
    lpc(:,i)=lsf2poly(lsp(:,i));
    [h,w]=freqz([1],lpc(:,i),512);
    h=abs(h);
    gain(i)=max(n3sgram(1:512,i))/max(h);
end