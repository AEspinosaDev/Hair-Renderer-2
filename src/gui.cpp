#include "gui.h"

void UserInterface::init(Core::WindowBase* window, Core::Scene* scene, Systems::RendererBase* renderer)
{

    overlay = new Tools::GUIOverlay((float)window->get_extent().width, (float)window->get_extent().height, GuiColorProfileType::DARK);

    Tools::Panel *explorerPanel = new Tools::Panel("EXPLORER", 0, 0, 0.2f, 0.7f, PanelWidgetFlags::NoMove, false);
    sceneWidget = new Tools::SceneExplorerWidget(scene);
    explorerPanel->add_child(sceneWidget);
    explorerPanel->add_child(new Tools::Space());
    explorerPanel->add_child(new Tools::RendererSettingsWidget(renderer));
    explorerPanel->add_child(new Tools::Separator());
    explorerPanel->add_child(new Tools::TextLine(" Application average"));
    explorerPanel->add_child(new Tools::Profiler());
    explorerPanel->add_child(new Tools::Space());

    overlay->add_panel(explorerPanel);
    explorer = explorerPanel;

    Tools::Panel *propertiesPanel = new Tools::Panel("OBJECT PROPERTIES", 0.75f, 0, 0.25f, 0.8f, PanelWidgetFlags::NoMove, true);
    objectWidget = new Tools::ObjectExplorerWidget();
    propertiesPanel->add_child(objectWidget);

    overlay->add_panel(propertiesPanel);
    properties = propertiesPanel;
}