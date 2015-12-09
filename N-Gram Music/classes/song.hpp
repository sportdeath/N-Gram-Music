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
    Song(std::string fileName);
    
    //Song(std::vector<Note *> * notes);
    
    void splitIntoNotes(std::vector<Note *> * splitNotes);

};

#endif /* song_hpp */
