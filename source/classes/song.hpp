//
//  song.hpp
//  N-Gram Music
//
//  Created by tfh on 11/25/15.
//  Copyright © 2015 6.S083. All rights reserved.
//

#ifndef song_hpp
#define song_hpp

#include <stdio.h>
#include <vector>
#include "audio.hpp"

class Note;

class Song : public Audio {
public:
    /*
     Creates a song from a wav file
     */
    Song(std::string fileName);
    
    /*
     Returns true if the song has been created with a wav file that is invalid
     */
    bool isInvalid();
    
    /*
     Creates an array of integers representing the samples in a song
     where there are note onsets.
     */
    void detectOnsets(std::vector<int> * onsets);
    
    /*
     Splits a song into individual notes at each onset
     */
    void splitIntoNotes(std::vector<Note *> * splitNotes);

    /*
     Creates three output files for plotting with gnuplot.
     
     Use command:
     "plot 'audio' w l, 'transients' w l, 'onsets' w p"
     */
    static void plotOnsets(std::vector<double> * sampleArray, double * odfValues, std::vector<int> * onsets);
};

#endif /* song_hpp */
