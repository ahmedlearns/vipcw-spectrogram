vipcw-spectrogram
=================

Make library object: "gcc -lm -o fft.o -c fft.c"
To compile test: "gcc fft_testing.c fft.o -o fft_testing -lm"
Compile monofft: "gcc monofft.c fft.o -o runMonofft -lm"


This project will sample a .wav file using an fft and afterwardsc
represent it in a graphical manner to be used on a Raspberry Pi.

After using an fft to transform a signal from the time domain to the frequency
domain, we can use a python script to show the spectrogram. 

FFT Calculator: http://www.random-science-tools.com/maths/FFT.htm

FFT Test Vectors: http://www.sccon.ca/sccon/fft/fft3.htm

Understanding FFT's: http://www.youtube.com/watch?v=H144ipQa22Q

WaveFormat: https://ccrma.stanford.edu/courses/422/projects/WaveFormat/

FFT basics:
http://www.dspguru.com/dsp/faqs/fft
http://www.dspguide.com/ch12/2.htm


To run fft.c:

Change to the directory for the shared folder CW

There should be a script file in the folder called compileOPT. We will use this file to compile the FFT code that turns a WAV file into a single channel then perform the transform, finally outputting the values into a text file.

While in the same directory, type in the console ./compileOPT to compile the code using the board's compiler.
A new file named fft will now appear in the directory. We will use this on the board.

Now, in order to make it work on the board we need to move the fft file and the bird.wav file to the directory where the board is located.

Change the directory to home/ti/filesys/home/root and drop the two files here.

Boot up the board through SecureCRT. You should be in the correct directory where the previous files were moved.

To run the code on the board, type ./fft

You should now have a few new files in the directory, namely
	monobird.wav
	birdfft.txt

The values in birdfft.txt should already be normalized. These are then passed to the video display.

