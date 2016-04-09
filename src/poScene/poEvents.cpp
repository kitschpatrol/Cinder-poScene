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

#include "poEvents.h"
#include "poNode.h"
#include "poScene.h"

namespace po {
namespace scene {

//------------------------------------
//	Base Event
//------------------------------------

Event::Event()
		: mPropagationEnabled(false)
		, mWindowPos(0, 0) {
}

ci::vec2 Event::getScenePos() {
	NodeRef source = getSource();
	if (source) {
		return source->windowToScene(getWindowPos());
	}

	return getWindowPos();
}

ci::vec2 Event::getLocalPos() {
	NodeRef source = getSource();
	if (source) {
		return source->windowToLocal(getWindowPos());
	}

	return getWindowPos();
}

//------------------------------------
//	Mouse Event
//------------------------------------

MouseEvent::MouseEvent(ci::app::MouseEvent event, Type type)
		: mCiEvent(event)
		, mType(type) {
	mWindowPos = event.getPos();
}

//------------------------------------
//	Touch Event
//------------------------------------

TouchEvent::TouchEvent(ci::app::TouchEvent::Touch event, Type type)
		: mCiEvent(event)
		, mType(type) {
	mWindowPos = event.getPos();
}

} // namespace scene
} // namespace po
