#pragma once

#include "cinder/cairo/Cairo.h"
#include "cinder/gl/draw.h"
#include "poNode.h"

namespace po {
namespace scene {

class CairoNode;
typedef std::shared_ptr<CairoNode> CairoNodeRef;

class CairoNode : public Node {
public:
	//! Create an empty CairoNode (texture is a null_ptr, won't draw until set)
	static CairoNodeRef create();
	static CairoNodeRef create(float width, float height);

	//! Draw the texture if set
	void draw() override;
	void update() override;

	//! Get the bounds of the CairoNode
	ci::Rectf getBounds() override;

	CairoNode();
	virtual ~CairoNode();

	void setNeedsRender(bool value);
	void renderWithFunction(const std::function<void(ci::cairo::Context &, float, float)> &renderFunction); // spares subclassing for trivial drawing

protected:
	void setup(float width, float height);
	void render();

	ci::vec2 setupSize;

private:
	ci::cairo::SurfaceImage mSurface;
	ci::cairo::Context mContext;
	ci::gl::TextureRef mTexture;

	bool mIsRenderNeeded;
};

} // namespace scene
} // namespace po
