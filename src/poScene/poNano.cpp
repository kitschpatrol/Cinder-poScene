#include "poNano.h"

#include "cinder/gl/scoped.h" 

namespace po {
namespace scene {

Nano::Nano() {
}

Nano::~Nano() {
}

NanoRef Nano::create() {
	return Nano::create(100, 100);
}

NanoRef Nano::create(float width, float height) {
	NanoRef ref = std::shared_ptr<Nano>(new Nano());

	ref->setup(width, height);
	return ref;
}

void Nano::createFont(const std::string &name, const ci::fs::path &path) {
	mContextRef->createFont(name, path.string());
}

void Nano::setup(float width, float height) {
	setupSize = ci::vec2(width, height);
	mIsRenderNeeded = false;

	// mip map...
	ci::gl::Texture::Format textureFormat;
	//	textureFormat.setMinFilter(GL_LINEAR_MIPMAP_NEAREST);
	//	textureFormat.setMagFilter(GL_LINEAR);
	//	textureFormat.enableMipmapping(true);

	ci::gl::Fbo::Format fboFormat;
	fboFormat.enableStencilBuffer();
	fboFormat.setColorTextureFormat(textureFormat);

	mFbo = ci::gl::Fbo::create(width, height, fboFormat);
	mContextRef = std::make_shared<ci::nvg::Context>(ci::nvg::createContextGL());
	setNeedsRender(true);
}

void Nano::update() {
	if (mIsRenderNeeded) {
		if (mRenderFunction) {
			auto &vg = *mContextRef;

			ci::gl::ScopedFramebuffer fboScope(mFbo);
			ci::gl::ScopedViewport fboViewPort(mFbo->getSize());
			ci::gl::clear(ci::ColorAf::zero());
			ci::gl::clear(GL_STENCIL_BUFFER_BIT);

			vg.beginFrame(mFbo->getSize(), 1.0); // TODO HIDPI
			mRenderFunction(vg, mFbo->getWidth(), mFbo->getHeight());
			vg.endFrame();

			mTexture = mFbo->getColorTexture();
		} else {
			this->render();
		}
		mIsRenderNeeded = false;
	}
}

void Nano::setNeedsRender(bool value) {
	mIsRenderNeeded = value;
}

void Nano::draw() {
	if (mTexture) {
		ci::gl::ScopedBlendAlpha scopedAlpha;
		ci::gl::ScopedColor scopedColor(ci::ColorA(getFillColor(), getAppliedAlpha()));
		ci::gl::draw(mTexture);
	}
}

void Nano::render() {
	// override this to draw something more interesting...
	//	mContext.setSourceRgba(1.0, 1.0, 1.0, 1.0);
	//	mContext.rectangle(this->getBounds());
	//	mContext.fill();
}

ci::Rectf Nano::getBounds() {
	if (mTexture) {
		return mTexture->getBounds();
	} else {
		return ci::Rectf::zero();
	}
}

void Nano::setRenderFunction(std::function<void(ci::nvg::Context &, float, float)> renderFunction) {
	mRenderFunction = renderFunction;
	mIsRenderNeeded = true;
}

} // namespace scene
} // namespace po