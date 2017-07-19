function writecmp(x,fout)

[lsp,f0,gain] = getinfo(x);
lsp=[gain';lsp];

f0=addwin(f0,1);

[m,n]=size(lsp);

for j=1:n
    lsp(1,j)=log(lsp(1,j));
end

lspap=zeros(75,n);
lspap(1:25,:)=lsp;
lspap(26:50,1)=(  lspap(1:25,3) - lspap(1:25,1) )/2;
lspap(51:75,1)=lspap(1:25,3) + lspap(1:25,1) -2* lspap(1:25,2);

for j=2:n-1
    lspap(26:50,j) = (  lspap(1:25,j+1) - lspap(1:25,j-1) )/2;
    lspap(51:75,j) = lspap(1:25,j+1) + lspap(1:25,j-1) -2* lspap(1:25,j);
end
lspap(26:50,n)= ( lspap(1:25,n)-lspap(1:25,n-2) ) /2;
lspap(51:75,n)= lspap(1:25,n) + lspap(1:25,n-2) -2* lspap(1:25,n-1) ;

pars=[lspap',f0];

a=[n,50000];
b=[(78)*4,9];
fid=fopen(fout,'w');
a=int32(a);
b=int16(b);
fwrite(fid,a,'long');
fwrite(fid,b,'short');
fwrite(fid,pars','float');
fclose(fid);