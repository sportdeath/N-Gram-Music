//
//  audio.cpp
//  N-Gram Music
//
//  Created by tfh on 11/25/15.
//  Copyright © 2015 6.S083. All rights reserved.
//

#include <vector>
#include <math.h>
#include <algorithm>
#include "audio.hpp"
#include "fftw3.h"
#include "detectionfunctions.h"



std::vector<double> * Audio::getSampleArray() {
    return &sampleArray;
}

int Audio::getSampleRate() {
    return sampleRate;
}

void Audio::setSampleArray(std::vector<double> array) {
    sampleArray = array;
}

void Audio::setSampleRate(int rate) {
    sampleRate = rate;
}

void Audio::detectOnsets(std::vector<int> * onsets) {
    
    onsets -> push_back(0);
    int frameSize = 2048;
    int hopSize = 512;
    
    OnsetDetectionFunction * odf = new ComplexODF();
    odf -> set_frame_size(frameSize);
    odf -> set_hop_size(hopSize);
    odf -> set_sampling_rate(sampleRate);
    double * odfValues = new double [sampleArray.size()/hopSize];
    
    odf -> process((int) sampleArray.size(), &sampleArray[0], (int) sampleArray.size()/hopSize, odfValues);
    
    int peakWindow = ceil(0.02*sampleRate/hopSize); //audio transient in music around 20ms
    
    for (int i = peakWindow; i < sampleArray.size()/hopSize - peakWindow; i++) {
        bool isOnset = true;
        for (int j = 1; j < peakWindow; j++) {
            if (odfValues[i] < 0.1 || 1.5*odfValues[i] < odfValues[i + j] || odfValues[i] < 1.5*odfValues[i - j]) {
                isOnset = false;
            }
        }
        if (isOnset) {
            onsets -> push_back(i*hopSize);
        }
    }
    
    //plotOnsets(&sampleArray, odfValues, onsets);
    
    return;
    
}


//plot 'audio' w l, 'transients' w l, 'onsets' w p
void Audio::plotOnsets(std::vector<double> * sampleArray, double * odfValues, std::vector<int> * onsets) {

    int hopSize = 512;
    
    FILE * audioFile;
    audioFile = fopen ("/Users/tfh/Dropbox (Personal)/6.S083/FinalProject/N-Gram Music/N-Gram Music/audio","w");
           
    FILE * transFile;
    transFile = fopen ("/Users/tfh/Dropbox (Personal)/6.S083/FinalProject/N-Gram Music/N-Gram Music/transients","w");
           
    FILE * onsetFile;
    onsetFile = fopen ("/Users/tfh/Dropbox (Personal)/6.S083/FinalProject/N-Gram Music/N-Gram Music/onsets","w");
           
    for (int n=0 ; n< sampleArray -> size()/hopSize; n++) {
        fprintf (audioFile, "%i\t%f\n", n, (*sampleArray)[hopSize * n]);
        fprintf (transFile, "%i\t%f\n", n, odfValues[n]);
        
    }
           
    for (int n=0 ; n< onsets -> size(); n++) {
        fprintf (onsetFile, "%i\t%f\n", (*onsets)[n]/hopSize, 0.5);
    }
           
    fclose(onsetFile);
    fclose(audioFile);
    fclose(transFile);
    
    return;
}
