/*
	specDisplay.m

	Takes the excel file containing the sound data file (720 chunks of 256 bytes).
	Create an array of the fft of the data.
	Output a text file containing the normalized fft to be used as an input to the video display code.

*/

[num, ~, ~]= xlsread('pianoSample.csv');
i=0;
j=1;
k=256;
numRaw=num; // numRaw will be used to for the spectrogram in matLab to test
while(i<720)
   num(j:k)=fft( num(j:k),256);
   j=j+256;
   k=k+256;
   i=i+1;
end
num=abs(num);
num=num/(max(num));
    
file = fopen('Shashank.txt', 'w');
i=0;
while (i<(720*256))   
 
   fprintf(file,'%.6f,\n' ,num(i+1));
   array(i+1)=num(i+1);
   i=i+1;
end

spectrogram(numRaw,256,[],[],44100,'yaxis')