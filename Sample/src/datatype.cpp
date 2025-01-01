#include "datatype.h"

////////////////////////////////
//   struct TextureGenParam   //
////////////////////////////////
GLfloat TextureGenParam::GetValue(TextureMode mode) { 
    switch (mode) {
        case TextureGenParam::LINEAR:
            return GL_LINEAR;
        case TextureGenParam::NEAREST:
            return GL_NEAREST;
    }
}


////////////////////////////////
//    struct TextureSource    //
////////////////////////////////
uint TextureSource::counter = 1;

void TextureSource::SetRange(Rect2D range) {
    Point2D ref(width, height);
    Point2D::Clamp(range.lb, ref);
    Point2D::Clamp(range.rt, ref);
    this->range = range;
}


////////////////////////////////
//    struct TextureStorage   //
////////////////////////////////
void TextureStorage::Remove(uint uid) {
    map<uint, const TextureSource*>::iterator iter(texmap.find(uid));
    if (iter != texmap.end()) {
        delete iter->second;
        texmap.erase(iter);
    }
}

const TextureSource& TextureStorage::Find(uint uid) {
    static TextureSource NULL_TEXTURESOURCE = TextureSource();
    map<uint, const TextureSource*>::iterator iter(texmap.find(uid));
    if (iter != texmap.end())
        return *(iter->second);
    else
        return NULL_TEXTURESOURCE;
}


////////////////////////////////
// struct NamedTextureStorage //
////////////////////////////////
void NamedTextureStorage::Remove(string alias) {
    map<string, uint>::iterator iter(namedmap.find(alias));
    if (iter != namedmap.end())
        namedmap.erase(iter);
}

uint NamedTextureStorage::Find(string alias) {
    map<string, uint>::iterator iter(namedmap.find(alias));
    return (iter != namedmap.end()) ? iter->second : 0;
}
