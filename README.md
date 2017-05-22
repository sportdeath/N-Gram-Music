# N-Gram Music

This is my Final Project for 6.S083: Computation, Cognitive Science, and Language, taught by Professor Robert Berwick. 

The code is constructed to produce an n-gram-inspired improvisation from a source song or series of songs.
To do this, the original audio is chopped up into individual note and chord samples.
Samples with similar harmonic content are clustered together.
Each successive note is randomly chosen from a pool of notes 
whose preceding N notes in the source are harmonically similar to the
last N notes in the generated song. The N-gram order N for the pitch is 2 by default. 
Dynamics are then generated independently of
pitch in order to minizimize sharp uncharachteristic jumps in volume between notes.

## Dependencies

This code requires the 
[FFTW](http://www.fftw.org/)
and 
[libSndFile](http://www.mega-nerd.com/libsndfile/)
C++ libraries. 
On OSX, both can be installed though 
[Homebrew](http://brew.sh/)
with the following commands:

```
$ brew install fftw libsndfile
```

This code is built to include the Complex Onset Detection Function from John Glover's library 
[MODAL](https://github.com/johnglover/modal).

## Usage

The most simple usage of this code is:

```
./n-gram-music inputSongFile.wav
```

This produces a 2-minute "improvisation" in the style of the input song. 
The output is writen to `inputSongFile-n-gram.wav`. 
Three example songs are included in the `testSamples.zip` file. 
Recordings of unaccompanied instruments with percussive attacks, such as piano
and guitar work best in the system, however the system will work on any input.
Full electronic, hip hop, and rock recordings produce extremely amusing results.

Other valid arguments include:

```
./n-gram-music inputSongFile.wav outputSongFile.wav
./n-gram-music inputSongFile.wav outputSongFile.wav pitchOrder dynamicsOrder desiredTime
```
These arguments allow for the user to set a custom output directory, 
and manually set the N-Gram orders for both pitch and dynamic generation 
which are 3 and 4 by default. The desiredTime parameter allows the user to set
the approximate length of the output song in minutes, and is 2 by default.

If the dynamicsOrder is less than or equal to zero dynamics will be disabled (the volume of each note will be exactly the volume in the source).
If the desiredTime is less than or equal to zero the output song will be approximately the same lenth as the input song. 
A complete description of the theory behind the code can be found in the
write-up. 

## Examples

Some of the "music" I made with this code can be found at
[Soundcloud](https://soundcloud.com/user-12541400/).
This program was used to make a variation of each of Bach's
[Goldberg Variations](https://soundcloud.com/user-12541400/sets/goldberg-variations-variations), 
as well as some nice
[impressionist music](https://soundcloud.com/user-12541400/gymnopedie-no1).
