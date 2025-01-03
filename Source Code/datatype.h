// Class automatically generated by Dev-C++ New Class wizard
#ifndef DATATYPE_H
#define DATATYPE_H

#include <gl/gl.h>
#include <vector>
#include <map>
#include <string>


using namespace std;

/////////////////////////////////////////////////////////////////////////
//////                      Typedef                                //////
/////////////////////////////////////////////////////////////////////////

typedef unsigned char       uchar;
typedef unsigned int        uint;
typedef unsigned long       ulong;
typedef unsigned long long  ullong;


/////////////////////////////////////////////////////////////////////////
//////                 Common Data Type                            //////
/////////////////////////////////////////////////////////////////////////

// Point2D (interger type)
struct Point2D {
    int x, y;
    Point2D() : x(0), y(0) { }
    Point2D(int _x, int _y) : x(_x), y(_y) { }
    inline bool operator< (const Point2D& _t) const     { return x <  _t.x && y <  _t.y; }
    inline bool operator> (const Point2D& _t) const     { return x >  _t.x && y >  _t.y; }
    inline bool operator<=(const Point2D& _t) const     { return x <= _t.x && y <= _t.y; }
    inline bool operator>=(const Point2D& _t) const     { return x >= _t.x && y >= _t.y; }
    inline bool operator==(const Point2D& _t) const     { return x == _t.x && y == _t.y; }
    inline Point2D operator+(const Point2D& _t)    { return Point2D(x + _t.x, y + _t.y); }
    inline Point2D operator-(const Point2D& _t)    { return Point2D(x - _t.x, y - _t.y); }
    inline Point2D operator*(const Point2D& _t)    { return Point2D(x * _t.x, y * _t.y); }
    inline Point2D operator/(const Point2D& _t)    { return Point2D(x / _t.x, y / _t.y); }
    inline Point2D operator*(const int _t)         { return Point2D(x * _t, y * _t); }
    inline Point2D operator/(const int _t)         { return Point2D(x / _t, y / _t); }
    inline void operator+=(const Point2D& _t)      { x += _t.x; y += _t.y; }
    inline void operator-=(const Point2D& _t)      { x -= _t.x; y -= _t.y; }
    inline void operator*=(const Point2D& _t)      { x *= _t.x; y *= _t.y; }
    inline void operator/=(const Point2D& _t)      { x /= _t.x; y /= _t.y; }
    inline void operator*=(const int _t)           { x *= _t; y *= _t; }
    inline void operator/=(const int _t)           { x /= _t; y /= _t; }
    inline static void Clamp(Point2D& src, const Point2D& ref) { 
        if (src.x > ref.x) src.x = ref.x; 
        if (src.y > ref.y) src.y = ref.y; 
    }
};

// Vector2D (float type)
struct Vector2D {
    float x, y; 
    Vector2D() : x(0.0f), y(0.0f) { }
    Vector2D(Point2D _t) : x(_t.x), y(_t.y) { }
    Vector2D(float _x, float _y) : x(_x), y(_y) { }
    inline Point2D toPoint2D() const { return Point2D((int)x, (int)y); }
    inline bool operator< (const Vector2D& _t) const    { return x <  _t.x && y <  _t.y; }
    inline bool operator> (const Vector2D& _t) const    { return x >  _t.x && y >  _t.y; }
    inline bool operator<=(const Vector2D& _t) const    { return x <= _t.x && y <= _t.y; }
    inline bool operator>=(const Vector2D& _t) const    { return x >= _t.x && y >= _t.y; }
    inline bool operator==(const Vector2D& _t) const    { return x == _t.x && y == _t.y; }
    inline Vector2D operator+(const Vector2D& _t)  { return Vector2D(x + _t.x, y + _t.y); }
    inline Vector2D operator-(const Vector2D& _t)  { return Vector2D(x - _t.x, y - _t.y); }
    inline Vector2D operator*(const Vector2D& _t)  { return Vector2D(x * _t.x, y * _t.y); }
    inline Vector2D operator/(const Vector2D& _t)  { return Vector2D(x / _t.x, y / _t.y); }
    inline Vector2D operator*(const float _t)      { return Vector2D(x * _t, y * _t); }
    inline Vector2D operator/(const float _t)      { return Vector2D(x / _t, y / _t); }
    inline Vector2D operator*(const Point2D& _t)   { return Vector2D(x * _t.x, y * _t.y); }
    inline Vector2D operator/(const Point2D& _t)   { return Vector2D(x / _t.x, y / _t.y); }
    inline void operator+=(const Vector2D& _t)     { x += _t.x; y += _t.y; }
    inline void operator-=(const Vector2D& _t)     { x -= _t.x; y -= _t.y; }
    inline void operator*=(const Vector2D& _t)     { x *= _t.x; y *= _t.y; }
    inline void operator/=(const Vector2D& _t)     { x /= _t.x; y /= _t.y; }
    inline void operator*=(const float _t)         { x *= _t; y *= _t; }
    inline void operator/=(const float _t)         { x /= _t; y /= _t; }
    inline void operator*=(const Point2D& _t)      { x *= _t.x; y *= _t.y; }
    inline void operator/=(const Point2D& _t)      { x /= _t.x; y /= _t.y; }
};

// Rect2D (from left bottom to right top)
struct Rect2D {
    Point2D lb, rt;
    Rect2D() : lb(), rt() { }
    Rect2D(int left, int bottom, int right, int top)    : lb(Point2D(left, bottom)), rt(Point2D(right, top)) { }
    Rect2D(Point2D _lb, Point2D _rt)                    : lb(_lb), rt(_rt) { }
    inline bool Contain(const Point2D& _p) { return lb <= _p && _p <= rt; }
};

