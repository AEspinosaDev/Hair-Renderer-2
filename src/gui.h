#pragma once

#include <engine/utilities/gui.h>
#include <engine/utilities/renderer_widget.h>
USING_VULKAN_ENGINE_NAMESPACE

struct UserInterface
{
    GUIOverlay *overlay{nullptr};

    Panel *explorer{nullptr};
    Panel *properties{nullptr};

    SceneExplorerWidget *sceneWidget{nullptr};
    ObjectExplorerWidget *objectWidget{nullptr};

    void init(Window* window, Scene* scene, Renderer* renderer);
};