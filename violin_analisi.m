cd('\\wsl$\ubuntu-20.04\home\annafalpi\PAV\P5\samples\C_instruments');

    [y,Fs] = audioread('Violin.wav');
    %sound(y,Fs)
    
    L=length(y);
    Y = fft(y);
    P2 = abs(Y/L);
    P1 = P2(1:L/2+1);
    P1(2:end-1) = 2*P1(2:end-1);
    f = Fs*(0:(L/2))/L;
    plot(f,P1) 
    title('Single-Sided Amplitude Spectrum of X(t)')
    xlabel('f (Hz)')
    ylabel('|P1(f)|')
    %% 
    
    harmonics = zeros(1,15);
    amplitudes = zeros(1,15);
    harmonics(1) = 262.9; amplitudes(1) = 0.2209;
    harmonics(2) = 525.8; amplitudes(2) = 0.09162;
    harmonics(3) = 788.2; amplitudes(3) = 0.04479;
    harmonics(4) = 1047; amplitudes(4) = 0.03961;
    harmonics(5) = 1313; amplitudes(5) = 0.0306;
    harmonics(6) = 1574; amplitudes(6) = 0.01209;
    harmonics(7) = 1837; amplitudes(7) = 0.01583;
    harmonics(8) = 1852; amplitudes(8) = 0.01268;
    harmonics(9) = 2100; amplitudes(9) = 0.0006896;
    harmonics(10) = 2365; amplitudes(10) = 0.01915;
    harmonics(11) = 2627; amplitudes(11) = 0.005669;
    harmonics(12) = 2890; amplitudes(12) = 0.004701;
    harmonics(13) = 3143; amplitudes(13) = 0.004973;
    harmonics(14) = 2627; amplitudes(14) = 0.00245;
     harmonics(15) = 4192; amplitudes(15) = 0.007382;
    cd('\\wsl$\ubuntu-20.04\home\annafalpi\PAV\P5');
    writematrix(harmonics.','violin_f.txt');
    writematrix(amplitudes.','violin_amp.txt');
    
    
    
    sum = 0;
    for i=1:15
        [sine,t] = sinus(harmonics(i),3);
        sum = sum + (amplitudes(i)/amplitudes(1))* sine;
        %plot(sine,t)
    end
    writematrix(sum.','violin_synth.txt');
    audiowrite('violin_synth.wav', sum, Fs); 
    
%sound(sum,Fs)
%     figure
%     L=length(sum);
%     SUM = fft(sum);
%     P2_1 = abs(SUM/L);
%     P1_1 = P2_1(1:L/2+1);
%     P1_1(2:end-1) = 2*P1_1(2:end-1);
%     f_1 = Fs*(0:(L/2))/L;
%     %plot(f_1,P1_1) 
%     title('Single-Sided Amplitude Spectrum of sintesi')
%     xlabel('f (Hz)')
%     ylabel('|P1(f)|')
    
    
    
