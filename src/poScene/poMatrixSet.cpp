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

#include "poMatrixSet.h"

namespace po {
namespace scene {

void MatrixSet::set(glm::mat4x4 modelview, glm::mat4x4 projection, ci::Area viewport) {
	mModelview = modelview;
	mProjection = projection;
	mViewport = glm::vec4(viewport.getX1(), viewport.getY1(), viewport.getX2(), viewport.getY2());
}

ci::vec2 MatrixSet::globalToLocal(const ci::vec2 &point) {
	// Flip Y ?
	ci::vec3 p(point.x, (mViewport.w - mViewport.y) - point.y, 0.f); // hmm no longer needed?
	// ci::vec3 p(point.x, point.y, 0.0);
	ci::vec3 r = unProject(p);
	return ci::vec2(r.x, r.y);
}

ci::vec2 MatrixSet::localToGlobal(const ci::vec2 &point) {
	glm::mat4x4 a = mProjection * mModelview;
	a = glm::inverse(a);
	ci::vec3 p = project(ci::vec3(point.x, point.y, 0.f));
	return ci::vec2(p.x, (mViewport.w - mViewport.y) - p.y); // hmm does flipping this help?
}

ci::vec3 MatrixSet::project(const ci::vec3 &pt) {
	return glm::project(pt, mModelview, mProjection, mViewport);
}

ci::vec3 MatrixSet::unProject(const ci::vec3 &pt) {
	return glm::unProject(pt, mModelview, mProjection, mViewport);
}

} // namespace scene
} // namespace po