close all
clear
clc

N=10;
Av = 15;
Aa = 1.5;
phaseshift = 30/180*pi;
f = 50;
w = 2*pi*f;

%%
%set up plot for smooth waveform
T_fine = linspace(0,1/f,10000/f);
V_fine = Av*sin(w*T_fine);
%%
% set up plot
T_samples = linspace(0, 1/f, N+1);
Tsample = 1/f/N;
T_samples_A = T_samples + Tsample/2;
%%
% calculate and plot voltage input raw and sampled points
V = Av*sin(w*T_samples);
A = Aa*sin(w*T_samples_A+phaseshift);
Vrms = Av/sqrt(2)
Arms = Aa/sqrt(2)
P_actual = Vrms*Arms*cos(phaseshift)
Vrms_ADC = Vrms*1024/5;
figure('Name','Vac')
plot(T_samples, V, 'o')
hold on
plot(T_samples_A,A,'x-')

plot(T_fine,V_fine)

xlabel('t (ms)')
ylabel('V')
grid on

% calculate and plot the Vvf
Gvs = 0.1;
Gvo = 1;
Voff = 2;

Gis = 0.5;
Gio = 2;

Vvs = V*Gvs*Gvo +Voff;
Vis = A*Gis*Gio + Voff;

figure('Name', 'Vvs')
plot(T_samples, Vvs, 'o-')
hold on
plot(T_samples_A, Vis, 'x-')
hold on
xlabel('t (ms)')
ylabel('V')
grid on

% calculate and plot the ADC value
VADCValue = Vvs*1024/5;
AADCValue = Vis*1024/5;
%This value has both the AC and DC encoded in digital form!
% we only want the AC RMS in the digital domain
figure('Name','Vvs_ADC')
plot(T_samples,VADCValue, 'o-')
hold on
plot(T_samples_A,  AADCValue, 'o-')
hold on
xlabel('t (ms)')
ylabel('V')
grid on

P_discrete = (VADCValue*5/1024-Voff)/(Gvo*Gvs).*(AADCValue*5/1024-Voff)/(Gio*Gis);

figure('Name', 'Power_discrete')
plot(T_samples_A, P_discrete,'o-')
hold on
xlabel('t (ms)')
ylabel('V')
grid on

P_predicted = 1/(N)*(sum((VADCValue(1:N)*5/1024-Voff)/(Gvo*Gvs).*(AADCValue(1:N)*5/1024-Voff)/(Gio*Gis)))

error_percent = (P_actual - P_predicted)/P_actual * 100

%% realign
AADCValue_realigned = [(AADCValue(11)+AADCValue(10)),(AADCValue(1)+AADCValue(2)),(AADCValue(2)+AADCValue(3)),(AADCValue(3)+AADCValue(4)),(AADCValue(4)+AADCValue(5)),(AADCValue(5)+AADCValue(6)),(AADCValue(6)+AADCValue(7)),(AADCValue(7)+AADCValue(8)),(AADCValue(8)+AADCValue(9)),(AADCValue(9)+AADCValue(10)),(AADCValue(10)+AADCValue(11))]/2

figure('Name','ADC realigned')
plot(T_samples, VADCValue,'o-', 'DisplayName','Raw Voltage ADC')
hold on
plot(T_samples_A, AADCValue, 'o-', 'DisplayName','Raw Current ADC')
hold on
plot(T_samples, AADCValue_realigned, 'x-', 'DisplayName','Adjusted Voltage ADC')
xlabel('t (ms)')
ylabel('V')
grid on
legend

P_predicted_corrected = 1/(N)*(sum((VADCValue(1:N)*5/1024-Voff)/(Gvo*Gvs).*(AADCValue_realigned(1:N)*5/1024-Voff)/(Gio*Gis)))

error_percent_corrected = (P_actual-P_predicted_corrected)/P_actual*100