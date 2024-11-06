#include "application.h"
#include <iostream>

int main(int argc, char* argv[]) {

    HairViewer app;
    try
    {
        Systems::RendererSettings settings{};
        settings.samplesMSAA = MSAASamples::MSAA_x8;
        settings.clearColor  = Vec4(0.02, 0.02, 0.02, 1.0);
        settings.enableUI    = true;
        // settings.renderingType = RendererType::TFORWARD;
        // settings.shadowResolution = ShadowResolution::MEDIUM;

        if (argc == 1)
            std::cout << "No arguments submitted, initializing with default parameters..." << std::endl;

        for (int i = 0; i < argc; ++i)
        {
            std::string token(argv[i]);
            if (token == "-type")
            {
                if (i + 1 >= argc)
                {
                    std::cerr << "\"-type\" argument expects a rendering type keyword:" << std::endl;
                    std::cerr << "[forward]" << std::endl;
                    std::cerr << "[deferred]" << std::endl;
                    return EXIT_FAILURE;
                }
                std::string type(argv[i + 1]);

                if (type == "forward")
                {
                    // settings.renderingType = RendererType::TFORWARD;
                    i++;
                    continue;
                }
                if (type == "deferred")
                {
                    // settings.renderingType = RendererType::TDEFERRED;
                    i++;
                    continue;
                }

                std::cerr << "\"--type\" invalid argument:" << std::endl;
                std::cerr << "[forward]" << std::endl;
                std::cerr << "[deferred]" << std::endl;
                return EXIT_FAILURE;
            } else if (token == "-aa")
            {
                if (i + 1 >= argc)
                {
                    std::cerr << "\"-aa\" argument expects an antialiasing type keyword:" << std::endl;
                    std::cerr << "[none]" << std::endl;
                    std::cerr << "[msaa4]" << std::endl;
                    std::cerr << "[msaa8]" << std::endl;
                    std::cerr << "[fxaa]" << std::endl;
                    return EXIT_FAILURE;
                }
                std::string aaType(argv[i + 1]);
                if (aaType == "none")
                    settings.samplesMSAA = MSAASamples::_NONE;
                if (aaType == "msaa4")
                    settings.samplesMSAA = MSAASamples::MSAA_x4;
                if (aaType == "msaa8")
                    settings.samplesMSAA = MSAASamples::MSAA_x8;
                // if (aaType == "fxaa"){}
                //     settings.AAtype = AntialiasingType::FXAA;

                i++;
                continue;
            } else if (token == "-gui")
            {
                if (i + 1 >= argc)
                {
                    std::cerr << "\"-gui\" argument expects an enabling gui type keyword:" << std::endl;
                    std::cerr << "[false]" << std::endl;
                    std::cerr << "[true]" << std::endl;
                    return EXIT_FAILURE;
                }
                std::string enableGui(argv[i + 1]);
                if (enableGui == "true")
                    settings.enableUI = true;
                if (enableGui == "false")
                    settings.enableUI = false;
                i++;
                continue;
            }
            continue;
        }

        app.run(settings);
    } catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}