//
//  GameState.h
//  GameDemo
//
//  Created by qiuhelin on 13-3-23.
//
//

#ifndef __GameDemo__GameState__
#define __GameDemo__GameState__

class CGameStateManager : public CCObject
{
protected:
    CGameStateManager();
public:
    ~CGameStateManager();
    virtual bool init();
    
    CREATE_FUNC_PARAM(CGameStateManager, ());
    static CGameStateManager* sharedGameStateManager();
    
protected:
    static CGameStateManager* m_pInst;
};
#endif /* defined(__GameDemo__GameState__) */
