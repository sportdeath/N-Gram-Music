//
//  audio.hpp
//  N-Gram Music
//
//  Created by tfh on 11/25/15.
//  Copyright © 2015 6.S083. All rights reserved.
//

#ifndef audio_hpp
#define audio_hpp

#include <stdio.h>
#include <vector>

class Audio {
    /* The sample information of each channel */
    std::vector<std::vector<double>> sampleArrays;
    
    /* The sample rate */
    int sampleRate;
public:
    /* Returns all channels of the song */
    std::vector<std::vector<double>> * getChannels();
    
    /* Returns the leftmost channel of the song */
    std::vector<double> * getSampleArray();
    
    /* Returns the sample rate of the song */
    int getSampleRate();
    
    /* Sets the channel samples */
    void setSampleArray(std::vector<std::vector<double>> array);
    
    /* Sets the sample rate */
    void setSampleRate(int rate);
    

};

#endif /* audio_hpp */
