// Class automatically generated by Dev-C++ New Class wizard

#include "glapi.h" // class's header file

const int charWidthMid = 17;
const int charWidthBig = 33;
const int charHeight = 33; 
const int charStrideMid = 15;
const int charStrideBig = 21;

void* GetAnyGLFuncAddress(const char* name) {
    void *p = (void*)wglGetProcAddress(name);
    if (p == 0 || (p == (void*)0x1) || (p == (void*)0x2) || 
       (p == (void*)0x3) || (p == (void*)-1) )
    {
        HMODULE module = LoadLibraryA("opengl32.dll");
        p = (void*)GetProcAddress(module, name);
    }
    return p;
}

void GLAPIClass::EnableOpenGL(HWND hWnd, HDC* hDC, HGLRC* hRC, Point2D windowSize) {
    PIXELFORMATDESCRIPTOR pfd;
    int iFormat;
    
    // get the device context (DC)
    *hDC = GetDC( hWnd );
    this->hdc = hDC;
    
    // set the pixel format for the DC
    ZeroMemory( &pfd, sizeof( pfd ) );
    pfd.nSize = sizeof( pfd );
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    //pfd.cColorBits = 24;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;
    iFormat = ChoosePixelFormat( *hDC, &pfd );
    SetPixelFormat( *hDC, iFormat, &pfd );
    
    // create and enable the render context (RC)
    *hRC = wglCreateContext( *hDC );
    wglMakeCurrent( *hDC, *hRC );
    
    // GL options
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // ViewPort And Projection Setting
    glViewport(0, 0, windowSize.x, windowSize.y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat aspect = (GLfloat)windowSize.x / (GLfloat)windowSize.y;
    if (windowSize.x > windowSize.y) {
        glOrtho(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0, 1.0, -1.0);
        this->viewportSize = Vector2D(2.0f * aspect, 2.0f);
    } else {
        glOrtho(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect, 1.0, -1.0);
        this->viewportSize = Vector2D(2.0f, 2.0f / aspect);
    }
    this->unitVertex2f = viewportSize / windowSize;
    this->centerVertex2f = viewportSize / 2.0f;
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // etc
    this->windowSize = windowSize;
    this->enabled = true;
}

void GLAPIClass::DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC) {
    wglMakeCurrent( NULL, NULL );
    wglDeleteContext( hRC );
    ReleaseDC( hWnd, hDC );
    enabled = false;
}


uint GLAPIClass::GenerateEmptyTexture(int width, int height, uint RGBA) {
    // Calculate Memory Size
    uint longer = width > height ? width : height;
    uint power = 0;
    while ( (1 << power) < longer )
        power++;
    ulong length = (1 << power);
    
    // Color Buffer
    unsigned char color[4];
    color[0] = (unsigned char)((RGBA >> 24) & 0xFF);
    color[1] = (unsigned char)((RGBA >> 16) & 0xFF);
    color[2] = (unsigned char)((RGBA >> 8)  & 0xFF);
    color[3] = (unsigned char)( RGBA        & 0xFF);
    
    // Generate Plain Color Data Stream
    ulong totalSize = length * length * 4;
    uchar* data = new uchar[totalSize];
    int repeat = length * length;
    for (int i = 0; i < repeat; i++)
        memcpy(&data[i * 4], color, sizeof(uchar) * 4);
    
    // Generate Texture
    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, length, length, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    delete data;
    
    // Build TextureSource
    TextureSource* dst = new TextureSource(id);
    dst->power = power;
    dst->length = length;
    dst->totalSize = totalSize;
    dst->width = width;
    dst->height = height;
    dst->size = width * height * 4;
    dst->coord = Vector2D((float)width / length, (float)height / length);
    dst->range.rt = Point2D(width, height);
    
    // Add Texture to reference map
    texStorage.Add(dst);
    return dst->uid;
}

