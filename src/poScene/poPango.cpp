#include "poPango.h"

namespace po {
namespace scene {

PangoRef Pango::create() {
	return Pango::create(0, 0);
}

PangoRef Pango::create(float maxWidth, float maxHeight) {
	PangoRef ref = std::shared_ptr<Pango>(new Pango());
	ref->setup(maxWidth, maxHeight);
	return ref;
}

Pango::Pango() {
}

Pango::~Pango() {
}

void Pango::setup(float maxWidth, float maxHeight) {
	mPango = kp::pango::CinderPango::create();
	mPango->setMaxSize(maxWidth, maxHeight);
}

void Pango::update() {
	if (mPango->render()) {
		// Update alignment
		this->setAlignment(this->getAlignment());
	}
}

ci::Rectf Pango::getBounds() {
	if (mPango->getTexture()) {
		return mPango->getTexture()->getBounds();
	} else {
		return ci::Rectf::zero();
	}
}

void Pango::draw() {
	// TODO background size?

	if (mPango->getTexture()) {
		ci::gl::ScopedBlendPremult scopedBlend;
		ci::gl::ScopedColor scopedColor(ci::ColorA(getFillColor(), getAppliedAlpha()));
		ci::gl::draw(mPango->getTexture());
	}
}

const kp::pango::CinderPangoRef Pango::getPango() {
	return mPango;
}

} // namespace scene
} // namespace po