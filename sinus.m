function [x,t] = sinus(Fc,durada)
Fs = 44100;                                    
t = (0:1/Fs:durada-1/Fs);   
x = sin(2*pi*Fc*t);
end

