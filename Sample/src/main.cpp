#include <windows.h>
#include "gameenginebuilder.h"
#include "gameengineimpl.h"

// Main Window Function
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow) {
    
    GameEngineBuilder builder(hInstance, nCmdShow);
    builder.SetEngineInstance(&GameEngineImpl::GetInstance())
           .SetWindowSize(800, 600)
           .SetWindowTitle("RPGME TEST APP")
		   .SetFixedUpdateDelay(1000000 / 30)
		   .SetUpdateDelay(1000000 / 30)
           .Build()
           .Launch();
    
    return 0;
}
