// NodeDebugger.h
// WorkbenchProjection
//
// Created by Eric Mika on 1/16/16.
//

#pragma once

#include "cinder/Cinder.h"
#include "cinder/app/App.h"

#include "poNode.h"
#include "poScene.h"

namespace po {
namespace scene {

typedef std::shared_ptr<class NodeDebugger> NodeDebuggerRef;

class NodeDebugger : public std::enable_shared_from_this<NodeDebugger> {
public:
	static NodeDebuggerRef create(po::scene::SceneRef scene);
	~NodeDebugger();

	const po::scene::NodeRef getSelectedNode() const;
	void setSelectedNode(po::scene::NodeRef node); // usually called internally

protected:
	NodeDebugger();

private:
	void setup(po::scene::SceneRef scene);
	po::scene::NodeRef mSelectedNode = nullptr;
	po::scene::NodeRef mFakeNode = nullptr; // "fake" null node to receive RUI messages when nothing is selected

	po::scene::SceneRef mScene = nullptr; // target scene
	void removeParamIfExists(std::string paramName);

	const float NUDGE_SMALL = 0.1f;
	const float NUDGE_NORMAL = 1.0f;
	const float NUDGE_BIG = 10.0f;

	float getNudgeForKeyEvent(ci::app::KeyEvent &event);

	std::string mNameToShare;
};

} // namespace scene
} // namespace po