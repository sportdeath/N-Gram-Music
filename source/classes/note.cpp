//
//  note.cpp
//  N-Gram Music
//
//  Created by tfh on 11/25/15.
//  Copyright © 2015 6.S083. All rights reserved.
//

#include <array>
#include <math.h>
#include <vector>
#include <algorithm>
#include "fftw3.h"
#include "note.hpp"
#include "audio.hpp"
#include "song.hpp"


double RealNote::quinnKappa(double in) {
    double firstTerm = log(3*in*in + 6*in +1)/4.;
    
    double top = in + 1 - sqrt(2/3.);
    double bot = in + 1 + sqrt(2/3.);
    double secondTerm = sqrt(6)/24. * log(top/bot);
    
    return firstTerm + secondTerm;
}



double RealNote::quinnsSecondEstimator(int k, fftw_complex * fft) {
    double squaredMagnitude = fft[k][0]* fft[k][0] + fft[k][1]* fft[k][1];
    
    double alpha1 = (fft[k - 1][0]*fft[k][0] + fft[k - 1][1]*fft[k][1])/squaredMagnitude; //Re(fft[k-1]/fft[k])
    double alpha2 = (fft[k + 1][0]*fft[k][0] + fft[k + 1][1]*fft[k][1])/squaredMagnitude; //Re(fft[k+1]/fft[k])
    
    double delta1 = alpha1/(1 - alpha1);
    double delta2 = -alpha2/(1 - alpha2);
    
    
    double delta = (delta1 + delta2)/2. - quinnKappa(delta1*delta1) + quinnKappa(delta2 * delta2);
    
    return delta;
}

bool EndNote::isEndNote() {
    return true;
}


Note * EndNote::getNextNote() {
    return new EndNote();
}

double EndNote::compareDynamics(Note * that) {
    if (that -> isEndNote()) {
        return 1;
    } else {
        return 0;
    }
}

double EndNote::comparePitchWithoutShift(Note * that) {
    if (that -> isEndNote()) {
        return 1;
    } else {
        return 0;
    }
}
    
void EndNote::comparePitchWithShift(Note * that, std::array<double, 2> * correlationMatrix) {
    if (that -> isEndNote()) {
        (*correlationMatrix) = {1, 0};
    } else {
        (*correlationMatrix) = {0, 0};
    }
    return;
}

void RealNote::computeSpectrogram() {
    
    spectrogram = {0};
    
    int size = (int) (*this -> getSampleArray()).size();
    
    double * fftIn = &(*this -> getSampleArray())[0];
    fftw_complex * fftOut = new fftw_complex[size/2 + 1];
    
    fftw_plan plan = fftw_plan_dft_r2c_1d(size, fftIn, fftOut, FFTW_ESTIMATE);
    
    fftw_execute(plan);
    
    fftw_destroy_plan(plan);

    //Fill with magnitudes of peak values
    for (int bin = 2; bin < size/2; bin++) {
        
        double leftMag = fftOut[bin-1][0]*fftOut[bin-1][0]+fftOut[bin-1][1]*fftOut[bin-1][1];
        double midMag = fftOut[bin][0]*fftOut[bin][0]+fftOut[bin][1]*fftOut[bin][1];
        double rightMag = fftOut[bin+1][0]*fftOut[bin+1][0]+fftOut[bin+1][1]*fftOut[bin+1][1];
        
        if ((midMag >rightMag) && (midMag > leftMag)) {
            
            double delta = quinnsSecondEstimator(bin,fftOut);
            
            if ( fabs(delta) < 1) {
                double peakBin = bin + delta;
                
                double peakFreq = peakBin * (this -> getSampleRate())/size;
                double freqMod =fmod(log2(peakFreq), 1);
                if (freqMod < 0 ) {
                    freqMod += 1;
                }
                
                double desired =freqMod* spectrogramSize;
                
                int leftNote = floor(desired);
                int rightNote = (leftNote + 1) % spectrogramSize;
                
                double rightPercent = desired - leftNote;
                double leftPercent = 1 - rightPercent;
                
                double rightAmp = rightPercent * sqrt(fftOut[bin][0] * fftOut[bin][0] + fftOut[bin][1] * fftOut[bin][1]);
                double leftAmp = leftPercent * sqrt(fftOut[bin][0] * fftOut[bin][0] + fftOut[bin][1] * fftOut[bin][1]);
                
                spectrogram[leftNote] += leftAmp;
                spectrogram[rightNote] += rightAmp;
            }
        }
    }
    
    //Find max of spectrogram
    double max = spectrogram[0];
    for (int i = 0; i < spectrogramSize; i++) {
        if (spectrogram[i] > max) {
            max = spectrogram[i];
        }
    }
    
    //Normalize spectrogram using max
    for (int i = 0; i < spectrogramSize; i++) {
        spectrogram[i] = spectrogram[i]/max;
    }
}

