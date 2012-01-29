#!/usr/bin/octave

X = load('-ascii', 'log.csv');

# [x,y]=meshgrid(X(:,1)*8+X(:,2),X(:,3));

x = [0:31];
y = [0:19];

z = zeros(length(x),length(y));

size(z)

for i=1:size(X,1)
	e = X(i,:);
	if(e(1) < 4)
		z(e(1)*8+e(2)+1,e(3)+1) = e(6);
	end
end
z

mesh(y,x',z) 

pause
