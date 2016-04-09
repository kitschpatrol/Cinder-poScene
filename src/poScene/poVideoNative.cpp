#include "poVideoNative.h"

#include "cinder/Log.h"
// TODO split this into two implementations...
// OR just wrap WMF like MovieBase

namespace po {
namespace scene {

VideoNativeRef VideoNative::create() {
	VideoNativeRef ref(new VideoNative());
	ref->setup();
	return ref;
}

void VideoNative::setup() {
}

VideoNative::VideoNative() {
}

VideoNative::~VideoNative() {
}

void VideoNative::update() {
#ifdef CINDER_MSW
	mBackingVideo.update();
	// native loop doesn't work
	// big mess... position never == duration, stops short
	// detect loops
	if (mLoopEnabled && wasPlaying && (lastFrameTime != 0) && (mBackingVideo.getPosition() == 0)) {
		mBackingVideo.setPosition(0);
		mBackingVideo.play();
	}

	lastFrameTime = mBackingVideo.getPosition();
	wasPlaying = mBackingVideo.isPlaying();

#endif

	// Nothing for mac?
}

bool VideoNative::isPlaying() {
#ifdef CINDER_MSW
	return mBackingVideo.isPlaying();
#endif
#ifdef CINDER_MAC

	if (mBackingVideo && mBackingVideo->isLoaded()) {
		return mBackingVideo->isPlaying();
	} else {
		return false;
	}

#endif
}

void VideoNative::draw() {
	ci::gl::ScopedBlendAdditive scopedBlending;
	ci::gl::ScopedColor scopedColor(ci::ColorA(getFillColor(), getAppliedAlpha()));

#ifdef CINDER_MAC
	ci::gl::draw(mBackingVideo->getTexture());
#endif

#ifdef CINDER_MSW
	mBackingVideo.draw(0, 0);
#endif
}

void VideoNative::load(ci::fs::path path) {
#ifdef CINDER_MAC
	mBackingVideo = ci::qtime::MovieGl::create(path);
#endif

#ifdef CINDER_MSW
	mBackingVideo.loadMovie(path);
#endif
}

void VideoNative::play() {

#ifdef CINDER_MAC
	mBackingVideo->play();
#endif

#ifdef CINDER_MSW
	wasPlaying = true;
	mBackingVideo.play();
#endif
}

void VideoNative::pause() {
#ifdef CINDER_MAC
	mBackingVideo->stop();
#endif

#ifdef CINDER_MSW
	wasPlaying = false;
	mBackingVideo.pause();
#endif
}

void VideoNative::stop() {
#ifdef CINDER_MAC
	mBackingVideo->stop();
	mBackingVideo->seekToStart();
#endif

#ifdef CINDER_MSW
	wasPlaying = false;
	mBackingVideo.stop();
#endif
}

void VideoNative::setPlayheadPosition(float value) {
#ifdef CINDER_MAC
	mBackingVideo->seekToTime(value);
#endif

#ifdef CINDER_MSW
	mBackingVideo.setPosition(value);
#endif
}

void VideoNative::setLoopEnabled(bool enabled) {
	if (mLoopEnabled != enabled) {

#ifdef CINDER_MAC
		mBackingVideo->setLoop(enabled);
#endif

#ifdef CINDER_MSW
// mBackingVideo.setLoop(enabled); // currently broken? do it manually
#endif

		mLoopEnabled = enabled;
	}
}

float VideoNative::getDuration() {
#ifdef CINDER_MAC
	return mBackingVideo->getDuration();
#endif

#ifdef CINDER_MSW
	return mBackingVideo.getDuration();
#endif
}

ci::Rectf VideoNative::getBounds() {
#ifdef CINDER_MAC

	if (mBackingVideo) {
		return ci::Rectf(0, 0, mBackingVideo->getWidth(), mBackingVideo->getHeight());
	} else {
		return ci::Rectf::zero();
	}
#endif

#ifdef CINDER_MSW
	return ci::Rectf(0, 0, mBackingVideo.getWidth(), mBackingVideo.getHeight());
#endif
}

} // namespace scene
} // namespace po