uint GLAPIClass::LoadTexture(string filename, TextureGenParam param) {
    // Open file
    FILE* file;
    file = fopen(filename.c_str(), "rb");
    if (!file)
        return 0;
    
    // Read BMP File Descriptor
    char filedef[2];
    fread(&filedef, sizeof(char), 2, file);
    if (filedef[0] != 'B' || filedef[1] != 'M')
        return 0;
    
    // Get Filesize
    ullong filesize;
    fread(&filesize, sizeof(ullong), 1, file);
    
    // Get Effective Width, Height, Size
    ulong eWidth, eHeight;
    fseek(file, 18, 0);
    fread(&eWidth, sizeof(ulong), 1, file);
    fread(&eHeight, sizeof(ulong), 1, file);
    ulong eSize = eWidth * eHeight * 4;
    
    // Calculate Total Size and Allocate Memory
    uint power = 0;
    while ( (1 << power) < eWidth )
        power++;
    ulong length = (1 << power);
    ulong totalSize = length * length * 4;
    uchar* data = new uchar[totalSize];
    memset(data, 0x0, sizeof(uchar) * totalSize);

    // Check File Cotains Alpha Channel
    filesize -= 54;
    ulong bytePerRow = filesize / eHeight;
    bool alphaExists = (bytePerRow == eWidth * 4);
    uchar paddingByte = alphaExists ? 0 : bytePerRow - eWidth * 3;
    
    // Copy Data to memory
    fseek(file, 54, 0);
    if (alphaExists) {
        // Alpha Channel Exists
        for (int y = 0; y < eHeight; y++)
            fread(&data[y * length], sizeof(uchar), eWidth * 4, file);
    } else {
        // Alpha Channel Not Exists
        uchar buffer[4];
        for (int y = 0; y < eHeight; y++) {
            for (int x = 0; x < eWidth; x++) {
                fread(buffer, sizeof(char), 3, file);
                buffer[3] = ((buffer[0] == 0xFF) && (buffer[1] == 0xFF) && (buffer[2] == 0xFF)) ? 0x00 : 0xFF;
                memcpy(&data[(y * length + x) * 4], buffer, sizeof(uchar) * 4);
            }
            fread(buffer, sizeof(char), paddingByte, file);
        }
    }
    fclose(file);
    
    // Create Texture
    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param.GetMinFilter());
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param.GetMagFilter());
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, length, length, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
    delete data;
    
    // Build TextureSource
    TextureSource* dst = new TextureSource(id);
    dst->power = power;
    dst->length = length;
    dst->totalSize = totalSize;
    dst->width = eWidth;
    dst->height = eHeight;
    dst->size = eSize;
    dst->coord = Vector2D((float)eWidth / length, (float)eHeight / length);
    dst->range.rt = Point2D(eWidth, eHeight);
    
    // Add Texture to reference map
    texStorage.Add(dst);
    return dst->uid;
}

uint GLAPIClass::BuildAnimation (const vector<uint>& uids) {
    if (uids.empty())   return 0;   // Empty
    const TextureSource& ref = texStorage.Find(uids.at(0));
    return BuildAnimation(uids, ref.range);
}

uint GLAPIClass::BuildAnimation (const vector<uint>& uids, const Rect2D& range) {
    if (uids.empty())   return 0;   // Empty
    vector<GLuint> tids;
    vector<uint>::const_iterator iter;
    for (iter = uids.begin(); iter != uids.end(); iter++) {
        const TextureSource& ref = texStorage.Find(*iter);
        tids.push_back(ref.Get(0));
    }
    AnimatedTexture* anim = new AnimatedTexture(tids);
    const TextureSource& ref = texStorage.Find(uids.at(0));
    anim->power = ref.power;
    anim->width = ref.width;
    anim->height = ref.height;
    anim->SetRange(range);
    texStorage.Add(anim);
    return anim->uid;
}

uint GLAPIClass::CutTexture (const uint uid, const Rect2D& range) {
    const TextureSource& ref = texStorage.Find(uid);
    if (!ref.tid)   return 0;
    TextureSource* tex = new TextureSource(ref.tid);
    tex->power = ref.power;
    tex->width = ref.width;
    tex->height = ref.height;
    tex->SetRange(range);
    texStorage.Add(tex);
    return tex->uid;
}

void GLAPIClass::ClearTexture(const uint uid, const uint RGBA) {
    const TextureSource& ref = texStorage.Find(uid);
    if (!ref.tid)   return;
    // Color buffer
    uchar color[4];
    color[0] = (uchar)((RGBA >> 24) & 0xFF);
    color[1] = (uchar)((RGBA >> 16) & 0xFF);
    color[2] = (uchar)((RGBA >>  8) & 0xFF);
    color[3] = (uchar)( RGBA        & 0xFF);
    
    // Rebuild Texture
    int rep = ref.width * ref.height;
    uchar* data = new uchar[ref.size];
    memset(data, 0x0, ref.size);
    for (int i = 0; i < rep; i++)
        memcpy(&data[i * 4], color, sizeof(uchar) * 4);
            
    // Sub Tex
    glBindTexture(GL_TEXTURE_2D, ref.tid);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, ref.width, ref.height, GL_RGBA, GL_UNSIGNED_BYTE, data);
    delete data;
}


