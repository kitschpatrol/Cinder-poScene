#pragma once

#include "CinderPango.h"
#include "poNode.h"

namespace po {
namespace scene {

class PangoNode;
typedef std::shared_ptr<PangoNode> PangoNodeRef;

class PangoNode : public Node {
public:
	static PangoNodeRef create();
	static PangoNodeRef create(float maxWidth, float maxHeight);

	virtual ~PangoNode();

	const kp::pango::CinderPangoRef getPango();
	virtual ci::Rectf getBounds() override;

protected:
	void setup(float maxWidth, float maxHeight);
	PangoNode();
	void update() override;
	void draw() override;

private:
	kp::pango::CinderPangoRef mPango;
	void render();
};

} // namespace scene
} // namespace po