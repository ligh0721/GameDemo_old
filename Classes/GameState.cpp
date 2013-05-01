//
//  GameState.cpp
//  GameDemo
//
//  Created by qiuhelin on 13-3-23.
//
//
#include "CommInc.h"

#include "GameState.h"

CGameStateManager* CGameStateManager::m_pInst = NULL;

CGameStateManager::CGameStateManager()
{}

CGameStateManager::~CGameStateManager()
{}

bool CGameStateManager::init()
{
    return true;
}

CGameStateManager* CGameStateManager::sharedGameStateManager()
{
    if (!m_pInst)
    {
        m_pInst = CGameStateManager::create();
    }
    return m_pInst;
}
