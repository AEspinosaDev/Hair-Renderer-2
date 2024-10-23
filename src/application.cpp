#include "application.h"
#include <filesystem>

#define USE_NEURAL_MODELS

void HairViewer::init(Systems::RendererSettings settings)
{
    m_window = new WindowGLFW("Hair Viewer", 1280, 1024);

    m_window->init();

    m_window->set_window_size_callback(
        std::bind(&HairViewer::window_resize_callback, this, std::placeholders::_1, std::placeholders::_2));
    m_window->set_mouse_callback(
        std::bind(&HairViewer::mouse_callback, this, std::placeholders::_1, std::placeholders::_2));
    m_window->set_key_callback(std::bind(&HairViewer::keyboard_callback, this, std::placeholders::_1,
                                         std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));

    m_renderer = new Systems::ForwardRenderer(m_window, settings, {HIGH, false});

    setup();

    m_interface.init(m_window, m_scene, m_renderer);
    //m_renderer->set_gui_overlay(m_interface.overlay);
}

void HairViewer::run(Systems::RendererSettings settings)
{

    init(settings);
    while (!m_window->get_window_should_close())
    {

        // I-O
        m_window->poll_events();

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

    PointLight *light = new PointLight();
    light->set_position({-6.0f, 6.0f, 0.0f});
    light->set_shadow_fov(90.0f);
    light->set_shadow_bias(0.00005f);
    light->set_shadow_near(0.1f);
    light->set_area_of_effect(30.0f);
    light->set_name("PointLight");

    Mesh *lightDummy = new Mesh();
    Tools::Loaders::load_3D_file(lightDummy, ENGINE_MESH_PATH + "sphere.obj", false);
    lightDummy->push_material(new UnlitMaterial());
    lightDummy->set_cast_shadows(false);
    lightDummy->set_name("LightDummy");
    light->add_child(lightDummy);

    m_scene->add(light);

#ifdef USE_NEURAL_MODELS
    load_neural_avatar(RESOURCES_PATH "models/neural_hair_PABLO.ply", RESOURCES_PATH "models/neural_head_PABLO.ply",
                       "Pablo", Vec3{0.04f, 0.027f, 0.015f});
    load_neural_avatar(RESOURCES_PATH "models/neural_hair_ALVARO.ply", RESOURCES_PATH "models/neural_head_ALVARO.ply",
                       "Alvaro", Vec3{0.21f, 0.13f, 0.067f});
    load_neural_avatar(RESOURCES_PATH "models/neural_hair_TONO.ply", RESOURCES_PATH "models/neural_head_TONO.ply",
        "Antonio", Vec3{ 0.3f, 0.21f, 0.1f });
#else
    Mesh *hair = new Mesh();
    hair->load_file(MESH_PATH + "curly.hair", false);
    hair->set_scale(0.053f);
    hair->set_rotation({-90.0, 0.0f, 90.0f});
    HairMaterial *hmat = new HairMaterial();
    hmat->set_base_color(Vec3{0.21f, 0.13f, 0.067f});
    hair->set_material(hmat);
    hair->set_name("Hair");
    m_scene->add(hair);

    Mesh *head = new Mesh();
    head->load_file(MESH_PATH + "woman.ply");
    head->set_rotation({0.0, 270.0f, 180.0f});
    auto headMat = new PhysicallyBasedMaterial();
    headMat->set_albedo(Vec3{0.21f, 0.18f, 0.085f});
    headMat->set_metalness(0.0f);
    headMat->set_roughness(0.4f);
    head->set_material(headMat);
    head->set_name("Head");
    m_scene->add(head);
#endif

m_scene->set_ambient_color({0.2, 0.2, 0.2});
m_scene->set_ambient_intensity(0.0f);

m_controller = new Tools::Controller(camera, m_window);
}

void HairViewer::update()
{
    if (!m_interface.overlay->wants_to_handle_input())
        m_controller->handle_keyboard( 0, 0, m_time.delta);

    // Rotate the vector around the ZX plane
    auto light = m_scene->get_lights()[0];
    if (animateLight)
    {
        float rotationAngle = glm::radians(10.0f * m_time.delta);
        float _x = light->get_position().x * cos(rotationAngle) - light->get_position().z * sin(rotationAngle);
        float _z = light->get_position().x * sin(rotationAngle) + light->get_position().z * cos(rotationAngle);

        light->set_position({_x, light->get_position().y, _z});
        static_cast<UnlitMaterial *>(static_cast<Mesh *>(light->get_children().front())->get_material(0))
            ->set_color({light->get_color(), 1.0f});
    }

    m_interface.objectWidget->set_object(m_interface.sceneWidget->get_selected_object());
}

void HairViewer::tick()
{
    float currentTime = (float)m_window->get_time_elapsed();
    m_time.delta = currentTime - m_time.last;
    m_time.last = currentTime;
    m_time.framesPerSecond = 1.0f / m_time.delta;

    update();

    m_interface.overlay->render();
    m_renderer->render(m_scene);
}
void HairViewer::load_neural_avatar(const char *hairFile, const char *headFile, const char *objName, Vec3 hairColor, Vec3 position)
{

    Mesh *hair = new Mesh();
    std::thread loadThread1(hair_loaders::load_neural_hair, hair, hairFile, nullptr, true, false, false, false);
    loadThread1.detach();
    
   
    HairMaterial *hmat = new HairMaterial();
    hmat->set_base_color(hairColor);
    hair->push_material(hmat);
    hair->set_name(std::string(objName) + " hair");

    const std::string HEAD_PATH(headFile);
    Mesh *head = new Mesh();
    Tools::Loaders::load_3D_file(head, HEAD_PATH);

    //Transform
    head->set_position(position);
    head->set_scale(3.f);
    head->set_rotation({-90.0, 0.0f, 215.0f}); // Correct blender axis

    auto headMat = new PhysicallyBasedMaterial();
    headMat->set_albedo(Vec3{0.21f, 0.18f, 0.085f});
    headMat->set_metalness(0.0f);
    headMat->set_roughness(0.4f);
    head->push_material(headMat);
    head->set_name(std::string(objName) + " head");

    head->add_child(hair);
    m_scene->add(head);
}
