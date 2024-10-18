#pragma once
#include "GameObject.h"
class RootJob :
    public GameObject
{
public:
    RootJob();
    ~RootJob();

    virtual void Initialize() override;
    virtual void Update() override;
    virtual void Draw() override;
    virtual void Release() override;
};

