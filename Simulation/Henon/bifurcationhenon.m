
% Matlab code (see [4]) to demonstrate bifurcation diagram for Hénon map
% x=f(a), b=0.3 and xo=yo=0
%%clc; clear all;
n = input('number of iterations = ');
% fix the parameter b and vary the parameter a
b=0.3;
a=0:0.001:1.4;
% initialization a zero for x and y
% x(0)=y(0)=0
x(:,1)=zeros(size(a,2),1);
y(:,1)=zeros(size(a,2),1);
% iterate the Hénon map
for k=1 : size(a,2)
 for i=1:130
 y(k,i+1)=b*x(k,i);
 x(k,i+1)=1+y(k,i)-a(k)*x(k,i)^2;
 end
end
% display module of the last 50 values of x:
r=a(1,1)*ones(1,51);
m=x(1,80:130);
for k=2 : size(a,2)
 r=[r,a(1,k)*ones(1,51)];
 m=[m,x(k,80:130)];
end
plot(r,m,'.k');
grid;
zoom;