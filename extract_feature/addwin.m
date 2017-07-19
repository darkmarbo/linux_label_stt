function y=addwin(x,kind)
len=length(x);
y=zeros(len,3);

for j=1:len
    if x(j)==0
        y(j,1)=-1e+10;
    else
        y(j,1)=log(x(j));
    end
end

y(1,2)=logwin(x(1),x(1),x(2),kind,1);
y(1,3)=logwin(x(1),x(1),x(2),kind,2);

y(len,2)=logwin(x(len-1),x(len),x(len),kind,1);
y(len,3)=logwin(x(len-1),x(len),x(len),kind,2);

for j=2:(len-1)
    y(j,2)=logwin(x(j-1),x(j),x(j+1),kind,1);
    y(j,3)=logwin(x(j-1),x(j),x(j+1),kind,3);
end