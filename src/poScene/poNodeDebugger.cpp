// NodeDebugger.cpp
// WorkbenchProjection
//
// Created by Eric Mika on 1/16/16.
//

#include "poNodeDebugger.h"

#include "cinder/Log.h"

#include "ofxRemoteUIServer.h"
#include "poShape.h"

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
	//	//		// Print info no matter what
	//	//		std::string position = "someNode->setPosition(" + kp::kit::toCodeString(mSelectedNode->getPosition()) + ");";
	//	//		std::string scale = "someNode->setScale(" + ci::toString(mSelectedNode->getScale().x) + ");";
	//	//		std::string alpha = "someNode->setAlpha(" + ci::toString(mSelectedNode->getAlpha()) + ");";
	//	//		std::string info = position + "\n" + scale + "\n" + alpha;
	//	//		ci::Clipboard::setString(info);
	//	//		CI_LOG_V("\n" + info);
}

void NodeDebugger::setup(po::scene::SceneRef scene) {
	mScene = scene;
	mFakeNode = po::scene::Shape::create();
	mFakeNode->setName("None");
	this->setSelectedNode(mFakeNode);

	ci::app::getWindow()->getSignalMouseDown().connect([&](ci::app::MouseEvent event) {
		if (event.isRightDown()) {
			po::scene::NodeRef nodeUnderPoint = mScene->getNodeUnderPoint(event.getPos());
			this->setSelectedNode(nodeUnderPoint);
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
	// Map null to fake...
	if ((node == nullptr) || (node == mScene->getRootNode())) {
		setSelectedNode(mFakeNode); // try again
	}

	if (mSelectedNode != node) {

		// Clean up the old
		if (mSelectedNode != nullptr && mSelectedNode != mFakeNode) {
			mSelectedNode->setDrawBounds(false);
		}

		removeParamIfExists("Node Debugger");
		removeParamIfExists("Node Name");
		removeParamIfExists("Node Position X");
		removeParamIfExists("Node Position Y");
		removeParamIfExists("Node Offset X");
		removeParamIfExists("Node Offset Y");
		removeParamIfExists("Node Rotation");
		removeParamIfExists("Node Scale");

		// Set up the new
		mSelectedNode = node;

		if (mSelectedNode != mFakeNode) {
			mSelectedNode->setDrawBounds(true);
			mSelectedNode->setBoundsColor(ci::Color("yellow"));
		}

		CI_LOG_V("Selected node: " << mSelectedNode->getName());

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
		RUI_SHARE_GS_PARAM_WCN("Node Scale",																					 //
													 [&]() -> float { return mSelectedNode->getScale().x; }, //
													 [&](float value) { mSelectedNode->setScale(value); },	 //
													 -10.0, 10.0);

		// Regardless, feed RUI
		ofxRemoteUIServer::instance()->pushParamsToClient();
	}
}

NodeDebugger::NodeDebugger() {
	CI_LOG_V("NodeDebugger Created");
}

NodeDebugger::~NodeDebugger() {
	CI_LOG_V("NodeDebugger Destroyed");
}
