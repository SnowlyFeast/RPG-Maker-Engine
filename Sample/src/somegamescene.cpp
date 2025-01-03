// Class automatically generated by Dev-C++ New Class wizard

#include "somegamescene.h" // class's header file

const int FADE_IN_FRAME = 30;
const int FADE_OUT_FRAME = 30;

const Point2D PLAYER_SIZE = Point2D(75, 84);
const Point2D ENEMY_SIZE = Point2D(32, 32);

void SomeGameScene::Initialize() {
    const string TEXTURE_DIR = "./data/textures/";
    
    srand(time(NULL));
    
    // Add Scene Reference
    Scene::AddSceneToMap(this, "game");
    
    // Load Texture
    TextureGenParam param(TextureGenParam::LINEAR, TextureGenParam::LINEAR);
    nts.Add(gl.GenerateEmptyTexture(30, 30, 0xFFFFFFFF),            "fade");
    nts.Add(gl.LoadTexture(TEXTURE_DIR+"shork.bmp", param),         "shork");
    nts.Add(gl.LoadTexture(TEXTURE_DIR+"test_anim_002.bmp", param), "enemy");
    nts.Add(gl.LoadTexture(TEXTURE_DIR+"gameover.bmp", param),      "gg");
    
    
    // Register Object
    RegisterObject(hidden);
    RegisterObject(fade);
    RegisterObject(bg);
    RegisterObject(player);
    
    
    // Initialize Scene Object
    hidden.SetDepth(50);
    
    fade.texture = nts.Find("fade");
    fade.SetDepth(100);
    fade.renderOp = RenderObject::FIT_TO_SCREEN;
    
    bg.texture = nts.Find("gg");
    bg.SetDepth(60);
    
    player.texture = nts.Find("shork");
    player.SetDepth(30);
    player.renderOp = RenderObject::GIVEN_SIZE;
    player.renderSize = PLAYER_SIZE;
    player.transformation.position = Vector2D(300, 0);
    
}


void SomeGameScene::OnBegin() {
    frame = 0;
    tick = 90;
    increment = 1;
    jump = 0;
    gg = false;
    fade.enabled = true;
    bg.enabled = false;
    player.transformation.position = Vector2D(300, 0);
    
    // Clear Enemy
    vector<RenderObject*>::iterator iter;
    for (iter = enemy.begin() ; iter != enemy.end(); iter++) {
        delete *iter;
    }
    enemy.clear();
}

void SomeGameScene::OnEnd() {
    frame = 0;
    bg.enabled = false;
}



void SomeGameScene::OnFixedUpdate() {
    // Swap Scene
    KeyState h = ki.GetKeyState(H);
    if (h.control && h.shift && h.pressed) {
        fade.enabled = true;
        SceneEndOfUpdate();
        return;
    }
    
    // GG
    if (gg) return;
    
    // Move Character
    KeyState a = ki.GetKeyState(A);
    KeyState d = ki.GetKeyState(D);
    KeyState s = ki.GetKeyState(S);
    KeyState space = ki.GetKeyState(SPACE);
    
    if (a.down) {
        player.transformation.position.x -= 13;
        player.transformation.scale.x = 1.0f;
        player.transformation.anchor = Anchor::LEFT_BOTTOM;
    } else if (d.down) {
        player.transformation.position.x += 13;
        player.transformation.scale.x = -1.0f;
        player.transformation.anchor = Anchor::RIGHT_BOTTOM;
    }
    
    // Clamp x moving
    if (player.transformation.position.x < 0)
        player.transformation.position.x = 0;
    else if (player.transformation.position.x > 725)
        player.transformation.position.x = 725;
    
    // Jump
    if (space.down && (player.transformation.position.y < 1.0f)) 
        jump = 20;
    
    if (jump > 0 || player.transformation.position.y > 0.0f) {
        player.transformation.position.y += jump;
        jump--;
    }
    
    // Clamp y moving
    if (player.transformation.position.y < 0)
        player.transformation.position.y = 0;
        
    
    // Push Enemy
    if (tick >= 100) {
        tick = 0;
        if (increment < 100)
            increment += 1;
        RenderObject* shit = new RenderObject();
        shit->texture = nts.Find("enemy");
        shit->transformation.position = Vector2D(rand() % 770, 600);
        enemy.push_back(shit);
        RegisterObject(*shit, hidden);
    } else {
        tick += increment;
    }
    
    // Update Enemy
    vector<RenderObject*>::iterator iter;
    for (iter = enemy.begin() ; iter != enemy.end(); iter++) {
        // Get object
        RenderObject& ro = **iter;
        ro.transformation.position.y -= 10;
        if (ro.transformation.position.y < -50) {
            // Delete object
            ro.Dispose();
            delete *iter;
            enemy.erase(iter);
        }
        
        // Hit Check
        Point2D lb, rb, lt, rt;
        lb = ro.transformation.position.toPoint2D();
        rb = lb + Point2D(ENEMY_SIZE.x, 0);
        lt = lb + Point2D(0,            ENEMY_SIZE.y);
        rt = lb + Point2D(ENEMY_SIZE.x, ENEMY_SIZE.y);
        
        Rect2D rec(player.transformation.position.toPoint2D() + Point2D(10, 30),
                   player.transformation.position.toPoint2D() + PLAYER_SIZE - Point2D(10, 30));
        if (rec.Contain(lb) || rec.Contain(rb) || rec.Contain(lt) || rec.Contain(rt)) {
            // GG
            bg.enabled = true;  
            gg = true;
        }
        
    }
}

void SomeGameScene::OnFixedLoading() {
    // Fade-in time control
    if (frame >= FADE_IN_FRAME) {
        SceneEndOfLoading();
        frame = 0;
        fade.enabled = false;
    } else {
        frame++;
    }
}

void SomeGameScene::OnFixedClosing() {
    // Fade-out time control
    if (frame >= FADE_OUT_FRAME) {
        frame = 0;
        fade.enabled = false;
        QueueSceneSwap("sample");
        SceneEndOfClosing();
    } else {
        frame++;
    }
}

void SomeGameScene::OnLoading() {
    // Fade in effect
    uchar c = (uchar)(frame * 0xFF / FADE_IN_FRAME);
    uint rgba = 0x00000000 | ((uchar)0xFF - c);
    gl.ClearTexture(nts.Find("fade"), rgba);
    masterSceneObject.Draw();
    
}

void SomeGameScene::OnUpdate() {
}

void SomeGameScene::OnClosing() {
    // Fade out effect
    uchar c = (uchar)(frame * 0xFF / FADE_OUT_FRAME);
    uint rgba = 0x00000000 | c;
    gl.ClearTexture(nts.Find("fade"), rgba);
    masterSceneObject.Draw();
}