vector<uint> GLAPIClass::LoadMultipleTextures(string prefix, string suffix, uint digit, TextureGenParam param) {
    int index = 1;
    stringstream ss;
    stringstream len;
    vector<uint> uids;
    
    while (1) {
        ss.str(string());
        len.str(string());
        
        ss << prefix;
        len << index;
        int zeros = digit - len.str().length();
        for (int i = 0; i < zeros; i++)
            ss << "0";
        ss << index << suffix;
        uint uid = LoadTexture(ss.str(), param);
        if (!uid)
            break;
        uids.push_back(uid);
        index++;
    }
    return uids;
}
		


void GLAPIClass::Transform (const Transformation& tf) {
    // Transformation
    Vector2D pos = PxCoordToVertex2f(tf.position.toPoint2D());
    glTranslatef(pos.x, pos.y, 0.0f);
    glRotatef(tf.rotate.Get(), 0.0f, 0.0f, 1.0f);
    glScalef(tf.scale.x, tf.scale.y, 1.0f);
}

void GLAPIClass::TranslateMasterSO(const Transformation& tf) {
    // Anchor Correction
    Vector2D ac = centerVertex2f;
    switch(tf.anchor.Get()) {
        case Anchor::LEFT_BOTTOM:
            ac *= -1.0f;
            break;
            
        case Anchor::RIGHT_BOTTOM:
            ac.y *= -1.0f;
            break;
            
        case Anchor::LEFT_TOP:
            ac.x *= -1.0f;
            break;
            
        case Anchor::RIGHT_TOP:
            break;
            
        case Anchor::CENTER:
            ac = Vector2D();
            break;
    }
    Transform(tf);
    glTranslatef(ac.x, ac.y, 0.0f); // Anchor Correction
}

void GLAPIClass::DrawTextureAuto(const Transformation& tf, const uint uid, const ullong frame, const Point2D size) {
    const TextureSource& ref = texStorage.Find(uid);
    // Texture Size in Pixel Coord
    Point2D rtPx = size;
    Point2D texSize = Point2D(ref.range.rt) - ref.range.lb;
    if (size.x == 0)    // Use texture size for 0 size
        rtPx.x = texSize.x;
    if (size.y == 0)
        rtPx.y = texSize.y;
    
    // Texture Size in Vertex Coord
    Vector2D lbVer, rtVer;
    rtVer = PxCoordToVertex2f(rtPx);
    lbVer = rtVer / -2.0f;
    rtVer = rtVer /  2.0f;
    
    // Texture Coord
    Vector2D lbTex = PxCoordToTexCoord2f(ref.range.lb, ref.power);
    Vector2D rtTex = PxCoordToTexCoord2f(ref.range.rt, ref.power);
    
    // Anchor Correction Factor
    Vector2D ac = rtVer;
    switch(tf.anchor.Get()) {
        case Anchor::LEFT_BOTTOM:
            break;
            
        case Anchor::RIGHT_BOTTOM:
            ac.x *= -1;
            break;
            
        case Anchor::LEFT_TOP:
            ac.y *= -1;
            break;
            
        case Anchor::RIGHT_TOP:
            ac.x *= -1;
            ac.y *= -1;
            break;
            
        case Anchor::CENTER:
            ac = Vector2D();
            break;
    }
    
    // Get Texture
    GLuint tid = ref.Get(frame);
    
    // Transformation
    Transform(tf);
    
    // Draw Texture
    glPushMatrix();
    glTranslatef(ac.x, ac.y, 0.0f); // Anchor Correction
    DrawQuadTexture(lbVer.x, lbVer.y, rtVer.x, rtVer.y, lbTex.x, lbTex.y, rtTex.x, rtTex.y, tid);
    glPopMatrix();
}