// Angle In Degree
class Angle {
    private:
        float degree;
    public:
        Angle() : degree(0.0f) { }
        Angle(float f) { Set(f); }
        inline float Get() const { return degree; }
        void Set(float a) { 
            while (a < 0) 
                a += 360.0f; 
            degree = (a - (int)a) + ((int)a % 360); 
        }
        inline Angle operator+(const Angle& a) { return Angle(degree + a.degree); }
        inline Angle operator-(const Angle& a) { return Angle(degree - a.degree); }
        inline Angle operator+(const float a)  { return Angle(degree + a); }
        inline Angle operator-(const float a)  { return Angle(degree - a); }
        inline void operator+=(const Angle& a) { Set(degree + a.degree); }
        inline void operator-=(const Angle& a) { Set(degree - a.degree); }
        inline void operator+=(const float a)  { Set(degree + a); }
        inline void operator-=(const float a)  { Set(degree - a); }
};

struct Anchor {
    enum AnchorPos {
        LEFT_BOTTOM,
        RIGHT_BOTTOM,
        LEFT_TOP,
        RIGHT_TOP,
        CENTER
    };
    AnchorPos ap;
    Anchor() : ap(Anchor::LEFT_BOTTOM) { }
    Anchor(AnchorPos _ap) : ap(_ap) { }
    inline AnchorPos Get() const { return ap; }
    inline void operator=(const AnchorPos ap) { this->ap = ap; }
};

// Transformation Struct
struct Transformation {
    Vector2D position;
    Vector2D scale;
    Angle rotate;
    Anchor anchor;
    Transformation() : position(), scale(1.0f, 1.0f), rotate(), anchor() { }
    Transformation(Vector2D _pos, Vector2D _sca, Angle _rot) : position(_pos), scale(_sca), rotate(_rot), anchor() { }
};


/////////////////////////////////////////////////////////////////////////
//////                  GL API Texture                             //////
/////////////////////////////////////////////////////////////////////////

// Texture Generate Param
struct TextureGenParam {
    enum TextureMode {
        LINEAR,
        NEAREST
    };

    TextureMode minFilter;
    TextureMode magFilter;
    
    TextureGenParam() : minFilter(LINEAR), magFilter(LINEAR) { }
    TextureGenParam(TextureMode min, TextureMode mag) : minFilter(min), magFilter(mag) { }
    GLfloat GetValue(TextureMode mode);
    inline GLfloat GetMinFilter() { return GetValue(minFilter); }
    inline GLfloat GetMagFilter() { return GetValue(magFilter); }
};

// Texture Source
struct TextureSource {
    private:
        static uint counter;
    public:
        // ID
        const uint      uid;    //  Unique Identifier
        const GLuint    tid;    // Texture Identifier
        
        // Memory Size
        uchar power;
        ulong length;   // row size in pixel
        ulong totalSize;
        
        // Effective Range (Actual Data Available Range)
        ulong width, height;
        ulong size;
        Vector2D coord;
        
        // Used Range
        Rect2D range;
        
        TextureSource() :   uid(counter++), tid(0), power(0), length(0), totalSize(0), 
                            width(0), height(0), size(0), coord(), range() { }
        TextureSource(GLuint _tid) :  uid(counter++), tid(_tid), power(0), length(0), totalSize(0),
                                        width(0), height(0), size(0), coord(), range() { }
        virtual inline GLuint Get(ullong frame) const { return tid; }
        void SetRange(Rect2D range);
};

// Animation
struct AnimatedTexture : TextureSource {
    const vector<GLuint> tids;
    uint count;
    
    AnimatedTexture() : tids(), count(0) { }
    AnimatedTexture(const vector<GLuint>& _tids) : tids(_tids), count(_tids.size()) { }
    virtual inline GLuint Get(ullong frame) const { return count ? tids.at(frame % count) : 0; }
    inline vector<GLuint>::const_iterator Begin() { return tids.begin(); }
    inline vector<GLuint>::const_iterator End()   { return tids.end();   }
};

// Texture Storage
struct TextureStorage {
    map<uint, const TextureSource*> texmap;
    inline void Add(const TextureSource* texture) { texmap.insert(make_pair(texture->uid, texture)); }
    void Remove(uint uid);
    const TextureSource& Find(uint uid);
};

// Named Texture Storage
struct NamedTextureStorage {
    map<string, uint> namedmap;
    inline void Add(uint uid, string alias) { namedmap.insert(make_pair(alias, uid)); }
    void Remove(string alias);
    uint Find(string alias);
};


/////////////////////////////////////////////////////////////////////////
//////                     Interface                               //////
/////////////////////////////////////////////////////////////////////////

struct IDisposable {
    bool disposed;
    virtual void Dispose() { };
};

struct IUpdatable {
    // Single Called Initializer For Each State
	virtual void OnBegin() { }
	virtual void OnEnd() { }
    
    // Keep Called Until State Change (For Logic)
    virtual void OnFixedLoading() { }
	virtual void OnFixedUpdate() { }
	virtual void OnFixedClosing() { }
	
	// Keep Called Ultil State Change (For Drawing)
    virtual void OnLoading() { }
	virtual void OnUpdate() { }
	virtual void OnClosing() { }
};


#endif // DATATYPE_H
