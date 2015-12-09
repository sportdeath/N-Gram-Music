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
    std::vector<std::vector<double>> sampleArrays;
    int sampleRate;
public:
    std::vector<std::vector<double>> * getChannels();
    std::vector<double> * getSampleArray();
    int getSampleRate();
    
    void setSampleArray(std::vector<std::vector<double>> array);
    void setSampleRate(int rate);
    
    void detectOnsets(std::vector<int> * );
    static void plotOnsets(std::vector<double> * sampleArray, double * odfValues, std::vector<int> * onsets);
};

#endif /* audio_hpp */