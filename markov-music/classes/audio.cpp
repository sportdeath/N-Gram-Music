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
    return &sampleArrays[0];
}

std::vector<std::vector<double>> * Audio::getChannels() {
    return &sampleArrays;
}

int Audio::getSampleRate() {
    return sampleRate;
}

void Audio::setSampleArray(std::vector<std::vector<double>> array) {
    sampleArrays = array;
}

void Audio::setSampleRate(int rate) {
    sampleRate = rate;
}
