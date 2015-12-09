//
//  song.cpp
//  N-Gram Music
//
//  Created by tfh on 11/25/15.
//  Copyright © 2015 6.S083. All rights reserved.
//

#include <vector>
#include <iostream>
#include "song.hpp"
#include "audio.hpp"
#include "note.hpp"
#include "sndfile.hh"

Song::Song(std::string fileName) {
    SndfileHandle file = SndfileHandle(fileName);

    this -> setSampleRate(file.samplerate());
        
    int N = (int) file.frames();
        
    //Write to array
    double * rawAudioData;
    rawAudioData = new double [ N * file.channels() ];
    file.read(rawAudioData, N * file.channels());
        
    //Read left channel to vector
    std::vector<double> leftChannel(N);
        
    for (int i = 0; i < N; i++) {
        leftChannel[i] = rawAudioData[file.channels() * i];
    }
    
    this -> setSampleArray(leftChannel);
}

void Song::splitIntoNotes(std::vector<Note *> * splitNotes) {
    std::vector<int> startFrames;
    this -> detectOnsets(&startFrames);
    
    splitNotes -> push_back(new EndNote);

    long endFrame = (this -> getSampleArray()) -> size();
    
    for (long i = startFrames.size() - 1; i >= 0; i--) {
        if (startFrames[i] < endFrame) {
            Note * nextNote = (splitNotes -> back());
            
            splitNotes -> push_back(new RealNote(nextNote, this, startFrames[i], endFrame));
            
            endFrame = startFrames[i];
        }
    }
    
    return;
}