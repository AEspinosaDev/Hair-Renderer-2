#include "gui.h"

void UserInterface::init(Window* window, Scene* scene, Renderer* renderer)
{

    overlay = new GUIOverlay((float)window->get_extent().width, (float)window->get_extent().height, GuiColorProfileType::DARK);

    Panel *explorerPanel = new Panel("EXPLORER", 0, 0, 0.2f, 0.7f, PanelWidgetFlags::NoMove, false);
    sceneWidget = new SceneExplorerWidget(scene);
    explorerPanel->add_child(sceneWidget);
    explorerPanel->add_child(new Space());
    explorerPanel->add_child(new RendererSettingsWidget(renderer));
    explorerPanel->add_child(new Separator());
    explorerPanel->add_child(new TextLine(" Application average"));
    explorerPanel->add_child(new Profiler());
    explorerPanel->add_child(new Space());

    overlay->add_panel(explorerPanel);
    explorer = explorerPanel;

    Panel *propertiesPanel = new Panel("OBJECT PROPERTIES", 0.75f, 0, 0.25f, 0.8f, PanelWidgetFlags::NoMove, true);
    objectWidget = new ObjectExplorerWidget();
    propertiesPanel->add_child(objectWidget);

    overlay->add_panel(propertiesPanel);
    properties = propertiesPanel;
}