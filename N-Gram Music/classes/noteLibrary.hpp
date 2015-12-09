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
    //std::vector<Note*> noteList;
    //std::map<std::array<Note*, 2>, std::array<double, 2>> distanceMap;
    
    //std::map<Note *, std::map<Note *, int>> freqShift;
    
    double pitchThreshold;
    double dynamicsThreshold;
    
    std::map<Note *, std::vector<Note *>> nearbyNotes;
    std::map<Note *, std::vector<Note *>> nearbyDynamics;

public:
    
    enum property {
        PITCH,
        DYNAMICS
    };
    
    NoteLibrary(double pThreshold, double dThreshold);
    
    void addSong(Song * song);
    
    RealNote * chooseRandomNote();
    
    /*
     for all notes that are within threshold distance of thisnote
     choose one at random and output its next note
     */
    Note * nextNote(std::deque<RealNote *> * noteHistory, property p);
    
    int downsample(std::vector<RealNote *> * song);
    
    void composeProperty(int order, property p, std::vector<RealNote *> * song, long length = -1);
    
    void writeToFile(std::vector<RealNote *> * song, const char * fileName);
    
    //void getShiftVector(std::vector<RealNote *> * song, std::vector<int> * output);
    
    //void pitchShift(std::vector<RealNote *> * song, std::vector<int> shiftAmount);
    
    static void crossFade(int crossFadeSamples, std::vector<double> * left, std::vector<double> * right);
    
    std::map<Note *, std::vector<Note *>> * getProperty(property p);
    
    static void normaliseToPower(std::vector<double> * input, double oldPower, double newPower);
    
    void createOutput(std::map<property, std::vector<RealNote *> * > * song, std::vector<double> * output);
    
    void writeToFile(std::map<property, std::vector<RealNote *> *> * song, const char * fileName);
};

#endif /* noteLibrary_hpp */
