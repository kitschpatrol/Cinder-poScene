#pragma once

#include "CinderPango.h"
#include "poNode.h"

namespace po {
namespace scene {

class Pango;
typedef std::shared_ptr<Pango> PangoRef;

class Pango : public Node {
public:
	static PangoRef create();
	static PangoRef create(float maxWidth, float maxHeight);

	virtual ~Pango();

	const kp::pango::CinderPangoRef getPango();
	virtual ci::Rectf getBounds() override;

protected:
	void setup(float maxWidth, float maxHeight);
	Pango();
	void update() override;
	void draw() override;

private:
	kp::pango::CinderPangoRef mPango;
	void render();
};

} // namespace scene
} // namespace po