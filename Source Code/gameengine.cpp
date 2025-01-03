// Class automatically generated by Dev-C++ New Class wizard

#include "gameengine.h" // class's header file

GameEngine::GameEngine() :  eg(EngineAPI::GetInstance()),
                            gl(GLAPIClass::GetInstance()), 
                            ki(KeyInputAPI::GetInstance()), 
                            as(AssetParser::GetInstance()) {
    this->mutex = CreateMutex(NULL, false, NULL);
    this->mainWnd = NULL;
    this->flagExit = false;
}

void GameEngine::Initialize(GameEngineInitializer& param) {
    this->mainWnd = param.mainWnd;
    
    // Recalculate Visible area
    RECT rect;
    GetClientRect(mainWnd, &rect);
    visibleHeight = rect.bottom - rect.top;
    param.windowSize.y = visibleHeight;
    // Init
    gl.EnableOpenGL(mainWnd, &hdc, &hrc, param.windowSize);
    gl.LoadCharacterSet();
    keepUpdateOnBackground = param.keepUpdateOnBackground;
    keepDrawOnBackground = param.keepDrawOnBackground;
    CustomInitialize();
    UpdateEngineState();
    
}

void GameEngine::OnEngineDestruct() {
    CustomOnEngineDestruct();
    gl.DisableOpenGL(mainWnd, hdc, hrc);
    CloseHandle(mutex);
    UpdateEngineState();
}

void GameEngine::FixedUpdate() {
    WaitForSingleObject(mutex, INFINITE);
    // Check Foreground
    if (keepUpdateOnBackground) {
        ki.SetIsWindowActive(isWindowActive);
    } else if (!isWindowActive) {
        ReleaseMutex(mutex);
        return;
    }
    
    // Stop Engine Flag Check
    if (eg.InternalGetShutdown()) {
        StopEngine();
        return;
    }
    
    // Update Cursor Pos Information
    POINT cpos;
    if (GetCursorPos(&cpos))
        if (ScreenToClient(mainWnd, &cpos))
            ki.UpdateCursorPos(Point2D(cpos.x, visibleHeight - cpos.y));
    
    switch (sCurrent->GetSceneState()) {
        // Important Note : BEGIN and END is not visible state. it's just an initializer and destructor for the scene.
        case Scene::END:
            sCurrent->InternalOnEnd();
            
            // Swap Scene
            if (Scene::GetSwapFlag()) {
                Scene* next = Scene::GetNextScene();
                if (next != NULL)
                    sCurrent = next;
                Scene::ResetSwapFlag();
            }
            sCurrent->SetSceneState(Scene::BEGIN);
            // Call OnBegin() without break
            
        case Scene::BEGIN:
            sCurrent->SetSceneState(Scene::LOADING);
            sCurrent->InternalOnBegin();
            // Call OnLoading() without break;
            
        case Scene::LOADING:
            sCurrent->InternalOnFixedLoading();
            break;
        
        case Scene::UPDATE:
            sCurrent->InternalOnFixedUpdate();
            break;
            
        case Scene::CLOSING:
            sCurrent->InternalOnFixedClosing();
            break;
    }
    ki.UpdateKeyState();
    ReleaseMutex(mutex);
}

void GameEngine::Update() {
    WaitForSingleObject(mutex, INFINITE);
    // Check Foreground
    isWindowActive = GetActiveWindow() == mainWnd;
    if (!keepDrawOnBackground && !isWindowActive) {
        ReleaseMutex(mutex);
        return;
    }
    
    // Stop Update Frame on Initialize / Destruct State
    Scene::SceneState state = sCurrent->GetSceneState();
    if (state == Scene::BEGIN || state == Scene::END) {
        ReleaseMutex(mutex);
        return;
    }
    
    // Update
    gl.ClearBuffer();
    gl.LoadIdentity();
    switch (state) {
        // Not gonna happen
        case Scene::BEGIN:
        case Scene::END:
            break;
            
        case Scene::LOADING:
            sCurrent->InternalOnLoading();
            break;
        
        case Scene::UPDATE:
            sCurrent->InternalOnUpdate();
            break;
            
        case Scene::CLOSING:
            sCurrent->InternalOnClosing();
            break;
    }
    gl.SwapBuffer();
    ReleaseMutex(mutex);
}

void GameEngine::SetInitialScene(Scene* scene) { 
    if (sCurrent) return;
    sCurrent = scene; 
    sCurrent->SetSceneState(Scene::BEGIN);
}

void GameEngine::UpdateEngineState() {
    // Set Engine State
    if ( gl.GetSuccess() ) {
        if ( state & GL_NOT_READY == GL_NOT_READY )
            state ^= GL_NOT_READY;
    } else
        state |= GL_NOT_READY;
    
    if ( sCurrent ) {
        if ( state & NO_CURRENT_SCENE == NO_CURRENT_SCENE)
            state ^= NO_CURRENT_SCENE;
    } else
        state |= NO_CURRENT_SCENE;
        
    if ( flagExit )
        state |= STOPPED;
    
    // Generate State String
    if (state != GameEngine::FINE) {
        stringstream ss;
        ss << "Incomplete Engine Initialization." << endl;
        ss << "State Code : " << state << endl << endl;
        if (state & GameEngine::GL_NOT_READY)
            ss << "* GL API Not Initialized." << endl;
        if (state & GameEngine::NO_CURRENT_SCENE)
            ss << "* No Initial Scene assigned." << endl;
        if (state & GameEngine::STOPPED)
            ss << "* Engine Already Stopped." << endl;
            
        stateString = ss.str();
    }
}
