#!/usr/bin/octave
# cu -l /dev/ttyACM0 -s 9600
# cat log.csv | egrep -v --color "[0-9]+,[0-9]+,[0-9]+,[0-9]+"

X = load('-ascii', 'log.csv');

X = sortrows(X,1);

hold on

plot(X(:,1),X(:,2),"-");
# plot(X(:,1),X(:,3));
plot(X(:,1),ones(size(X,1),1)*484,"-1")
plot(X(:,1),X(:,4)*40+484-20,"-1")

pause

