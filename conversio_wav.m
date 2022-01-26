cd('\\wsl$\ubuntu-20.04\home\annafalpi\PAV\P5\samples\C_instruments');

instruments = dir('*.wav'); 
for i=1:length(instruments)
    cd('\\wsl$\ubuntu-20.04\home\annafalpi\PAV\P5\samples\C_instruments');
    [y,Fs] = audioread(instruments(i).name);
    sound(y,Fs)
    name = strsplit(instruments(i).name,'.');            
    nom = string(name(1)) ; 
    cd('\\wsl$\ubuntu-20.04\home\annafalpi\PAV\P5');
    writematrix(y,strcat(nom,'.txt'));
    t=(1:1:length(y));
    plot(t, y.')
end



