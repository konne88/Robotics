#!/usr/bin/octave

X = load('-ascii', 'log.csv');

# [x,y]=meshgrid(X(:,1)*8+X(:,2),X(:,3));

x = [0:15];
y = [0:9];

z = zeros(length(x),length(y));
minC = z;
maxC = z;

size(z)

for i=1:size(X,1)
	e = X(i,:);
	a = e(1)+1;
	b = e(2)+1;
#	x(a) = e(3);
#	y(b) = e(4);
	z(a,b) = e(6);
	minC(a,b) = min(minC(a,b),e(6));
	maxC(a,b) = max(maxC(a,b),e(6));
end
z
c = maxC-minC;

mesh(y,x',z,c) 

pause
