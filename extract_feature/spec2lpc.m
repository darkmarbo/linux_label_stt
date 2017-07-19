function y=spec2lpc(x,order)
x=x.^2;
x=[x;x(512:-1:2)];
u=real(ifft(x));
y=levinson(u,order);