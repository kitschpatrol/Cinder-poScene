/*
 Copyright (c) 2015, Potion Design LLC
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

 * Neither the name of copyright holder nor the names of its
 contributors may be used to endorse or promote products derived from
 this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include "cinder/Camera.h"
#include "poEventCenter.h"
#include "poNodeContainer.h"

namespace po {
namespace scene {
// Scenes are (naturally) the main entry points for po::scene.
// Scenes control the update and draw phases of the draw tree,
// as well as the orthographic camera (if enabled)
//
// Scenes also are responsible for tracking all Nodes currently
// in the root Node's hierarchy.
//
// The Scene hands these NodeRefs every frame to the event center for processing
// and distribution of events.
//
// An application can have multiple Scenes, and Nodes can belong to
// any Scene (but only 1 at a given time).
//
// Scenes can not have children; instead, all NodeRefs should
// be added to the Scene's root Node. The root Node is generated by default
// but can also be set at any point.

//	Forward declare node
class Node;
typedef std::shared_ptr<Node> NodeRef;

//	Create SceneRef typedef
class Scene;
typedef std::shared_ptr<Scene> SceneRef;

class Scene : public std::enable_shared_from_this<Scene> {
	friend class Node;
	friend class EventCenter; // So we can access the child nodes

public:
	static SceneRef create();
	static SceneRef create(NodeContainerRef rootNode);
	~Scene();

	//! Get this Scene's internal camera
	ci::CameraOrtho &getCamera() {
		return mCamera;
	}
	//! Enable this Scene's internal camera. If this is disabled the user is responsible for setting their own camera.
	void setAutoCam(bool autoCam) {
		mAutoCam = autoCam;
	}
	//! See if this Scene's internal camera is enabled
	bool getAutoCam() {
		return mAutoCam;
	};

	//! Update the Scene. This needs to be called manually for every scene.
	virtual void update();
	//! Draw the Scene. This needs to be called manually for every scene
	virtual void draw();

	//! Get the next draw order ID for this frame (reset every frame)
	uint32_t getNextDrawOrder();

	//! Get the current root Node for this scene
	NodeContainerRef getRootNode() {
		return mRootNode;
	};
	//! Get the root Node
	void setRootNode(NodeContainerRef node);

	NodeRef getNodeUnderPoint(ci::vec2 point);

protected:
	Scene(NodeContainerRef rootNode);

	//	Root Node of scene
	NodeContainerRef mRootNode;

	//	Our Event Center (each Scene has its own)
	EventCenterRef eventCenter;

	//	Internal tracking functions
	void trackChildNode(NodeRef node);
	void untrackChildNode(NodeRef node);
	std::vector<NodeRef> allChildren;

	// Fbo's for use by nodes
	std::shared_ptr<ci::gl::Fbo> getWindowFbo() {
		return mFbo;
	};
	std::shared_ptr<ci::gl::Fbo> getMaskFbo() {
		return mMaskFbo;
	};

private:
	//
	//	Each object gets its own draw order every frame.
	//	This lets us sort objects for hit testing.
	//
	uint32_t drawOrderCounter;

	ci::CameraOrtho mCamera;
	bool mAutoCam;

	//
	//	Queue to track/untrack children on the next frame
	//	This lets us pass our children to the event center by reference (copying super slow)
	//	but not risk modifying the vector during iteration (i.e. add/remove child during event callback)
	//
	void processTrackingQueue();
	std::map<NodeRef, bool> mTrackingQueue;

	//
	//  FBOs for caching + masking
	void createFbos();
	void resetFbos();
	ci::gl::FboRef mFbo;
	ci::gl::FboRef mMaskFbo;
};

} // namespace scene
} // namespace po