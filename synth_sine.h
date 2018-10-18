/* Audio Library for Teensy 3.X
 * Copyright (c) 2014, Paul Stoffregen, paul@pjrc.com
 *
 * Development of this audio library was funded by PJRC.COM, LLC by sales of
 * Teensy and Audio Adaptor boards.  Please support PJRC's efforts to develop
 * open source software by purchasing Teensy or other PJRC products.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice, development funding notice, and this permission
 * notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef synth_sine_h_
#define synth_sine_h_

#include "Arduino.h"
#include "AudioStream.h"

class AudioSynthWaveformSine : public AudioStream
{
//GUI: inputs:0, outputs:1 //this line used for automatic generation of GUI node
//GUI: shortName:sine  //this line used for automatic generation of GUI node
public:
	AudioSynthWaveformSine() : AudioStream(0, NULL, "AudioSynthWaveformSine"), magnitude(1.0) { } //uses default AUDIO_SAMPLE_RATE from AudioStream.h
	void frequency(float freq) {
		if (freq < 0.0) freq = 0.0;
		else if (freq > AUDIO_SAMPLE_RATE_EXACT/2.f) freq = AUDIO_SAMPLE_RATE_EXACT/2.f;
		phase_increment = freq * (256.0 / AUDIO_SAMPLE_RATE_EXACT);
	}
	void phase(float angle) {
		if (angle < 0.0f) angle = 0.0f;
		else if (angle > 360.0f) {
			angle = angle - 360.0f;
			if (angle >= 360.0f) return;
		}
		phase_accumulator = angle * (256.0 / 360.0f);
	}
	void amplitude(float n) {
		if (n < 0) n = 0;
		else if (n > 1.0f) n = 1.0f;
		magnitude = n;
	}
	virtual void update(void);
	
private:
	float phase_accumulator;
	float phase_increment;
	float magnitude;
};

class AudioSynthWaveformSineModulated : public AudioStream
{
public:
	AudioSynthWaveformSineModulated() : AudioStream(1, inputQueueArray, "AudioSynthWaveformSineModulated"), magnitude(1.0) {}
	// maximum unmodulated carrier frequency is 11025 Hz
	// input = +1.0 doubles carrier
	// input = -1.0 DC output
	void frequency(float freq) {
		if (freq < 0.0) freq = 0.0;
		else if (freq > AUDIO_SAMPLE_RATE_EXACT/4) freq = AUDIO_SAMPLE_RATE_EXACT/4;
		phase_increment = freq * (4294967296.0 / AUDIO_SAMPLE_RATE_EXACT);
	}
	void phase(float angle) {
		if (angle < 0.0) angle = 0.0;
		else if (angle > 360.0) {
			angle = angle - 360.0;
			if (angle >= 360.0) return;
		}
		phase_accumulator = angle * (4294967296.0 / 360.0);
	}
	void amplitude(float n) {
		if (n < 0) n = 0;
		else if (n > 1.0) n = 1.0;
		magnitude = n * 65536.0;
	}
	virtual void update(void);
private:
	uint32_t phase_accumulator;
	uint32_t phase_increment;
	audio_block_t *inputQueueArray[1];
	float magnitude;
};

#endif