import sys
from scipy.io.wavfile import read,write
from pylab import plot,show,subplot,specgram, title, ylim

rate,data = read(sys.argv[1]) # reading

Fs = rate
NFFT = int(Fs*0.01)	# 10ms window
noverlap = 0			# no overlap

# NFFT is the number of data points used in each block for the FFT
# and noverlap is the number of points of overlap between blocks
specgram(data, NFFT=NFFT, Fs=Fs, noverlap=noverlap) # small window

ylim([0,5000])
title('Chirp signal, Sweep 10-5KHz 10s')
show()
