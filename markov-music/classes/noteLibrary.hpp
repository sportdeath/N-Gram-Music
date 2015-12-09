//
//  noteLibrary.hpp
//  N-Gram Music
//
//  Created by tfh on 11/25/15.
//  Copyright © 2015 6.S083. All rights reserved.
//

#ifndef noteLibrary_hpp
#define noteLibrary_hpp

#include <stdio.h>
#include <map>
#include <deque>
#include <vector>
#include "note.hpp"
#include "song.hpp"

class NoteLibrary {
    
    double pitchThreshold;
    double dynamicsThreshold;
    
    /* Maps from a NOTE to a list of notes representing
     notes whose pitch and dynamics are witin their respective
     thresholds of NOTE. */
    std::map<Note *, std::vector<Note *>> nearbyNotes;
    std::map<Note *, std::vector<Note *>> nearbyDynamics;

public:
    
    enum property {
        PITCH,
        DYNAMICS
    };
    
    NoteLibrary(double pThreshold, double dThreshold);
    
    /*
     Adds all the notes in a song into the song library.
     */
    void addSong(Song * song);
    
    /*
     Randomly choses a note. Used to pick the start note
     */
    RealNote * chooseRandomNote();
    
    /*
     Randomly choses a new note whose predecessors have similar
     properties to those in noteHistory.
     */
    Note * nextNote(std::deque<RealNote *> * noteHistory, property p);
    
    
    /* 
     Composes a list of notes by selecting for a particular property p.
     If length = -1, the list is generated until an EndNote is reached,
     otherwise, the list is generated until it is of the desired length.
     */
    void composeProperty(int order, property p, std::vector<RealNote *> * song, long length = -1);
    
    /*
     Crossfades between two arrays over a length of at most crossFadeSamples
     */
    static void crossFade(int crossFadeSamples, std::vector<double> * left, std::vector<double> * right);
    
    /*
     Returns the map of nearby notes corresponding to property p
     */
    std::map<Note *, std::vector<Note *>> * getProperty(property p);
    
    /*
     Normalizes an array with an input power to a new power (sqrt of sum of squares)
     */
    static void normaliseToPower(std::vector<double> * input, double oldPower, double newPower);
    
    /*
     Creates an output array given a composition with both PITCH and DYNAMICS components.
     */
    void createOutput(std::map<property, std::vector<RealNote *> * > * song, std::vector<std::vector<double>> * output);
    
    /*
     Writes a composition to wav file
     */
    void writeToFile(std::map<property, std::vector<RealNote *> *> * song, const char * fileName);
};

#endif /* noteLibrary_hpp */
