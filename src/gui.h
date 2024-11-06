#pragma once

#include <engine/tools/gui.h>
#include <engine/tools/renderer_widget.h>
USING_VULKAN_ENGINE_NAMESPACE

struct UserInterface {
    
    Tools::GUIOverlay*           overlay{nullptr};
    Tools::Panel*                explorer{nullptr};
    Tools::Panel*                properties{nullptr};
    Tools::SceneExplorerWidget*  sceneWidget{nullptr};
    Tools::ObjectExplorerWidget* objectWidget{nullptr};

    void init(Core::IWindow* window, Core::Scene* scene, Systems::BaseRenderer* renderer);
};