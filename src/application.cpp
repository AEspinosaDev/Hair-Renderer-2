#include "application.h"
#include <filesystem>

void HairViewer::init(RendererSettings settings)
{
    m_window = new Window("VK Engine", 1280, 1024);

    m_window->init();

    m_window->set_window_size_callback(
        std::bind(&HairViewer::window_resize_callback, this, std::placeholders::_1, std::placeholders::_2));
    m_window->set_mouse_callback(
        std::bind(&HairViewer::mouse_callback, this, std::placeholders::_1, std::placeholders::_2));
    m_window->set_key_callback(std::bind(&HairViewer::keyboard_callback, this, std::placeholders::_1,
                                         std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));

    m_renderer = new ForwardRenderer(m_window, settings, {HIGH, false});

    setup();

    m_interface.init(m_window, m_scene, m_renderer);
    m_renderer->set_gui_overlay(m_interface.overlay);
}

void HairViewer::run(RendererSettings settings)
{

    init(settings);
    while (!m_window->get_window_should_close())
    {

        // I-O
        Window::poll_events();

        tick();
    }
    m_renderer->shutdown(m_scene);
}

void HairViewer::setup()
{
    const std::string MESH_PATH(RESOURCES_PATH "models/");
    const std::string TEXTURE_PATH(RESOURCES_PATH "textures/");
    const std::string ENGINE_MESH_PATH(ENGINE_RESOURCES_PATH "meshes/");

    camera = new Camera();
    camera->set_position(Vec3(0.0f, 0.0f, -7.0f));
    camera->set_far(100.0f);
    camera->set_near(0.1f);
    camera->set_field_of_view(70.0f);

    m_scene = new Scene(camera);

    m_scene->add(new PointLight());
    m_scene->get_lights()[0]->set_position({-3.0f, 3.0f, 0.0f});
    m_scene->get_lights()[0]->set_shadow_fov(115.0f);
    m_scene->get_lights()[0]->set_name("PointLight");

    Mesh *hair = new Mesh();
    hair->load_file(MESH_PATH + "curly.hair", false);
    HairMaterial* hmat = new HairMaterial(); 
    hmat->set_base_color(Vec3{0.21f,0.13f,0.067f});
    hair->set_material(hmat);
    hair->set_rotation({-90.0, 0.0f, 90.0f});
    hair->set_name("Hair");
    hair->set_scale(0.053f);
    m_scene->add(hair);

    Mesh *head = new Mesh();
    head->set_rotation({0.0, 270.0f, 180.0f});
    head->load_file(MESH_PATH + "woman.ply");
    auto headMat = new PhysicallyBasedMaterial();
    headMat->set_albedo(Vec3{0.21f,0.18f,0.085f});
    headMat->set_metalness(0.1f);
    headMat->set_roughness(0.65f);
    head->set_material(headMat);
    head->set_name("Head");
    m_scene->add(head);

    m_scene->set_ambient_color({0.2, 0.25, 0.61});

    m_controller = new Controller(camera);
}

void HairViewer::update()
{
    if (!m_interface.overlay->wants_to_handle_input())
        m_controller->handle_keyboard(m_window->get_handle(), 0, 0, m_time.delta);

    // Rotate the vector around the ZX plane
    auto light = m_scene->get_lights()[0];
    if (animateLight)
    {
        float rotationAngle = glm::radians(10.0f * m_time.delta);
        float _x = light->get_position().x * cos(rotationAngle) - light->get_position().z * sin(rotationAngle);
        float _z = light->get_position().x * sin(rotationAngle) + light->get_position().z * cos(rotationAngle);

        light->set_position({_x, light->get_position().y, _z});
    }
    // m_lightDummy->set_position(light->get_position());
    // dynamic_cast<UnlitMaterial *>(m_lightDummy->get_material())->set_color(glm::vec4(light->get_color(), 1.0f));

    m_interface.objectWidget->set_object(m_interface.sceneWidget->get_selected_object());
}

void HairViewer::tick()
{
    float currentTime = (float)Window::get_time_elapsed();
    m_time.delta = currentTime - m_time.last;
    m_time.last = currentTime;
    m_time.framesPerSecond = 1.0f / m_time.delta;

    update();

    m_renderer->render(m_scene);
}
