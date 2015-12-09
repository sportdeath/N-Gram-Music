//
//  main.cpp
//  N-Gram Music
//
//  Created by tfh on 11/20/15.
//  Copyright (c) 2015 6.S083. All rights reserved.
//

#include <iostream>
#include <stdlib.h>
#include <dirent.h>
#include <fstream>
#include <string>
#include "noteLibrary.hpp"

/*
 given random starting note

 Takes Sound file inputs and returns its left channel as an array of doubles.
 The array has file.frames() number of samples.
 */

int main(int argc, char* argv[]) {
    
    std::string songDirectory;
    std::string outputDirectory;
    
    double pitchThreshold = 0.8;
    double dynamicsThreshold = 0.8;
    int pitchOrder = 3;
    int dynamicsOrder = 3;
    
    int songsToGenerate = 5;
    
    if ( argc == 2 ) {
        songDirectory = argv[1];
        outputDirectory = songDirectory.substr(0, songDirectory.length() - 4) + "-markov.wav";
    } else if ( argc == 3 ) {
        songDirectory = argv[1];
        outputDirectory = argv[2];
    } else if ( argc == 4 ) {
        songDirectory = argv[1];
        outputDirectory = argv[2];
        pitchOrder = atoi(argv[3]);
        dynamicsOrder = atoi(argv[4]);
        if ( pitchOrder < 1 or dynamicsOrder < 1 ) {
            printf("Markov orders must be integers > 0");
            return 0;
        }
    } else {
        printf("Use one of the following:\n");
        printf("./markov-music songDirectory\n");
        printf("./markov-music songDirectory outputDirectory\n");
        printf("./markov-music songDirectory outputDirectory pitchOrder dynamicsOrder\n");
        return 0;
    }
    
    NoteLibrary * library = new NoteLibrary(pitchThreshold, dynamicsThreshold);
    
    printf("Adding file \"%s\"\n", songDirectory.c_str());
    Song * songFile = new Song(songDirectory);
    if ( songFile -> isInvalid() ) {
        printf("\"%s\" is not a valid wav file.\n", songDirectory.c_str());
        return 0;
    }
    library -> addSong(songFile);
    
    printf("Generating songs ...\n");
    
    
    std::vector<RealNote *> writePitches;
    
    for (int i = 0; i < songsToGenerate; i++) {
        std::vector<RealNote *> generatedPitches;
        library -> composeProperty(pitchOrder, NoteLibrary::PITCH, &generatedPitches);
        printf("... with %lu notes\n", generatedPitches.size());
        if (writePitches.empty() or generatedPitches.size() > writePitches.size()) {
            writePitches = generatedPitches;
        }
    }
    
    printf("Choosing song with %lu notes.\n", writePitches.size());
    
    printf("Generating dynamics...\n");
    std::vector<RealNote *> dynamics;
    library -> composeProperty(dynamicsOrder, NoteLibrary::DYNAMICS, &dynamics, writePitches.size());
    
    printf("Writing song to \"%s\".\n", outputDirectory.c_str());
    std::map<NoteLibrary::property, std::vector<RealNote *> *> song;
    song[NoteLibrary::PITCH] = &writePitches;
    song[NoteLibrary::DYNAMICS] = &dynamics;
    library ->writeToFile(&song, outputDirectory.c_str());
    
    return 0;
}

