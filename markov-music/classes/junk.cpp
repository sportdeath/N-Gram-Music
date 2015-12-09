//
//  junk.cpp
//  N-Gram Music
//
//  Created by tfh on 12/7/15.
//  Copyright © 2015 6.S083. All rights reserved.
//

#include "junk.hpp"


//std::vector<double> Audio::convolve(std::vector<double> a, std::vector<double> b) {
//
//    std::vector<double> c;
//
//    for (long i = 0; i < a.size(); i++) {
//        double sum = 0;
//        for (long j = 0; j < std::min((long) b.size(), i); j++) {
//            sum += a[i - j]*b[j];
//        }
//        c.push_back(sum);
//    }
//
//    return c;
//}
//
//std::vector<double> Audio::normalize(std::vector<double> inputArray) {
//    double max = inputArray[0];
//
//    for ( long i = 0; i < inputArray.size(); i++) {
//        if (inputArray[i] > max) {
//            max = inputArray[i];
//        }
//    }
//
//    std::vector<double> outputArray;
//
//    for ( long i = 0; i < inputArray.size(); i++) {
//        outputArray.push_back( inputArray[i] /max );
//    }
//
//    return outputArray;
//}
//
//std::vector<double> Audio::loPass(std::vector<double> inputArray, int sampleRate, double cutoff) {
//    static int size = 50;
//
//    double omega = cutoff * 2 * M_PI / sampleRate;
//
//    std::vector<double> sinc (2*size +1);
//
//    for (int i = -size; i <= size; i++) {
//        if (i == 0) {
//            sinc[i+size] = omega/M_PI;
//        } else {
//            sinc[i+size] = fabs(sin(omega * i)/(M_PI*i));
//        }
//    }
//
//    return convolve(inputArray, sinc);
//}
//
//std::vector<double> Audio::transientDetection(int windowSize, int hopSize) {
//    std::vector<double> transientArray (sampleArray.size()/hopSize);
//
//    double * fftIn = new double [windowSize];
//    fftw_complex * fftOut = new fftw_complex [windowSize/2 +1];
//    fftw_complex * oldFFT = new fftw_complex [windowSize/2 +1];
//    fftw_plan fftPlan = fftw_plan_dft_r2c_1d(windowSize, fftIn, fftOut, FFTW_ESTIMATE);
//
//    for (int i = 0; i < sampleArray.size() - windowSize; i += hopSize) {
//
//        for (int j = 0; j < windowSize; j++) {
//            fftIn[j] = (sampleArray[i+j]);
//        }
//
//        fftw_execute(fftPlan);
//
//        double transient = 0;
//
//        for (int j = 0; j < windowSize/2 +1; j++) {
//            double abs0XSquared = pow(fftOut[j][0],2) + pow(fftOut[j][1],2);
//            double abs1XSquared = pow(oldFFT[j][0],2) + pow(oldFFT[j][1],2);
//
//            transient += fabs(abs0XSquared - abs1XSquared);
//
//        }
//
//        oldFFT = fftOut;
//
//        transientArray[i/hopSize] = transient;
//    }
//    fftw_free(fftIn);
//    fftw_free(fftOut);
//    fftw_destroy_plan(fftPlan);
//
//
//
//    std::vector<double> filtered = loPass(transientArray, sampleRate/hopSize, 500);
//
//    return normalize(filtered);
//}
//
//std::vector<int> Audio::peakFinding(std::vector<double> transients) {
//    std::vector<int> output;
//    return output;
//}
//
//std::vector<double> Audio::timeStretch(double timeShift, double pitchShift) {
//
//    int size = 2048;
//    int overlaps = 2;
//
//    double * fftIn = new double [size];
//    fftw_complex * fftOut = new fftw_complex[size/2 + 1];
//    fftw_plan plan = fftw_plan_dft_r2c_1d(size, fftIn, fftOut, FFTW_ESTIMATE);
//    double * prevPhase = new double[size/2 +1];
//
//    //for all windows (with overlap 2?)
//    for (int i = 0; i < sampleArray.size()*overlaps/size; i++) {
//
//        for (int j = 0; j < size; j++) {
//            double hann = 0.5 *( 1+ cos(2*M_PI*j/(size -1)) );
//            fftIn[j] = (sampleArray[i + j]) * hann ;
//        }
//
//        fftw_execute(plan);
//
//        for (int j = 0; j < size/2 +1; j++) {
//            double magnitude = sqrt(fftOut[j][0]*fftOut[j][0]+fftOut[j][1]*fftOut[j][1]);
//            double phase = atan2(fftOut[j][1], fftOut[j][0]);
//
//            double unwrappedPhase = phase - prevPhase[j];
//            double phaseDif = unwrappedPhase - j * expected;
//
//
//
//
//            prevPhase[j] = phase;
//        }
//    }
//
//    fftw_destroy_plan(plan);
//
//
//    //0.5(1 - cos(2 pi n/ (size -1)))
//
//    double * fftIn = &sampleArray[0];
//
//
//
//
//
//
//    fftw_destroy_plan(plan);
//
//    sampleArray
//
//    return
//}



//    static std::vector<double> convolve(std::vector<double> a, std::vector<double> b);
//
//    static std::vector<double> normalize(std::vector<double> inputArray);
//    static std::vector<double> loPass(std::vector<double> inputArray, int sampleRate, double cutoff);
//
//    std::vector<double> timeStretch(double timeShift, double pitchShift);
//
//    std::vector<double> transientDetection(int windowSize, int overlaps);
//    static std::vector<int> peakFinding(std::vector<double> transients);


