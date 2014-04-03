//
//  poShape.cpp
//  BasicScene
//
//  Created by Stephen Varga on 3/19/14.
//
//
#include "cinder/TriMesh.h"
#include "cinder/Triangulate.h"
#include "cinder/gl/Texture.h"

#include "poShape.h"

namespace po {
    #pragma mark - Create -
    
    ShapeRef Shape::create()
    {
        return std::shared_ptr<Shape>(new Shape());
    }
    
    
    //Rect
    ShapeRef Shape::createRect(float width, float height)
    {
        std::shared_ptr<Shape> s = std::shared_ptr<Shape>(new Shape());
        
        ci::Shape2d shape;
        
        shape.moveTo(0,0);
        shape.lineTo(width,0);
        shape.lineTo(width,height);
        shape.lineTo(0,height);
        shape.close();
        
        s->setCiShape2d(shape);
        s->render();
        
        return s;
    }
    
    ShapeRef Shape::createRect(float size)
    {
        return createRect(size, size);
    }
    
    ShapeRef Shape::createRect(ci::gl::TextureRef texture)
    {
        
        std::shared_ptr<Shape> s = Shape::createRect(texture->getWidth(), texture->getHeight());
        s->mTexture = texture;
        s->render();
        
        return s;
    }
    
    //Ellipse
    ShapeRef Shape::createEllipse(float width, float height)
    {
        std::shared_ptr<Shape> s = std::shared_ptr<Shape>(new Shape());
        
        //Adapted from http://stackoverflow.com/questions/2172798/how-to-draw-an-oval-in-html5-canvas
        float kappa = .5522848;
        float x = 0;
        float y = 0;
        float ox = (width / 2) * kappa; // control point offset horizontal
        float oy = (height / 2) * kappa; // control point offset vertical
        float xe = x + width;           // x-end
        float ye = y + height;           // y-end
        float xm = x + width / 2;       // x-middle
        float ym = y + height / 2;       // y-middle
        
        ci::Shape2d shape;
        
        shape.moveTo(x, height/2);
        shape.curveTo(x, ym-oy, xm - ox, y, xm, y);
        shape.curveTo(xm + ox, y, xe, ym - oy, xe, ym);
        shape.curveTo(xe, ym + oy, xm + ox, ye, xm, ye);
        shape.curveTo(xm - ox, ye, x, ym + oy, x, ym);
        shape.close();
        
        s->setCiShape2d(shape);
        s->render();

        return s;
    }
    
    ShapeRef Shape::createEllipse(float size)
    {
        return createEllipse(size, size);
    }
    
    
    #pragma mark - Shape -
    Shape::Shape()
    :   mFillColor(255,255,255)
    ,   mFillEnabled(true)
    ,   mStrokeColor(255,255,255)
    ,   mStrokeEnabled(false)
    ,   mPrecision(100)
    ,   mTextureFit(TextureFit::NONE)
    ,   mTextureAlignment(Alignment::TOP_LEFT)
    {
    }
    
    Shape::~Shape() {}
    
    #pragma mark - ci::Shape2d -
    void Shape::setCiShape2d(ci::Shape2d shape)
    {
        mCiShape2d = shape;
        render();
    }
    
    #pragma mark - Drawing -
    void Shape::draw()
    {
        //Draw fill
        if(mFillEnabled) {
            ci::gl::color(mFillColor);
        
            if(mTexture) mTexture->enableAndBind();
            ci::gl::draw(mVboMesh);
            if(mTexture) mTexture->disable();
        }
        
        //Draw stroke
        #pragma message "Need to implement better stroke stuff"
        if(mStrokeEnabled) {
            ci::gl::color(mStrokeColor);
            ci::gl::draw(mCiShape2d, ci::app::getWindowContentScale());
        }
    }
    
    #pragma mark - Texture -
    void Shape::setTexture(ci::gl::TextureRef texture, po::TextureFit fit, po::Alignment alignment) {
        mTexture    = texture;
        mTextureFit = fit;
        mTextureAlignment  = alignment;
        
        render();
    }
    
    #pragma mark - Rendering -
    void Shape::render()
    {
        //Create Mesh
        ci::TriMesh2d mesh = ci::Triangulator(mCiShape2d, mPrecision).calcMesh(ci::Triangulator::WINDING_ODD);
        
        if(mTexture) {
            std::vector<ci::Vec2f> texCoords(mesh.getVertices().size());
            po::textureFit(getBounds(), mTexture, mTextureFit, mTextureAlignment, mesh.getVertices(), texCoords);
            mesh.appendTexCoords(&texCoords[0], texCoords.size());
        }
        
        //Create VBO Mesh
        mVboMesh = ci::gl::VboMesh::create( mesh );
    }
    
    #pragma mark - Dimensions -
    bool Shape::pointInside(const ci::Vec2f &point)
    {
        return mCiShape2d.contains(globalToLocal(point));
    }
    
    ci::Rectf Shape::getBounds()
    {
        if(mBoundsDirty) {
            mBounds       = mCiShape2d.calcPreciseBoundingBox();
            mBoundsDirty = false;
        }
        
        return mBounds;
    }
}