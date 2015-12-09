//
//  main.cpp
//  N-Gram Music
//
//  Created by tfh on 11/20/15.
//  Copyright (c) 2015 6.S083. All rights reserved.
//

#include <iostream>
#include <dirent.h>
#include <fstream>
#include <string>
#include "noteLibrary.hpp"

/*
 given random starting note

 Takes Sound file inputs and returns its left channel as an array of doubles.
 The array has file.frames() number of samples.
 */

int main() {
    
    double pitchThreshold = 0.8;
    double dynamicThreshold = 0.8;
    int order = 3;
    
    //const char * libraryDirectory = "/Users/tfh/Dropbox (Personal)/6.S083/FinalProject/N-Gram Music/Glenn Gould - Bach J.S. - Goldberg Variations/";
    
    const char * songDirectory = "/Users/tfh/Dropbox (Personal)/6.S083/FinalProject/N-Gram Music/Glenn Gould - Bach J.S. - Goldberg Variations/24 - Glenn Gould - Variation 23.wav";
    
    //const char * libraryDirectory = "/Users/tfh/Dropbox (MIT)/iTunes/iTunes Media/Music/Ifan Dafydd/Treehouse _ To Me/";
    
    //const char * libraryDirectory = "/Users/tfh/Dropbox (Personal)/6.S083/FinalProject/N-Gram Music/A Charlie Brown Christmas/";
    
    const char * outputFile = "/Users/tfh/Dropbox (Personal)/6.S083/FinalProject/N-Gram Music/matt3.wav";
    
    NoteLibrary * library = new NoteLibrary(pitchThreshold, dynamicThreshold);
    
//    DIR * dir;
//    struct dirent *de;
//    std::vector<char *> listOfFileNames;
    
//    dir = opendir(libraryDirectory);
    
//    while(dir) {
//        de = readdir(dir);
//        if (!de) break;
//        std::string name = de -> d_name;
//        if ( name.size() > 4 && name.substr(name.size() - 4) == ".wav") {
//            printf("Adding file \"%s\"\n", de->d_name);
//            Song * song = new Song(libraryDirectory + name);
//            library -> addSong(song);
//            break;
//        }
//    }
//
//    closedir(dir);
    
    printf("Adding file \"%s\"\n", songDirectory);
    Song * songFile = new Song(songDirectory);
    library -> addSong(songFile);
    
    printf("Generating song...\n");
    
    
    std::vector<RealNote *> writePitches;
    
    for (int i = 0; i < 10; i++) {
        std::vector<RealNote *> generatedPitches;
        library -> composeProperty(order, NoteLibrary::PITCH, &generatedPitches);
        printf("... with %lu notes.\n", generatedPitches.size());
        if (writePitches.empty() or generatedPitches.size() > writePitches.size()) {
            writePitches = generatedPitches;
        }
    }
    
    printf("Choosing song with %lu notes.\n", writePitches.size());
    
    printf("Generating dynamics...\n");
    std::vector<RealNote *> dynamics;
    library -> composeProperty(order, NoteLibrary::DYNAMICS, &dynamics, writePitches.size());
    
    printf("Writing song to %s.\n", outputFile);
    std::map<NoteLibrary::property, std::vector<RealNote *> *> song;
    song[NoteLibrary::PITCH] = &writePitches;
    song[NoteLibrary::DYNAMICS] = &dynamics;
    library ->writeToFile(&song, outputFile);
    
    return 0;
}

