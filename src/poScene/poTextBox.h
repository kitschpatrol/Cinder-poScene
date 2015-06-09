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

#include "cinder/Text.h"
#include "cinder/gl/Texture.h"
#include "poNode.h"

namespace po { namespace scene {
	
	class TextBox;
	typedef std::shared_ptr<TextBox> TextBoxRef;
	
	class TextBox
	: public Node
	{
	public:
		static TextBoxRef create(ci::TextBox ciTextBox);
		static TextBoxRef create();
		
		void draw();
		
		ci::Rectf getBounds();
		void setCiTextBox(ci::TextBox ciTextBox) { mCiTextBox = ciTextBox; render(); }
        ci::TextBox getCiTextBoxCopy() { return mCiTextBox; };
		
	protected:
		TextBox(ci::TextBox ciTextBox);
		
	private:
        void render();
		ci::TextBox mCiTextBox;
		ci::gl::TextureRef mTexture;
		bool mUseTextBounds;
		
	};
		
} } //  namespace po::scene