#include "poPangoNode.h"

namespace po {
namespace scene {

PangoNodeRef PangoNode::create() {
	return PangoNode::create(0, 0);
}

PangoNodeRef PangoNode::create(float maxWidth, float maxHeight) {
	PangoNodeRef ref = std::shared_ptr<PangoNode>(new PangoNode());
	ref->setup(maxWidth, maxHeight);
	return ref;
}

PangoNode::PangoNode() {
}

PangoNode::~PangoNode() {
}

void PangoNode::setup(float maxWidth, float maxHeight) {
	mPango = kp::pango::CinderPango::create();
	mPango->setMaxSize(maxWidth, maxHeight);
}

void PangoNode::update() {
	if (mPango->render()) {
		// Update alignment
		this->setAlignment(this->getAlignment());
	}
}

ci::Rectf PangoNode::getBounds() {
	if (mPango->getTexture()) {
		return mPango->getTexture()->getBounds();
	} else {
		return ci::Rectf::zero();
	}
}

void PangoNode::draw() {
	// TODO background size?

	if (mPango->getTexture()) {
		ci::gl::enableAlphaBlendingPremult();
		ci::gl::color(ci::ColorA(getFillColor(), getAppliedAlpha()));
		ci::gl::draw(mPango->getTexture());
	}
}

const kp::pango::CinderPangoRef PangoNode::getPango() {
	return mPango;
}

} // namespace scene
} // namespace po