close all
clear all
clc

filename = 'C:/Users/superian/Desktop/2.csv'; % CSV�ļ���
T = readtable(filename); % ��ȡCSV�ļ�

figure;
plot(T.Var1)
hold on
plot(T.Var2)
hold on
plot(T.Var3)
grid on

figure;
plot(T.Var1)
grid on

figure;
plot(fft(T.Var2))
grid on

figure;
plot(T.Var3)
grid on