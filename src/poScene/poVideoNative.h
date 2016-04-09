/*
 Copyright (c) 2015, Potion Design LLC
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

 * Neither the name of copyright holder nor the names of its
 contributors may be used to endorse or promote products derived from
 this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "cinder/gl/draw.h"

#ifdef CINDER_MSW
#include "ciWMFVideoPlayer.h"
#endif

#ifdef CINDER_MAC
#include "cinder/qtime/QuickTime.h"
#include "cinder/qtime/QuickTimeGlImplAvf.h"

typedef std::shared_ptr<ci::qtime::MovieGl> ciAvfVideoPlayer;
#endif

#include "poNode.h"

namespace po {
namespace scene {

class VideoNative;
typedef std::shared_ptr<VideoNative> VideoNativeRef;

class VideoNative : public Node {
public:
	static VideoNativeRef create();
	void setup();
	void update();
	void draw();

	void load(ci::fs::path path);
	void play();
	void pause();
	void stop();

	bool isPlaying();

	void setPlayheadPosition(float value);
	void setLoopEnabled(bool enabled);
	float getDuration();

	//! Get the bounds of the Image
	ci::Rectf getBounds();
	~VideoNative();

protected:
	VideoNative();

private:
	bool mLoopEnabled;
#ifdef CINDER_MSW
	ciWMFVideoPlayer mBackingVideo;
	bool wasPlaying = false;
	float lastFrameTime = 0;

#endif

#ifdef CINDER_MAC
	ciAvfVideoPlayer mBackingVideo;
#endif
};

} // namespace scene
} // namespace po