void GLAPIClass::LoadCharacterSet() {
    // Hard coded
    const char* FILENAME = "./data/characterset/characterset.bmp";
    
    // Open file
    FILE* file;
    file = fopen(FILENAME, "rb");
    if (!file)
        return;
    
    // Read BMP File Descriptor
    char filedef[2];
    fread(&filedef, sizeof(char), 2, file);
    if (filedef[0] != 'B' || filedef[1] != 'M')
        return;
    
    // Get Filesize
    ullong filesize;
    fread(&filesize, sizeof(ullong), 1, file);
    
    // Get Effective Width, Height, Size
    ulong eWidth, eHeight;
    fseek(file, 18, 0);
    fread(&eWidth, sizeof(ulong), 1, file);
    fread(&eHeight, sizeof(ulong), 1, file);
    ulong eSize = eWidth * eHeight * 4;
    
    // Calculate Total Size and Allocate Memory
    uint power = 0;
    while ( (1 << power) < eWidth )
        power++;
    ulong length = (1 << power);
    ulong totalSize = length * length * 4;
    uchar* data = new uchar[totalSize];
    memset(data, 0x0, sizeof(uchar) * totalSize);

    // Check File Cotains Alpha Channel
    filesize -= 54;
    ulong bytePerRow = filesize / eHeight;
    bool alphaExists = (bytePerRow == eWidth * 4);
    uchar paddingByte = alphaExists ? 0 : bytePerRow - eWidth * 3;
    
    // Copy Data to memory
    fseek(file, 54, 0);
    if (alphaExists) {
        // Alpha Channel Exists
        for (int y = 0; y < eHeight; y++)
            fread(&data[y * length], sizeof(uchar), eWidth * 4, file);
    } else {
        // Alpha Channel Not Exists
        uchar buffer[4];
        uchar med;
        for (int y = 0; y < eHeight; y++) {
            for (int x = 0; x < eWidth; x++) {
                fread(buffer, sizeof(char), 3, file);
                med = ( buffer[0] + buffer[1] + buffer[2] ) / 3;
                buffer[0] = med;
                buffer[1] = med;
                buffer[2] = med;
                buffer[3] = 0xFF - med;
                //buffer[3] = ((buffer[0] == 0xFF) && (buffer[1] == 0xFF) && (buffer[2] == 0xFF)) ? 0x00 : 0xFF;
                memcpy(&data[(y * length + x) * 4], buffer, sizeof(uchar) * 4);
            }
            fread(buffer, sizeof(char), paddingByte, file);
        }
    }
    fclose(file);
    
    // Create Texture
    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, length, length, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
    delete data;
    
    // Build TextureSource
    TextureSource* dst = new TextureSource(id);
    dst->power = power;
    dst->length = length;
    dst->totalSize = totalSize;
    dst->width = eWidth;
    dst->height = eHeight;
    dst->size = eSize;
    dst->coord = Vector2D((float)eWidth / length, (float)eHeight / length);
    dst->range.rt = Point2D(eWidth, eHeight);
    
    // Add Texture to reference map
    texStorage.Add(dst);
    charset = dst->uid;
}

void GLAPIClass::DrawString(const Transformation& tf, const string str, const int count) {
    // Charset Info
    const TextureSource& ref = texStorage.Find(charset);
    
    // Draw Character
    uchar msb, lsb;
    int stride = 0;
    int linebreak = 0;
    for (int i = 0; i <= count; i++) {
        // Get character
        msb = str.at(i);
        
        if (msb == '\n') {
            // Linebreak
            linebreak++;
            stride = 0;
            continue;
        } else if (msb == ' ') {
            // Spacing Char
            stride += charStrideMid;
            continue;
        }
        
        if ((msb & 0x80) == 0x80) {
            // Read one more byte for unicode
            lsb = str.at(i + 1);
            i++;
        } else {
            lsb = 0x0;
        }
        
        // Validate
        if (!ValidateChar(msb, lsb))
            continue;
            
        // Get Char Tex Coord
        Vector2D lbTex, rtTex;
        FindCharTexCoord(msb, lsb, lbTex, rtTex);
        
        // Get Vertex Coord
        Vector2D lbVer, rtVer;
        bool thin = (msb < 0x80) || (msb == 0xA3);
        Point2D shift(stride, -linebreak * charHeight);
        lbVer = PxCoordToVertex2f(Point2D(0, -charHeight) + shift);
        rtVer = PxCoordToVertex2f(Point2D(charWidthBig, 0)   + shift);
        
        // Draw Texture
        DrawQuadTexture(lbVer.x, lbVer.y, rtVer.x, rtVer.y, lbTex.x, lbTex.y, rtTex.x, rtTex.y, ref.tid);
        
        // Update Shift
        stride += thin ? charStrideMid : charStrideBig;
    }
}

