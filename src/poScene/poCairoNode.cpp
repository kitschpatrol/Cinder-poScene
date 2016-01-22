#include "poCairoNode.h"

namespace po {
namespace scene {

CairoNode::CairoNode() {
}

CairoNode::~CairoNode() {
}

CairoNodeRef CairoNode::create() {
	return CairoNode::create(100, 100);
}

CairoNodeRef CairoNode::create(float width, float height) {
	CairoNodeRef ref = std::shared_ptr<CairoNode>(new CairoNode());

	ref->setup(width, height);
	return ref;
}

void CairoNode::setup(float width, float height) {
	setupSize = ci::vec2(width, height);
	mIsRenderNeeded = false;

	mSurface = ci::cairo::SurfaceImage(setupSize.x, setupSize.y, true);
	mContext = ci::cairo::Context(mSurface);
	setNeedsRender(true);
}

void CairoNode::renderWithFunction(const std::function<void(ci::cairo::Context &, float, float)> &renderFunction) {
	renderFunction(mContext, setupSize.x, setupSize.y);
	mTexture = ci::gl::Texture::create(mSurface.getSurface());
	//	setNeedsRender(true);
}

void CairoNode::update() {
	if (mIsRenderNeeded) {
		this->render();
		mTexture = ci::gl::Texture::create(mSurface.getSurface());
		mIsRenderNeeded = false;
	}
}

void CairoNode::setNeedsRender(bool value) {
	mIsRenderNeeded = value;
}

void CairoNode::draw() {
	if (mTexture) {
		ci::gl::enableAlphaBlendingPremult();
		ci::gl::color(ci::ColorA(getFillColor(), getAppliedAlpha()));
		ci::gl::draw(mTexture);
	}
}

void CairoNode::render() {
	// override this to draw something more interesting...
	//	mContext.setSourceRgba(1.0, 1.0, 1.0, 1.0);
	//	mContext.rectangle(this->getBounds());
	//	mContext.fill();
}

ci::Rectf CairoNode::getBounds() {
	if (mTexture) {
		return mTexture->getBounds();
	} else {
		return ci::Rectf::zero();
	}
}

} // namespace scene
} // namespace po