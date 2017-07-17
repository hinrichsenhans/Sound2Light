// Copyright (c) 2016 Electronic Theatre Controls, Inc., http://www.etcconnect.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef MONOAUDIOBUFFER_H
#define MONOAUDIOBUFFER_H

#include "QCircularBuffer.h"

#include <QVector>


// A class that receives audio samples and buffers these with circular buffering.
class MonoAudioBuffer
{

public:
    enum InputType {
        MONO,
        STEREO,
        LEFT_ONLY,
        RIGHT_ONLY
    };

	explicit MonoAudioBuffer(int capacity);

	// puts samples in the buffer
	// - usually called by an AudioInputInterface object
	void putSamples(QVector<qreal>& data, const int& channelCount);

	// returns the value in the buffer at index i
	const qreal& at(int i) const { return m_buffer[i]; }
    const qreal& at_right(int i) const { return m_buffer_right[i]; }

    // returns the number of samples that have ever been put in the buffer
    int64_t getNumPutSamples() const { return m_numPutSamples; }
    int getCapacity() const { return m_capacity; }

    QString getAudioInputType() const;
    void setAudioInputType(QString type);

protected:
	// Converts PCM data with multiple channels to mono by averaging all channels.
	// Result is saved inplace and data object will be resized.
	void convertToMonoInplace(QVector<qreal>& data, const int& channelCount) const;
    // Selects single channel from PCM data and discards other channels.
    // Result is saved inplace and data object will be resized.
    void chooseSingleChannel(QVector<qreal> &data, const int &channelCount, const int chanIdx) const;
    //Splits buffer into two channels - left and right, if available
    //If single channel, right buffer empty
    //If multi-channel, first two streams are used, and rest are discarded
    void splitBuffer(QVector<qreal> &data, const int &channelCount);

    const int    m_capacity;  // max capacity of the buffer, should be length of FFT
	Qt3DCore::QCircularBuffer<qreal>	m_buffer;  // a circular buffer, removing the oldest elements when inserting new ones
    Qt3DCore::QCircularBuffer<qreal>	m_buffer_right;  // a circular buffer, removing the oldest elements when inserting new ones
    int64_t      m_numPutSamples; // the number of samples that have ever been put into the buffer
    InputType 	m_inputType;
};

#endif // MONOAUDIOBUFFER_H
