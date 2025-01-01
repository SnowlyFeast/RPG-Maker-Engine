// Class automatically generated by Dev-C++ New Class wizard

#include "renderobject.h" // class's header file

void RenderObject::SetTexture(uint texture) {
    this->texture = texture;
    const TextureSource& ref = gl.GetTextureInformation(texture);
    this->renderSize = Point2D(ref.range.rt) - ref.range.lb;
}

void RenderObject::Draw() {
    if (!enabled)   return;
    
    // Push Matrix
    gl.PushMatrix();
    
    // Render size decision
    Point2D arg;
    if (renderOp & RenderObject::FIT_TO_SCREEN_KEEP_RATIO) {
        // Fit to screen but keep ratio
        const TextureSource& ref = gl.GetTextureInformation(texture);
        Point2D usedRange = Point2D(ref.range.rt) - ref.range.lb;
        float aspect = (float)usedRange.x / usedRange.y;
        
        // Calculate stretch ratio
        Point2D winsize = gl.GetWindowSize();
        Vector2D stretchRatio = Vector2D(winsize) / usedRange;
        
        if (stretchRatio.x < stretchRatio.y) {
            arg = Point2D(winsize.x, (int)(stretchRatio.x * usedRange.y));
        } else {
            arg = Point2D((int)(stretchRatio.y * usedRange.x), winsize.y);
        }
        
    } else {
        // Size specifier
        if (renderOp & RenderObject::TEXTURE_SIZE) {
            arg = Point2D();
        } else if (renderOp & RenderObject::GIVEN_SIZE) {
            arg = renderSize;
        }
        
        // Stretch Options
        if (renderOp & RenderObject::STRETCH_HOR)
            arg.x = gl.GetWindowSize().x;
        
        if (renderOp & RenderObject::STRETCH_VER)
            arg.y = gl.GetWindowSize().y;
    }
    
    // Draw Texture
    gl.DrawTextureAuto(transformation, texture, tick, arg);
    
    // Draw Child Object
    vector<SceneObject*>& children = GetChildrenVector();
    vector<SceneObject*>::iterator iter;
    for (iter = children.begin(); iter != children.end(); iter++) {
        (*iter)->Draw();
    }
    
    // Pop Matrix
    gl.PopMatrix();
}