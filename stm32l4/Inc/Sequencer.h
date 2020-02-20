#ifndef __SEQU_H
#define __SEQU_H

#include "stm32l4xx_hal.h"
#include <stdbool.h>
//#include "AudioEffects.h"

#define NUMHALFBEATSPERLOOP 32 //number of half beats per loop
#define NUMINSTRUMENTS 5 //number of sequencer channels
#define NUMEFFECTSPERCHANNEL 5 //max number of effects that can be added to a channel

//forward declaration here because I want to store a pointer to sequencer channel in audio effect and vice versa 
typedef struct SequencerChannel SequencerChannel;
typedef struct AudioEffectFloat AudioEffectFloat; //implementation in AudioEffects.h
typedef struct PingPongBuff PingPongBuff; //implementation in AudioEffects.h

//structure which holds audio effects that affect all SequencerChannels
typedef struct Sequencer
{
	uint32_t numActiveAudioEffects; //number of active audio effects
	AudioEffectFloat * audioEffects[NUMEFFECTSPERCHANNEL]; //array of active audio effect structures of size numActiveAudioEffects
}Sequencer;


struct SequencerChannel
{
	bool step[NUMHALFBEATSPERLOOP]; //true if instrument plays at given half note id and false otherwise
	bool stepTemp[NUMHALFBEATSPERLOOP]; //stores the steps temporarily when cap is tapped until it becomes active next time around the loop
	uint32_t instrumentID; 
	volatile uint32_t numSamples; //number of 16 bit samples in the sample
	volatile uint32_t sampleIndex; //index of sample being played
	volatile float sampleIndexFloat; //index of sample being played float version for slow frequency controllers
	int16_t * addressAudio; //stores the address of the sound buffer to be played
	float frequencyController; //by how much does the sampleIndex increment
	int16_t previousAudioSample; //stores the previous valid audio sample in case structure is locked
	int32_t volume; //volume from 0->100
	int32_t volumeLive; //volume used for live playback
	bool live; //true if the sound being played came from a live tap
	volatile bool playback; //is the intrument currently playing
	bool lock; //semaphore
	bool empty; //if the step array is empty will be true
	volatile bool pingPongBuffUsed; //flag which is true if the samples are being grabbed from the ping pong buffer or from addressAudio
	uint32_t numActiveAudioEffects; //number of active audio effects
	AudioEffectFloat * audioEffects[NUMEFFECTSPERCHANNEL]; //array of active audio effect structures of size numActiveAudioEffects
	PingPongBuff * ppBuff; //pointer to the last ping pong buffer	in the chain
};

typedef struct
{
	uint32_t numSamples; //number of 16 bit samples in the sample
	uint32_t sampleIndex; //index of sample being played
	float sampleIndexFloat; //index of sample being played float version for slow frequency controllers
	PingPongBuff * ppBuff; //pointer to ping pong buffer
	float frequencyController; //by how much does the sampleIndex increment
	int16_t previousAudioSample; //stores the previous valid audio sample in case structure is locked
	int32_t volume; //volume from 0->100
	bool playback; //is the intrument currently playing
	bool lock; //semaphore
} FftTestChannel;

//bool initFFTChannel(void);

bool initSequencerChannel(uint8_t sequencerChannelIndex,uint32_t instID);
bool initMetronomeChannel(void);
void setMetronomeVolumeBasedOnSequencerFillAmount(void);
bool clearSequencerChannel(uint8_t sequencerChannelIndex);
bool isSequencerChannelEmpty(uint8_t sequencerChannelIndex);
bool areAllSequencerChannelEmpty(void);
uint32_t getFirstStepIndex(void);
uint32_t getSequencerChannelInstrumentID(uint8_t sequencerChannelIndex);
bool setSequencerChannelInstrumentID(uint8_t sequencerChannelIndex, uint32_t instID);
bool setSequencerChannelStep(uint8_t sequencerChannelIndex, uint32_t stepIndex);
bool getSequencerChannelStep(uint8_t sequencerChannelIndex, uint32_t stepIndex);
void updateMetronomeTime(uint32_t timeMS);
uint32_t getMetronomeTimeMS(void);
uint32_t getLooptime(void);
uint8_t getHalfBeatIndex(void);
uint32_t incrementHalfBeatIndex(void);
void startMetronome(void);
void stopMetronome(void);
uint32_t getStepIndexFromLoopTime(uint32_t loopTime, bool *early);
uint8_t getSequencerChannelAssociatedToFace(uint8_t faceUp);
void setEarlyTapPlayNextTick(uint8_t sequencerChIndex);
bool getEarlyTapPlayNextTick(uint8_t sequencerChIndex);


void recordAudioSample(uint32_t sample);
bool startRecording(uint8_t seqChIndex);
bool getRecordingSide(uint8_t seqChIndex);
void clearRecordingSide(uint8_t seqChIndex);
bool isRecordingComplete(void);
bool stopRecording(void);
void stopRecordingSignal(void);

bool setSequencerChannelVoiceRecording(uint8_t sequencerChannelIndex);
//void recordAudio2SPIFlash(uint8_t sequencerChannelIndex); //old
void recordAudio2SPIFlashMain(void);

uint16_t getNextSample(void);
void startPlayback(uint32_t channelIndex, bool live);
int8_t isVoicePlaying(void);
void setVolume(uint32_t channelIndex, int32_t volume);
int32_t getVolume(uint32_t channelIndex);

void setMasterVolume(int32_t volume);
int32_t getMasterVolume(void);

void initAllAudioEffects(void);
void appendEffectToChannel(AudioEffectFloat* audEff, SequencerChannel* seqCh);
void removeEffectFromChannel(SequencerChannel* seqCh);
void removeEffectFromChannelId(uint8_t seqChId);
void appendLPF(uint8_t sequencerChannelIndex);
void removeLPF(uint8_t sequencerChannelIndex);

void appendPitchShiftEffect(uint8_t sequencerChannelIndex);

bool activateCircleScratch(void);
bool deactivateCircleScratch(void);

bool activateDelay(void);
bool deactivateDelay(void);

void executeAllEffectsMain(void);


#endif