void RealNote::initializeSampleArray(Song * song, long startFrame, long endFrame) {
    std::vector<std::vector<double>> * songChannels = song -> getChannels();
    
    std::vector<std::vector<double>> noteChannels(song -> getChannels() -> size());
    
    for (int chan = 0; chan < songChannels -> size(); chan ++) {
        std::vector<double> sampleVector((*songChannels)[chan].begin() + startFrame,(*songChannels)[chan].begin() + endFrame);
        noteChannels[chan] = sampleVector;
    }
    
    this -> setSampleArray(noteChannels);
}

RealNote::RealNote(Note * next, Song * song, long startFrame, long endFrame) {
    
    nextNote = next;
    
    this -> setSampleRate(song -> getSampleRate());
    
    this -> initializeSampleArray(song, startFrame, endFrame);
    
    this -> computeSpectrogram();
    
    this -> computePower();
}
    
std::array<double, RealNote::spectrogramSize>& RealNote::getSpectrogram() {
    return spectrogram;
}

bool RealNote::isEndNote() {
    return false;
}

Note * RealNote::getNextNote() {
    return nextNote;
}

void RealNote::computePower() {
    power = 0;
    std::vector<double> * samples = this -> getSampleArray();
    for (std::vector<double>::iterator sample = samples -> begin() ; sample != samples -> end(); ++sample) {
        power += (*sample) * (*sample);
    }
    
    power = power/(samples -> size());
}

double RealNote::getPower() {
    return power;
}

double RealNote::compareDynamics(Note * that) {
    if (that -> isEndNote()) {
        return 0;
    }
    
    RealNote * thatNote;
    
    thatNote = dynamic_cast<RealNote *>(that);
    
    double thisPower = this -> getPower();
    double thatPower = thatNote -> getPower();
    if (thisPower < thatPower) {
        return thisPower/thatPower;
    } else {
        return thatPower/thisPower;
    }
}

double RealNote::comparePitchWithoutShift(Note * that) {
    
    if (that -> isEndNote()) {
        return 0;
    }
    
    RealNote * thatNote;
    
    thatNote = dynamic_cast<RealNote *>(that);
    
    std::array<double, spectrogramSize> thatSpectrogram = thatNote->getSpectrogram();
    
    double thisMag = 0, thatMag = 0;
    
    for (int i = 0; i < spectrogramSize; i++) {
        thisMag += spectrogram[i] * spectrogram[i];
        thatMag += thatSpectrogram[i] * thatSpectrogram[i];
    }
    
    thisMag = sqrt(thisMag);
    thatMag = sqrt(thatMag);
        
    //Compute correlation
    double correlation =  0;
    for ( int j = 0; j < spectrogramSize; j++) {
        correlation += spectrogram[j] * thatSpectrogram[(j) % spectrogramSize];
    }
    
    return correlation/(thisMag*thatMag);
}

void RealNote::comparePitchWithShift(Note * that, std::array<double, 2> * correlationArray) {
    
    if (that -> isEndNote()) {
        (*correlationArray)= {0,0};
        return;
    }
    
    RealNote * thatNote;
    
    thatNote = dynamic_cast<RealNote *>(that);
    
    std::array<double, spectrogramSize> thatSpectrogram = thatNote->getSpectrogram();
    
    double thisMag = 0, thatMag = 0;
    
    for (int i = 0; i < spectrogramSize; i++) {
        thisMag += spectrogram[i] * spectrogram[i];
        thatMag += thatSpectrogram[i] * thatSpectrogram[i];
    }
    
    thisMag = sqrt(thisMag);
    thatMag = sqrt(thatMag);
    
    //Compute cross-correlation
    double maxCorrelation = 0;
    int maximizingShift = 0;
    double correlation;
    for ( int i = 0; i < spectrogramSize; i++) {
    correlation = 0;
        for ( int j = 0; j < spectrogramSize; j++) {
            correlation += spectrogram[j] * thatSpectrogram[(i + j) % spectrogramSize];
        }
        if (correlation > maxCorrelation) {
            maxCorrelation = correlation;
            maximizingShift = i;
        }
    }
    
    (*correlationArray) = {maxCorrelation/(thisMag*thatMag), static_cast<double>(maximizingShift)};
    
    return;
}
