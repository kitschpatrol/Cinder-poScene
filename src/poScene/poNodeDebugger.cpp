// NodeDebugger.cpp
// WorkbenchProjection
//
// Created by Eric Mika on 1/16/16.
//

#include "poNodeDebugger.h"

#include "cinder/Clipboard.h"
#include "cinder/Log.h"
#include "cinder/Utilities.h"

#include "CinderKit.h"
#include "poNodeContainer.h"

#include "ofxRemoteUIServer.h"
#include "poShape.h"

namespace po {
namespace scene {

NodeDebuggerRef NodeDebugger::create(po::scene::SceneRef scene) {
	NodeDebuggerRef ref = NodeDebuggerRef(new NodeDebugger())->shared_from_this();
	ref->setup(scene);
	return ref;

	//	if (mSelectedNode && (mSelectedNode != scene->getRootNode())) {
	//		//		mSelectedNode->setPosition(mUiPositionPadRef->getValue());
	//		//		mSelectedNode->setAlpha(mUiAlphaSliderRef->getValue());
	//		//		mSelectedNode->setScale(mUiScaleSliderRef->getValue());
	//	}

	//
	//
	//	// Pick if different
	//	if (mSelectedNode != nodeUnderPoint) {
	//		mSelectedNode = nodeUnderPoint;
	//		mSelectedNode = scene->getNodeUnderPoint(event.getPos());
	//
	//		CI_LOG_V("Selected Node: " << mSelectedNode);
	//
	//		// mUiPositionPadRef->setValue(mSelectedNode->getPosition());
	//		// mUiScaleSliderRef->setValue(mSelectedNode->getScale().x);
	//		// mUiAlphaSliderRef->setValue(mSelectedNode->getAlpha());
	//	}
	//
}

void NodeDebugger::logChildren(po::scene::NodeRef node, int depth) {
	// Pad the string prefix
	std::string prefix = depth == 0 ? "┌─" : "├─";
	for (int i = 0; i < depth; i++) {
		prefix += "─";
	}

	CI_LOG_V(prefix << node->getName());

	depth++;

	po::scene::NodeContainerRef container = std::dynamic_pointer_cast<po::scene::NodeContainer>(node);
	if (container) {
		for (po::scene::NodeRef &child : container->getChildrenByReference()) {
			logChildren(child, depth);
		}
	}
}

void NodeDebugger::setup(po::scene::SceneRef scene) {
	mScene = scene;
	mFakeNode = po::scene::Shape::create();
	mFakeNode->setName("None");
	this->setSelectedNode(mFakeNode);

	// Mouse Down
	ci::app::getWindow()->getSignalMouseDown().connect([&](ci::app::MouseEvent event) {
		if (event.isRightDown()) {
			po::scene::NodeRef nodeUnderPoint = mScene->getNodeUnderPoint(event.getPos());
			this->setSelectedNode(nodeUnderPoint);

			if (mSelectedNode != mFakeNode) {
				// logNode(mSelectedNode);
				NodeDebugger::logChildren(mSelectedNode);
			}
		}
	});

	// Mouse Drag
	ci::app::getWindow()->getSignalMouseDrag().connect([&](ci::app::MouseEvent event) { //
		if (event.isRightDown()) {
			if (mSelectedNode != mFakeNode) {
				// Find node
				po::scene::NodeRef nodeUnderPoint = mScene->getNodeUnderPoint(event.getPos());

				// Start Dragging
				if (!mIsDragging && (nodeUnderPoint == mSelectedNode)) {
					if (mSelectedNode->hasParent()) {
						mDragStartNodePosition = mSelectedNode->getParent()->localToScene(mSelectedNode->getPosition());
						mDragStartMousePosition = mSelectedNode->windowToScene(event.getPos());
					} else {
						mDragStartNodePosition = mSelectedNode->getPosition();
						mDragStartMousePosition = event.getPos();
					}

					mIsDragging = true;
				}

				// Keep dragging
				if (mIsDragging) {
					ci::vec2 mousePosition = mSelectedNode->windowToScene(event.getPos());
					if (mSelectedNode->hasParent()) {
						mSelectedNode->setPosition(mSelectedNode->getParent()->sceneToLocal((mDragStartNodePosition + mousePosition) - mDragStartMousePosition));
					} else {
						mSelectedNode->setPosition((mDragStartNodePosition + static_cast<ci::vec2>(event.getPos())) - mDragStartMousePosition);
					}
				}
			}
		}
	});

	// Mouse Wheel
	ci::app::getWindow()->getSignalMouseWheel().connect([&](ci::app::MouseEvent event) {
		if (mSelectedNode != mFakeNode) {

			if (event.isShiftDown()) {
				// Ierate through siblings
				if (mSelectedNode->hasSiblings()) {

					int myIndex = mSelectedNode->getParent()->getChildIndex(mSelectedNode);
					int siblingIndex = myIndex + ((event.getWheelIncrement() > 0.0) ? 1 : -1);

					if (siblingIndex < 0) {
						CI_LOG_V("No previous sibling");
					} else if (siblingIndex > mSelectedNode->getParent()->getNumChildren() - 1) {
						CI_LOG_V("No next sibling");
					} else {
						this->setSelectedNode(mSelectedNode->getParent()->getChildByIndex(siblingIndex));
					}

				} else {
					CI_LOG_V("Only child");
				}

			} else {
				// Iterate through parents / children
				if (event.getWheelIncrement() < 0.0) {
					// up
					if (mSelectedNode->hasParent()) {
						this->setSelectedNode(mSelectedNode->getParent());
					} else {
						CI_LOG_V("At top");
					}

				} else {
					po::scene::NodeContainerRef container = std::dynamic_pointer_cast<po::scene::NodeContainer>(mSelectedNode);
					if (container && container->hasChildren()) {
						this->setSelectedNode(container->getFirstChild());
					} else {
						CI_LOG_V("At Bottom");
					}
				}
			}
		}
	});

	ci::app::getWindow()->getSignalMouseUp().connect([&](ci::app::MouseEvent event) { //
		if (mIsDragging) {
			// logNode(mSelectedNode);
			mIsDragging = false;
		}

	});

	ci::app::getWindow()->getSignalKeyDown().connect([&](ci::app::KeyEvent event) {
		switch (event.getCode()) {
			case ci::app::KeyEvent::KEY_ESCAPE:
				this->setSelectedNode(mFakeNode);
				break;
			case ci::app::KeyEvent::KEY_RIGHT:
				mSelectedNode->setX(mSelectedNode->getX() + getNudgeForKeyEvent(event));
				ofxRemoteUIServer::instance()->pushParamsToClient();
				break;
			case ci::app::KeyEvent::KEY_LEFT:
				mSelectedNode->setX(mSelectedNode->getX() - getNudgeForKeyEvent(event));
				ofxRemoteUIServer::instance()->pushParamsToClient();
				break;
			case ci::app::KeyEvent::KEY_UP:
				mSelectedNode->setY(mSelectedNode->getY() - getNudgeForKeyEvent(event));
				ofxRemoteUIServer::instance()->pushParamsToClient();
				break;
			case ci::app::KeyEvent::KEY_DOWN:
				mSelectedNode->setY(mSelectedNode->getY() + getNudgeForKeyEvent(event));
				ofxRemoteUIServer::instance()->pushParamsToClient();
				break;
		}
	});
}

float NodeDebugger::getNudgeForKeyEvent(ci::app::KeyEvent &event) {
	if (event.isShiftDown()) {
		return NUDGE_BIG;
	} else if (event.isAltDown()) {
		return NUDGE_SMALL;
	} else {
		return NUDGE_NORMAL;
	}
}

const po::scene::NodeRef NodeDebugger::getSelectedNode() const {
	return mSelectedNode;
}

void NodeDebugger::removeParamIfExists(std::string paramName) {
	if (ofxRemoteUIServer::instance()->paramExistsForName(paramName)) {
		ofxRemoteUIServer::instance()->removeParamFromDB(paramName);
	}
}

void NodeDebugger::setSelectedNode(po::scene::NodeRef node) {

	if (node == nullptr) {
		// Map null to fake...
		CI_LOG_V("trying again rather than set node to " << node->getName());
		setSelectedNode(mFakeNode); // try again
		return;
	} else if (false && (node == mScene->getRootNode())) {
		// Map root to its first child... can't manipulate root
		if (mScene->getRootNode()->hasChildren()) {
			setSelectedNode(mScene->getRootNode()->getChildByIndex(0));
			return;
		} else {
			setSelectedNode(mFakeNode); // try again
			return;
		}
	}

	if (mSelectedNode != node) {
		// Clean up the old
		if (mSelectedNode != nullptr && mSelectedNode != mFakeNode) {
			mSelectedNode->setDrawBounds(false);
			mSelectedNode->setIsSelectedByDebugger(false);
		}

		removeParamIfExists("Node Debugger");
		removeParamIfExists("Node Name");
		removeParamIfExists("Node Position X");
		removeParamIfExists("Node Position Y");
		removeParamIfExists("Node Offset X");
		removeParamIfExists("Node Offset Y");
		removeParamIfExists("Node Rotation");
		removeParamIfExists("Node Scale");
		removeParamIfExists("Node Scale X");
		removeParamIfExists("Node Scale Y");
		removeParamIfExists("Node Log");

		// Set up the new
		mSelectedNode = node;

		CI_LOG_V("Selected node: " << mSelectedNode->getName());

		if (mSelectedNode != mFakeNode) {
			mSelectedNode->setDrawBounds(true);
			mSelectedNode->setIsSelectedByDebugger(true);
			mSelectedNode->setBoundsColor(ci::Color("yellow"));
		}

		RUI_NEW_GROUP("Node Debugger");

		mNameToShare = mSelectedNode->getName();

		RUI_SHARE_PARAM_WCN("Node Name", mNameToShare);
		RUI_SHARE_GS_PARAM_WCN("Node Position X",																																				//
													 [&]() -> float { return mSelectedNode->getPosition().x; },																//
													 [&](float value) { mSelectedNode->setPosition(value, mSelectedNode->getPosition().y); }, //
													 -ci::app::getWindowWidth(), 2 * ci::app::getWindowWidth());
		RUI_SHARE_GS_PARAM_WCN("Node Position Y",																																				//
													 [&]() -> float { return mSelectedNode->getPosition().y; },																//
													 [&](float value) { mSelectedNode->setPosition(mSelectedNode->getPosition().x, value); }, //
													 -ci::app::getWindowHeight(), 2 * ci::app::getWindowHeight());
		RUI_SHARE_GS_PARAM_WCN("Node Offset X",																																													//
													 [&]() -> float { return mSelectedNode->getOffsetNormalized().x; },																				//
													 [&](float value) { mSelectedNode->setOffsetNormalized(value, mSelectedNode->getOffsetNormalized().y); }, //
													 -1.0, 1.0);
		RUI_SHARE_GS_PARAM_WCN("Node Offset Y",																																													//
													 [&]() -> float { return mSelectedNode->getOffsetNormalized().y; },																				//
													 [&](float value) { mSelectedNode->setOffsetNormalized(mSelectedNode->getOffsetNormalized().x, value); }, //
													 -1.0, 1.0);
		RUI_SHARE_GS_PARAM_WCN("Node Rotation",																					//
													 [&]() -> float { return mSelectedNode->getRotation(); }, //
													 [&](float value) { mSelectedNode->setRotation(value); }, //
													 -M_PI, M_PI);

		const float scaleMin = 0.0;
		const float scaleMax = 2.0;
		RUI_SHARE_GS_PARAM_WCN("Node Scale",																					 //
													 [&]() -> float { return mSelectedNode->getScale().x; }, //
													 [&](float value) { mSelectedNode->setScale(value); },	 //
													 scaleMin, scaleMax);
		RUI_SHARE_GS_PARAM_WCN("Node Scale X",																				//
													 [&]() -> float { return mSelectedNode->getScaleX(); }, //
													 [&](float value) { mSelectedNode->setScaleX(value); }, //
													 scaleMin, scaleMax);
		RUI_SHARE_GS_PARAM_WCN("Node Scale Y",																				//
													 [&]() -> float { return mSelectedNode->getScaleY(); }, //
													 [&](float value) { mSelectedNode->setScaleY(value); }, //
													 scaleMin, scaleMax);
		RUI_SHARE_GS_PARAM_WCN("Node Log",											//
													 [&]() -> bool { return false; }, //
													 [&](bool value) {
														 this->logNode(mSelectedNode);
														 ofxRemoteUIServer::instance()->pushParamsToClient();
													 });

		// Regardless, feed RUI

		ofxRemoteUIServer::instance()->pushParamsToClient();

		// tell the node
	}
}

void NodeDebugger::logNode(po::scene::NodeRef node) {
	std::string name = "Node Name: " + node->getName();

	std::string position = "someNode->setPosition(" + kp::kit::toCodeString(node->getPosition()) + ");";
	std::string scale = "someNode->setScale(" + ci::toString(node->getScale().x) + ");";
	std::string alpha = "someNode->setAlpha(" + ci::toString(node->getAlpha()) + ");";
	std::string rotation = "someNode->setRotation(" + ci::toString(node->getRotation()) + ");";
	std::string bounds = "Bounds: " + kp::kit::toCodeString(node->getBounds());

	std::string rectInParent = "Rect in parent: ";
	if (node->hasParent()) {
		ci::vec2 ul = node->localToNode(node->getBounds().getUpperLeft(), node->getParent());
		ci::vec2 lr = node->localToNode(node->getBounds().getLowerRight(), node->getParent());

		rectInParent += kp::kit::toCodeString(ci::Rectf(ul, lr));
		;
	} else {
		rectInParent += "No Parent.";
	}

	std::string info =
			"-------\n" + name + "\n\n" + position + "\n" + scale + "\n" + rotation + "\n" + alpha + "\n\n" + bounds + "\n" + rectInParent + "\n-------\n";

	ci::Clipboard::setString(info);
	std::cout << info << std::endl;

	std::cout << kp::kit::toCodeString(node->getPosition()) << ", " << kp::kit::toCodeString(node->getScale()) << ", " << node->getRotation() << "),"
						<< std::endl;
}

NodeDebugger::NodeDebugger() {
	CI_LOG_V("NodeDebugger Created");
}

NodeDebugger::~NodeDebugger() {
	CI_LOG_V("NodeDebugger Destroyed");
}

} // namespace scene
} // namespace po