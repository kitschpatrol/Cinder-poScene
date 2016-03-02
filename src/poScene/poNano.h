#pragma once

#include "ci_nanovg_gl.hpp"
#include "cinder/gl/draw.h"
#include "poNode.h"

namespace po {
namespace scene {

class Nano;
typedef std::shared_ptr<Nano> NanoRef;

class Nano : public Node {
public:
	//! Create an empty Nano (texture is a null_ptr, won't draw until set)
	static NanoRef create();
	static NanoRef create(float width, float height);

	//! Draw the texture if set
	void draw() override;
	void update() override;

	//! Get the bounds of the Nano
	ci::Rectf getBounds() override;

	void setRenderFunction(std::function<void(ci::nvg::Context &, float, float)> renderFunction);

	Nano();
	virtual ~Nano();

	void createFont(const std::string &name, const ci::fs::path &path);

	void setNeedsRender(bool value);
	void renderWithFunction(const std::function<void(ci::nvg::Context &, float, float)> &renderFunction); // spares subclassing for trivial drawing

protected:
	void setup(float width, float height);
	void render();

	ci::vec2 setupSize;

private:
	ci::gl::TextureRef mTexture;
	ci::gl::FboRef mFbo;
	std::shared_ptr<ci::nvg::Context> mContextRef;

	bool mIsRenderNeeded;
	std::function<void(ci::nvg::Context &, float, float)> mRenderFunction;
};

} // namespace scene
} // namespace po
