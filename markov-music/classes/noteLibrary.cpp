//
//  noteLibrary.cpp
//  N-Gram Music
//
//  Created by tfh on 11/25/15.
//  Copyright © 2015 6.S083. All rights reserved.
//

#include <vector>
#include <map>
#include <algorithm>
#include <array>
#include <deque>
#include <stdlib.h>
#include <math.h>
#include "sndfile.hh"
#include "audio.hpp"
#include "noteLibrary.hpp"
#include "note.hpp"
#include "song.hpp"

NoteLibrary::NoteLibrary(double pThreshold, double dThreshold) {
    pitchThreshold = pThreshold;
    dynamicsThreshold = dThreshold;
}
    
void NoteLibrary::addSong(Song * song) {
    std::vector<Note *> newNotes;
    
    song->splitIntoNotes(&newNotes);
    
    Note * oldNote;
    
    for (std::vector<Note *>::iterator newNote = newNotes.begin(); newNote != newNotes.end(); ++newNote) {
        
        nearbyNotes[* newNote] = * new std::vector<Note *>;
            
        for(std::map<Note *, std::vector<Note *>>::iterator mapIt = nearbyNotes.begin(); mapIt != nearbyNotes.end(); ++mapIt) {
            
            oldNote = mapIt -> first;
            
            double pitchCorrelation = (oldNote) -> comparePitchWithoutShift(* newNote);
            
            if (pitchCorrelation >= pitchThreshold) {
                nearbyNotes[* newNote].push_back(oldNote);
                if (oldNote != * newNote) {
                    nearbyNotes[oldNote].push_back(* newNote);
                }
            }
            
            double dynamicCorrelation = oldNote -> compareDynamics(* newNote);
            
            if (dynamicCorrelation > dynamicsThreshold) {
                nearbyDynamics[* newNote].push_back(oldNote);
                if (oldNote != *newNote) {
                    nearbyDynamics[oldNote].push_back(* newNote);
                }
            }
            
        }
        
    }
}

RealNote * NoteLibrary::chooseRandomNote() {
    
    Note * note = new EndNote();
    
    do {
        auto it = nearbyNotes.begin();
        std::advance(it, rand() % nearbyNotes.size());
        note = it -> first;
    } while (note -> isEndNote());
    
    return dynamic_cast<RealNote *>(note);
}

Note * NoteLibrary::nextNote(std::deque<RealNote *> * noteHistory, property p) {
    
    std::map<Note *, std::vector<Note *>> * nearby = this -> getProperty(p);
    
    std::vector<Note *> potentialNotes = (*nearby)[(*noteHistory)[0]];
    
    for (int i = 1; i < noteHistory -> size(); i++) {
        std::vector<Note *> newPotentialNotes;
        
        std::vector<Note *> nextNotes = (*nearby)[(*noteHistory)[i]];
        
        for (std::vector<Note *>::iterator pNote = potentialNotes.begin(); pNote != potentialNotes.end(); ++pNote) {
            
            Note * nextNote = (* pNote) -> getNextNote();
            
            if(std::find(nextNotes.begin(), nextNotes.end(), nextNote) != nextNotes.end()) {
                newPotentialNotes.push_back(nextNote);
            }
        }
        
        potentialNotes = newPotentialNotes;
    }
    
    int randomInt = rand() % potentialNotes.size();
    
    return potentialNotes[randomInt] -> getNextNote();
}

std::map<Note *, std::vector<Note *>> * NoteLibrary::getProperty(property p) {
    switch (p) {
        case PITCH: return &nearbyNotes;
        case DYNAMICS: return &nearbyDynamics;
    }
}

void NoteLibrary::composeProperty(int order, property p, std::vector<RealNote *> * song, long length) {
    RealNote * start = this -> chooseRandomNote();
    (*song) = {start};
    std::deque<RealNote *>  history = {start};
    
    while (song -> size() < length or length == -1) {
        
        Note* next = this -> nextNote(&history, p);
        
        if (next -> isEndNote()) {
            if (length == -1) {
                return;
            } else {
                RealNote * newStart = this -> chooseRandomNote();
                song -> push_back(newStart);
                history = {newStart};
            }
        } else {
            RealNote * realNext = dynamic_cast<RealNote *>(next);
            song -> push_back(realNext);
            
            history.push_back(realNext);
            while (history.size() > order - 1) {
                history.pop_front();
            }
        }
    }
    
    return;
}

void NoteLibrary::normaliseToPower(std::vector<double> * input, double oldPower, double newPower) {
    for (int i = 0; i < input -> size(); i++) {
        (*input)[i] = (*input)[i] * sqrt(newPower/oldPower);
    }
}

void NoteLibrary::createOutput(std::map<property, std::vector<RealNote *> * > * song, std::vector<std::vector<double>> * output) {
    
    for (int i = 0; i < (*song)[PITCH] -> size(); i++) {
        RealNote * note =(*(*song)[PITCH])[i];
        
        std::vector<std::vector<double>> channels = *note -> getChannels();

        for (int chan = 0; chan < channels.size(); chan ++) {
            normaliseToPower(&channels[chan], note -> getPower(), (*(*song)[DYNAMICS])[i] -> getPower());
            this -> crossFade(0.001 * note -> getSampleRate(), &(*output)[chan], &channels[chan]);
        }
    }
}

void NoteLibrary::crossFade(int crossFadeSamples, std::vector<double> * left, std::vector<double> * right) {
    
    long crossSize = std::min(crossFadeSamples, (int) std::min(left -> size(), right -> size()));
    
    for (int i = 0; i < crossSize; i++) {
        double x = i/(double)crossSize;
        (*left)[left -> size() - crossSize + i] = (*left)[left -> size() - crossSize + i]*(1-x)+ x*(*right)[i];
    }
    
    left -> insert(left -> end(), right -> begin() + crossSize, right -> end());
}


void NoteLibrary::writeToFile(std::map<property, std::vector<RealNote *> *> * song, const char * fileName) {
    
    std::vector<std::vector<double>> output((*(*song)[PITCH])[0] -> getChannels() -> size());
    this -> createOutput(song, &output);
    
    //write to file
    const int format=SF_FORMAT_WAV | SF_FORMAT_FLOAT;
    const int channels = (int) output.size();
    int sampleRate = (*(*song)[PITCH])[0] -> getSampleRate();
    
    std::vector<double> interlacedOutput;
    
    for (int samp = 0; samp < output[0].size(); samp ++) {
        for (int chan = 0; chan < channels; chan++) {
            interlacedOutput.push_back(output[chan][samp]);
        }
    }
    
    SndfileHandle outfile(fileName, SFM_WRITE, format, channels, sampleRate);
    outfile.write(&interlacedOutput[0], interlacedOutput.size());
    
}