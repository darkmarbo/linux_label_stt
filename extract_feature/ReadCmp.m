function data=ReadCmp(fname)
fid = fopen(fname);
a2=fread(fid,2,'long');
b2=fread(fid,2,'short');
data=zeros(a2(1),round(b2(1)/4));
for j=1:a2(1)
    c=fread(fid,round(b2(1)/4),'float');
    data(j,:)=c';
end
fclose(fid);
