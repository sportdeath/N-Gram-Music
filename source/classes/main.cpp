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
 Generates a new song using markov chains from a source song.
 The pitch and dynamics are generated independently and their
 markov orders can be adjusted. Because some generated songs
 can be extremely short, 10 are generated and the one with closest to
 2 minutes of material is chosen. If no output directory
 is given, the output will be in the same directory as the input 
 with "-markov" appended to the name.
 */
int main(int argc, char* argv[]) {
    
    std::string songDirectory;
    std::string outputDirectory;
    
    double pitchThreshold = 0.8;
    double dynamicsThreshold = 0.9;
    int pitchOrder = 3;
    int dynamicsOrder = 4;
    
    int songsToGenerate = 10;
    
    double desiredMinutes = 2;
    
    //Parse arguments
    if ( argc == 2 ) {
        songDirectory = argv[1];
        outputDirectory = songDirectory.substr(0, songDirectory.length() - 4) + "-markov.wav";
    } else if ( argc == 3 ) {
        songDirectory = argv[1];
        outputDirectory = argv[2];
    } else if ( argc == 6 ) {
        songDirectory = argv[1];
        outputDirectory = argv[2];
        pitchOrder = atoi(argv[3]);
        dynamicsOrder = atoi(argv[4]);
        if ( pitchOrder < 1 ) {
            printf("Pitch order must be an integer > 0");
            return 0;
        }
        desiredMinutes = atof(argv[5]);
    } else {
        printf("Use one of the following:\n");
        printf("./markov-music songDirectory\n");
        printf("./markov-music songDirectory outputDirectory\n");
        printf("./markov-music songDirectory outputDirectory pitchOrder dynamicsOrder desiredTime\n");
        printf("\n If dynamicsOrder <= 0, dynamics will be disabled.\n");
        printf("If desiredTime <= 0, the outputted song will be about the same length as the input song.\n");
        return 0;
    }
    
    //Initialize library
    NoteLibrary library(pitchThreshold, dynamicsThreshold);
    
    //Add song to Library
    printf("Analyzing file \"%s\"\n", songDirectory.c_str());
    Song * songFile = new Song(songDirectory);
    if ( songFile -> isInvalid() ) {
        printf("\"%s\" is not a valid wav file.\n", songDirectory.c_str());
        return 0;
    }
    
    long numNotes = library.addSong(songFile);
    double timeInSeconds = (songFile -> getSampleArray() -> size())/(double) (songFile -> getSampleRate());
    double timeInMinutes = timeInSeconds/60.;
    double notesPerMinute = numNotes/timeInMinutes;
    
    if (desiredMinutes <= 0) {
        desiredMinutes = timeInMinutes;
    }
    
    printf("Detected %li notes\n", numNotes);
    printf("Song is %f minutes long\n", timeInMinutes);
    printf("Song has about %i notes per minute\n", (int) notesPerMinute);
    
    
    //Generate (songsToGenerate) pitches and choose the longest one.
    printf("Generating songs ...\n");
    std::vector<RealNote *> writePitches;
    
    int desiredNoteLength = desiredMinutes * notesPerMinute;
    
    for (int i = 0; i < songsToGenerate; i++) {
        std::vector<RealNote *> generatedPitches;
        library.composeProperty(pitchOrder, NoteLibrary::PITCH, &generatedPitches);
        printf("... with %lu notes and %i%% originality.\n", generatedPitches.size(), (int) (library.originality(&generatedPitches)*100));
        
        if (writePitches.empty() or
            abs((int) generatedPitches.size() - desiredNoteLength)
            < abs((int) writePitches.size() - desiredNoteLength)) {
            writePitches = generatedPitches;
        }
    }
    
    printf("Choosing song with %lu notes and %i%% originality.\n", writePitches.size(), (int) (library.originality(&writePitches)*100));
    
    std::map<NoteLibrary::property, std::vector<RealNote *> *> song;
    song[NoteLibrary::PITCH] = &writePitches;
    
    std::vector<RealNote *> dynamics;
    
    if (dynamicsOrder > 0) {
        //Generate dynamics independently
        printf("Generating dynamics...\n");
        library.composeProperty(dynamicsOrder, NoteLibrary::DYNAMICS, &dynamics, writePitches.size());
        song[NoteLibrary::DYNAMICS] = &dynamics;
    }
    
    //Write song to file.
    printf("Writing song to \"%s\".\n", outputDirectory.c_str());

    library.writeToFile(&song, outputDirectory.c_str());
    
}

