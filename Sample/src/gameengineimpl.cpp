// Class automatically generated by Dev-C++ New Class wizard

#include "gameengineimpl.h" // class's header file


void GameEngineImpl::CustomInitialize() {
    sample.Initialize();
    game.Initialize();
    SetInitialScene(&sample);
}

void GameEngineImpl::CustomOnEngineDestruct() {
    
}