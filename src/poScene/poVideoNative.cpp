#include "poVideoNative.h"

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
	mBackingVideo.update();

	if (mLoopEnabled && (mBackingVideo.getPosition() >= mBackingVideo.getDuration())) {
		mBackingVideo.setPosition(0);
	}
}

void VideoNative::draw() {
	ci::gl::enableAlphaBlending();
	ci::gl::color(ci::ColorA(getFillColor(), getAppliedAlpha()));
	mBackingVideo.draw(0, 0);
}

void VideoNative::load(ci::fs::path path)
{
	mBackingVideo.loadMovie(path);
}

void VideoNative::play()
{
	mBackingVideo.play();
}

void VideoNative::pause() {
	mBackingVideo.pause();
}

void VideoNative::stop() {
	mBackingVideo.stop();
}

void VideoNative::setPlayheadPosition(float value) {
	mBackingVideo.setPosition(value);
}

void VideoNative::setLoopEnabled(bool enabled) {
	if (mLoopEnabled != enabled) {mBackingVideo.setLoop(enabled);
		// mBackingVideo.setLoop(enabled); // currently broken, do it manually
		mLoopEnabled = enabled;
	}
	
}

float VideoNative::getDuration()
{
	return mBackingVideo.getDuration();
}

ci::Rectf VideoNative::getBounds() {
	return ci::Rectf(0, 0, mBackingVideo.getWidth(), mBackingVideo.getHeight());
}

} // namespace scene
} // namespace po