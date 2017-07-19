function y=logwin(a,b,c,k1,k2)
% k1  0: gn 1 :f0
% k2  1: win1 2 win2
if a*b*c==0 & k1==1
    y=-1e+10;
    return
end

if k2==1
    y = (log(c)-log(a))/2;    
    return;
else 
    y =log(c)+log(a)-2*log(b);    
end