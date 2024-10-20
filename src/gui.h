#pragma once

#include <engine/tools/gui.h>
#include <engine/tools/renderer_widget.h>
USING_VULKAN_ENGINE_NAMESPACE

struct UserInterface
{
    GUIOverlay *overlay{nullptr};

    Panel *explorer{nullptr};
    Panel *properties{nullptr};

    SceneExplorerWidget *sceneWidget{nullptr};
    ObjectExplorerWidget *objectWidget{nullptr};

    void init(Core::Window* window, Core::Scene* scene, Systems::Renderer* renderer);
};