% programa utilizado para realizar filtrado de una muestra exportada de un
% archivo que se deberá guardar en la matriz TEST1
%El programa realizará un filtrado pasabandas.
fc_1=3; %frecuencia de corte 1
fc_2=9; %frecuancia de corte 2
fs=1250; %frecuencia de la toma de datos 
w1=fc_1/fs;
w2=fc_2/fs;
x_1=0;
x_0=0;
wn=[w1 w2];
b=fir1(80,wn,'bandpass'); %variables para un filtro pasabandas de orden 80

y=table2array(TEST1); %pasar la tabla del archivo de muestras a una matriz
yf=filter(a,1,y);   %matriz con los valores de la señal después del filtrado


plot(y);
hold on
plot(yf);