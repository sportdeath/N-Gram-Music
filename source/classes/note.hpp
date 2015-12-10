//
//  note.hpp
//  N-Gram Music
//
//  Created by tfh on 11/25/15.
//  Copyright © 2015 6.S083. All rights reserved.
//

#ifndef note_hpp
#define note_hpp

#include <array>
#include "audio.hpp"
#include "fftw3.h"

class Song;

class Note {
public:
    /* Returns whether or not the note is of the EndNote class */
    virtual bool isEndNote() = 0;
    
    /* Returns the note following this one in the  parent song */
    virtual Note * getNextNote() = 0;
    
    /* Returns a value between 0 and 1 representing similarity
     between the dynamics of 2 notes. 1 is identical and 0 is
     completely different */
    virtual double compareDynamics(Note * that) = 0;
    
    /* Returns a value between 0 and 1 representing similarity
     between the pitches of 2 notes. Done by computing the normalised
     dott product between the spectrograms of the two notes. 1 is 
     identical and 0 is completely different */
    virtual double comparePitchWithoutShift(Note * that) = 0;
    
    
    /* Returns a value between 0 and 1 representing similarity
     between the pitches of 2 notes given that we can transpose either note. 
     The output is an array [similarity, maximizing shift]. This is computed
     By finding the transposition amount that maximizes the cross correlation
     between the spectrograms of the two notes. 1 is identical and 0 is 
     completely different */
    virtual void comparePitchWithShift(Note * that, std::array<double, 2> * correlationMatrix) = 0;

};

/*
 A note subclass representing the end of a song. It has no associated
 musical sample. The next note of the last note in a song is an EndNote.
 */
class EndNote : public Note {
public:
    /* Returns true */
    bool isEndNote();
    
    Note * getNextNote();
    
    double compareDynamics(Note * that);
    double comparePitchWithoutShift(Note * that);
    void comparePitchWithShift(Note * that, std::array<double, 2> * correlationMatrix);
};

class RealNote : public Audio, public Note {
    Note * nextNote;
    double power;
    
    static const int spectrogramSize = 60;
    std::array<double, spectrogramSize> spectrogram;
public:
    RealNote(Note * nextNote, Song * song, long startFrame, long endFrame);
    
    /* Gets note sample array from parent song */
    void initializeSampleArray(Song * song, long startFrame, long endFrame);
    
    /* Computes the power (sqrt of sum of squares) of a note */
    void computePower();
    
    /* 
     Frequency estimation algorithm as described int this paper
     http://www.ingelec.uns.edu.ar/pds2803/Materiales/Articulos/AnalisisFrecuencial/00558515.pdf
    */
    static double quinnsSecondEstimator(int maximizingBin, fftw_complex * fft);
    
    /* Auxillary frequency estimation function */
    static double quinnKappa(double in);
    
    /* Computes the spectrogram of a note wrapped to an octave 
     with resolution (spectrogramSize). Uses frequency estimation
     to avoid bias.
     */
    void computeSpectrogram();
    
    /* Returns false */
    bool isEndNote();
    
    /* Returns the precomputed spectrogram */
    std::array<double, spectrogramSize>& getSpectrogram();
    
    /* Returns the precomputed power */
    double getPower();
    

    Note * getNextNote();
    

    double compareDynamics(Note * that);
    double comparePitchWithoutShift(Note * that);
    void comparePitchWithShift(Note * that, std::array<double, 2> * correlationMatrix);
};

#endif /* note_hpp */