//void NoteLibrary::crossFade(double crossFadeTime, std::vector<RealNote *> * input, std::vector<double> * output, int sampleRate) {
//    int crossFadeSamples = crossFadeTime*sampleRate;
//
//    for (std::vector<RealNote *>::iterator it = input -> begin() ; it != input -> end(); ++it) {
//
//
//        std::vector<double> * noteArray = (* it) -> getSampleArray();
//
//        long crossSize = std::min(crossFadeSamples, (int) std::min(output -> size(), noteArray -> size()));
//
//        for (int i = 0; i < crossSize; i++) {
//            double x = i/(double)crossFadeSamples;
//            (*output)[output -> size() - crossFadeSamples + i] = (*output)[output -> size() - crossFadeSamples + i]*(1-x)+ x*(*noteArray)[i];
//        }
//
//
//        output -> insert(output -> end(), noteArray -> begin() + crossSize, noteArray -> end());
//    }
//}

//int NoteLibrary::downsample(std::vector<RealNote *> * song) {
//    //downsample all songs to lowest sampleRate
//    int minSampleRate = (*song)[0] -> getSampleRate();
//    
//    for (std::vector<RealNote *>::iterator note = song -> begin() ; note != song -> end(); ++note) {
//        
//        if ((* note) -> getSampleRate() < minSampleRate) {
//            minSampleRate = (* note) -> getSampleRate();
//        }
//    }
//    
//    for (std::vector<RealNote *>::iterator note = song -> begin() ; note != song -> end(); ++note) {
//        
//        if ((* note) -> getSampleRate()  > minSampleRate) {
            ///DOWNSAMPLE:
            //            int sampleRate = (* note) -> getSampleRate();
            //            double gcd = std::__gcd(minSampleRate, sampleRate);
            //
            //            std::vector<double> * sampleArray = (* note) -> getSampleArray();
            //
            //            //upsample
            //            std::vector<double> upsampled;
            //            for (int i = 0; i < sampleArray -> size(); i ++) {
            //                upsampled.push_back((*sampleArray)[i]);
            //                for (int j = 1; j < minSampleRate/gcd; j++) {
            //                    upsampled.push_back(0);
            //                }
            //            }
            //
            //            //lopass
            //
            //
            //            //downsample
            //            sampleArray -> clear();
            //
            //            for (int i = 0; i < upsampled.size(); i++) {
            //                if (i % sampleRate/gcd == 0) {
            //                    sampleArray -> push_back(upsampled[i]);
            //                }
            //            }
//        }
//    }
//    
//    return minSampleRate;
//}
//
//void NoteLibrary::pitchShift(std::vector<RealNote *> * song, std::vector<int> shiftAmount) {
//    static int bufferSize = 6720;
//    
//    soundtouch::SoundTouch * pSoundTouch = new soundtouch::SoundTouch();
//    pSoundTouch -> setSampleRate((*song)[0] -> getSampleRate());
//    pSoundTouch -> setChannels(1);
//    pSoundTouch -> setSetting(SETTING_USE_AA_FILTER,0);
//    
//    for (int i = 0; i < song -> size(); i++) {
//        if (not (*song)[i] -> isEndNote()) {
//            RealNote * note = (*song)[i];
//            
//            pSoundTouch -> setPitchOctaves(shiftAmount[i]/(double)Note::spectrogramSize);
//            
//            std::vector<double> * array = note -> getSampleArray();
//            std::vector<double> output;
//            int inSamples;
//            int outSamples;
//            float sampleBuffer [bufferSize];
//            
//            int index = 0;
//            
//            while (index < array -> size() ) {
//                inSamples = std::min(bufferSize, (int) array -> size() - index);
//                
//                for (int i = 0; i < inSamples; i ++) {
//                    sampleBuffer[i] = (*array)[index + i];
//                }
//                
//                pSoundTouch -> putSamples(sampleBuffer, inSamples);
//                
//                index += inSamples;
//                
//                do {
//                    outSamples = pSoundTouch->receiveSamples(sampleBuffer, bufferSize);
//                    output.insert(output.end(), sampleBuffer, sampleBuffer + outSamples);
//                } while (outSamples != 0);
//            }
//            
//            pSoundTouch->flush();
//            do
//            {
//                outSamples = pSoundTouch->receiveSamples(sampleBuffer, bufferSize);
//                for (int i = 0; i < outSamples; i++) {
//                    if (sampleBuffer[i] == 0) {
//                        
//                    }
//                }
//                output.insert(output.end(), sampleBuffer, sampleBuffer + outSamples);
//            } while (outSamples != 0);
//            
//            note -> setSampleArray(output);
//        }
//    }
//}
//
//void NoteLibrary::getShiftVector(std::vector<RealNote *> * song, std::vector<int> * output) {
//    std::vector<int> shiftAmount;
//    
//    shiftAmount.push_back(0);
//    
//    for (int i = 1; i < song -> size(); i++) {
//        int newShift = shiftAmount[i-1] + freqShift[(*song)[i]][(*song)[i-1]];
//        shiftAmount.push_back(newShift % Note::spectrogramSize);
//    }
//    
//    
//    int min = Note::spectrogramSize * Note::spectrogramSize;
//    int minShift = 0;
//    for (int shift = 0; shift < Note::spectrogramSize; shift ++) {
//        int sum;
//        for (int i = 0; i < shiftAmount.size(); i ++) {
//            int iShift = (shiftAmount[i] + shift) % Note::spectrogramSize;
//            int normShift = iShift - Note::spectrogramSize/2;
//            sum += normShift;
//        }
//        
//        if ( abs(sum) < abs(min)) {
//            min = sum;
//            minShift = shift;
//        }
//    }
//    
//    for (int i = 0; i < shiftAmount.size(); i++) {
//        shiftAmount[i] = ((shiftAmount[i] + minShift) % Note::spectrogramSize ) - Note::spectrogramSize/2;
//    }
//}