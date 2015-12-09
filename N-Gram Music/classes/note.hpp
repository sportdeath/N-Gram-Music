//
//  note.hpp
//  N-Gram Music
//
//  Created by tfh on 11/25/15.
//  Copyright © 2015 6.S083. All rights reserved.
//

#ifndef note_hpp
#define note_hpp

#include <stdio.h>
#include <array>
#include "audio.hpp"
#include "fftw3.h"
//#include "song.hpp"

class Song;

class Note {
public:
    
    //The size of the spectrogram
    static const int spectrogramSize = 60;
    
    virtual bool isEndNote() = 0;
    
    /*
     Returns the note that follows
     */
    virtual Note * getNextNote() = 0;
    
    /*
     {distance, minimizingShift}
     */
    
    virtual double compareDynamics(Note * that) = 0;
    virtual double comparePitchWithoutShift(Note * that) = 0;
    virtual void comparePitchWithShift(Note * that, std::array<double, 2> * correlationMatrix) = 0;
    virtual double compareLengths(Note * that) = 0;
    
    static double magnitude(int binNumber, fftw_complex *fft);
    static double quinnTau(double in);
    static double quinnsSecondEstimator(int maximizingBin, fftw_complex * fft);
};

class EndNote : public Note {
public:
    bool isEndNote();
    Note * getNextNote();
    
    double compareDynamics(Note * that);
    double comparePitchWithoutShift(Note * that);
    void comparePitchWithShift(Note * that, std::array<double, 2> * correlationMatrix);
    double compareLengths(Note * that);
};

class RealNote : public Audio, public Note {
    Note * nextNote;
    std::array<double, spectrogramSize> spectrogram;
    double power;
public:
    RealNote(Note * nextNote, Song * song, long startFrame, long endFrame);
    
    void initializeSampleArray(Song * song, long startFrame, long endFrame);
    void computeSpectrogram();
    void computePower();
    
    bool isEndNote();
    
    std::array<double, spectrogramSize>& getSpectrogram();
    double getPower();
    Note * getNextNote();
    
    double compareDynamics(Note * that);
    double comparePitchWithoutShift(Note * that);
    void comparePitchWithShift(Note * that, std::array<double, 2> * correlationMatrix);
    double compareLengths(Note * that);
};

#endif /* note_hpp */
