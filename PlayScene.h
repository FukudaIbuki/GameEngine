#pragma once
#include "GameObject.h"
class PlayScene :
    public GameObject
{
public:
    PlayScene(GameObject* parent, const std::string& name);
    ~PlayScene();
     void Initialize() override;
     void Update() override;
     void Draw() override;
     void Release() override;
};