void GLAPIClass::DrawQuadTexture(const float x1,     const float y1,     const float x2,     const float y2,
                                 const float tex_x1, const float tex_y1, const float tex_x2, const float tex_y2, const GLuint tid)
{
    if (tid == 0)   return; // ignore null texture
    glBindTexture(GL_TEXTURE_2D, tid);
    glBegin(GL_POLYGON);
    glTexCoord2d(tex_x1, tex_y1); glVertex2f(x1, y1);
    glTexCoord2d(tex_x2, tex_y1); glVertex2f(x2, y1);
    glTexCoord2d(tex_x2, tex_y2); glVertex2f(x2, y2);
    glTexCoord2d(tex_x1, tex_y2); glVertex2f(x1, y2);
    glEnd();
}

/////////////////////////////////////////////////////////////////////
///////////////           Protected                   ///////////////
/////////////////////////////////////////////////////////////////////
Vector2D GLAPIClass::PxCoordToVertex2f(const Point2D& pixel) {
    Vector2D ver(pixel);
    ver *= unitVertex2f;
    return ver;
}

Vector2D GLAPIClass::PxCoordToTexCoord2f(const Point2D& pixel, const uchar power) { 
    Vector2D tex(pixel);
    tex /= (1 << power);
    return tex;
}

bool GLAPIClass::ValidateChar(const uint msb, const uint lsb) {
    // Is ASCII?
    if ((msb & 0x80) == 0x0) {
        return (0x21 <= msb) && (msb <= 0x7E);  // 0x20 = Spacing Char 
    }
    
    // Validate MSB
    if (msb < 0xA1 || msb > 0xC8 || ( (msb > 0xA4) && (msb < 0xB0) ))
        return false;
        
    // Validate LSB
    if (lsb < 0xA1 || lsb == 0xFF)
        return false;
    else if (msb == 0xA4 && lsb > 0xD4)
        return false;
    
    return true;
}

void GLAPIClass::FindCharTexCoord(uint msb, uint lsb, Vector2D& lbTex, Vector2D& rtTex) {
    bool isAscii = false;
    int row;
    if (msb < 0x80) {
        // ASCII
        row = 0;
        lsb = msb + 0x80;
        isAscii = true;
    } else if (msb == 0xA1) {
        // Special Char #1
        row = 27;
    } else if (msb == 0xA2) {
        // Special Char #2
        row = 28;
    } else if (msb == 0xA3) {
        // Also Unicode ASCII
        row = 0;
        isAscii = true;
    } else if (msb == 0xA4) {
        // Single KR Char
        row = 1;
    } else if (0xB0 <= msb && msb <= 0xC8) {
        // Combined KR Char
        row = msb - 0xB0 + 2;
    } else {
        // Not Found
        row = -1;
    }
    
    // Charset Info
    const TextureSource& ref = texStorage.Find(charset);
    
    // Texture Position Calculation
    Point2D charSetPos(lsb - 0xA1, row);
    Point2D lbpx, rtpx;
    if (isAscii) {
        lbpx = Point2D( charSetPos.x       * charWidthBig, ref.height - (charSetPos.y + 1) * charHeight);
        rtpx = Point2D((charSetPos.x + 1 ) * charWidthBig, ref.height -  charSetPos.y      * charHeight);
    } else {
        lbpx = Point2D( charSetPos.x       * charWidthBig, ref.height - (charSetPos.y + 1) * charHeight);
        rtpx = Point2D((charSetPos.x + 1 ) * charWidthBig, ref.height -  charSetPos.y      * charHeight);
    }
    lbTex = PxCoordToTexCoord2f(lbpx, ref.power);
    rtTex = PxCoordToTexCoord2f(rtpx, ref.power); 
}



// TODO : Move it to somewhere
/*
string GLAPIClass::GetPatternedFilename(const char* filenamePattern, int index) {
    stringstream ss;
    ss << filenamePattern;
    if (index < 100) {
        if (index < 10)
            ss << "0";
        ss << "0";
    }
    ss << index << ".bmp";
    return ss.str();
}

int GLAPIClass::GetAnimationFrameSize(const char* filenamePattern) {
    FILE* f = NULL;
    int counter = 1;
    do {
        if (f)  fclose(f);
        f = fopen(GetPatternedFilename(filenamePattern, counter).c_str(), "rb");
        counter++;
    } while (f);
    return counter - 2;
}
*/
