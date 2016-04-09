#include "poCairo.h"

namespace po {
namespace scene {

Cairo::Cairo() {
}

Cairo::~Cairo() {
}

CairoRef Cairo::create() {
	return Cairo::create(100, 100);
}

CairoRef Cairo::create(float width, float height) {
	CairoRef ref = std::shared_ptr<Cairo>(new Cairo());

	ref->setup(width, height);
	return ref;
}

void Cairo::setup(float width, float height) {
	setupSize = ci::vec2(width, height);
	mIsRenderNeeded = false;

	mSurface = ci::cairo::SurfaceImage(setupSize.x, setupSize.y, true);
	mContext = ci::cairo::Context(mSurface);
	setNeedsRender(true);
}

void Cairo::renderWithFunction(const std::function<void(ci::cairo::Context &, float, float)> &renderFunction) {
	renderFunction(mContext, setupSize.x, setupSize.y);
	mTexture = ci::gl::Texture::create(mSurface.getSurface());
	//	setNeedsRender(true);
}

void Cairo::update() {
	if (mIsRenderNeeded) {
		this->render();
		mTexture = ci::gl::Texture::create(mSurface.getSurface());
		mIsRenderNeeded = false;
	}
}

void Cairo::setNeedsRender(bool value) {
	mIsRenderNeeded = value;
}

void Cairo::draw() {
	if (mTexture) {
		ci::gl::ScopedBlendPremult scopedBlend;
		ci::gl::ScopedColor scopedColor(ci::ColorA(getFillColor(), getAppliedAlpha()));
		ci::gl::draw(mTexture);
	}
}

void Cairo::render() {
	// override this to draw something more interesting...
	//	mContext.setSourceRgba(1.0, 1.0, 1.0, 1.0);
	//	mContext.rectangle(this->getBounds());
	//	mContext.fill();
}

ci::Rectf Cairo::getBounds() {
	if (mTexture) {
		return mTexture->getBounds();
	} else {
		return ci::Rectf::zero();
	}
}

} // namespace scene
} // namespace po