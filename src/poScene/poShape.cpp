//
//  poShape.cpp
//  BasicScene
//
//  Created by Stephen Varga on 3/19/14.
//
//

#include "poShape.h"

namespace po {
    //BASE CLASS
    #pragma mark - Shape -
    
    ShapeRef Shape::create()
    {
        return std::shared_ptr<Shape>(new Shape());
    }
    
    
    //Rect
    ShapeRef Shape::createRect(float width, float height)
    {
        std::shared_ptr<Shape> s = std::shared_ptr<Shape>(new Shape());
        s->ciShape2d.moveTo(0,0);
        s->ciShape2d.lineTo(width,0);
        s->ciShape2d.lineTo(width,height);
        s->ciShape2d.lineTo(0,height);
        s->ciShape2d.close();
        return s;
    }
    
    ShapeRef Shape::createRect(float size)
    {
        return createRect(size, size);
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
        
        s->ciShape2d.moveTo(x, height/2);
        s->ciShape2d.curveTo(x, ym-oy, xm - ox, y, xm, y);
        s->ciShape2d.curveTo(xm + ox, y, xe, ym - oy, xe, ym);
        s->ciShape2d.curveTo(xe, ym + oy, xm + ox, ye, xm, ye);
        s->ciShape2d.curveTo(xm - ox, ye, x, ym + oy, x, ym);
        s->ciShape2d.close();

        return s;
    }
    
    ShapeRef Shape::createEllipse(float size)
    {
        return createEllipse(size, size);
    }
    
    Shape::Shape()
    :   fillColor(255,255,255)
    ,   fillEnabled(true)
    ,   strokeColor(255,255,255)
    ,   strokeEnabled(false)
    {
    }
    
    Shape::~Shape() {}
    
    bool Shape::pointInside(ci::Vec2f point)
    {
        std::cout << globalToLocal(point) << std::endl;
        return ciShape2d.contains(globalToLocal(point));
    }
    
    void Shape::draw()
    {
        //Draw fill
        if(fillEnabled) {
            ci::gl::color(fillColor);
            ci::gl::drawSolid(ciShape2d, ci::app::getWindowContentScale());
        }
        
        //Draw stroke
        if(strokeEnabled) {
            ci::gl::color(strokeColor);
            ci::gl::draw(ciShape2d, ci::app::getWindowContentScale());
        }
    }
    
    ci::Rectf Shape::getBounds()
    {
        #pragma message "Not sure if this is gonna work"
        ci::Rectf boundingBox = ciShape2d.calcBoundingBox();

        return ciShape2d.calcBoundingBox();
    }
}