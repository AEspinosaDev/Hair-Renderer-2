#include "hair_loader.h"

void hair_loaders::load_neural_hair(Mesh *const mesh, const char *fileName, Mesh *const skullMesh, bool preload,
                                    bool verbose, bool calculateTangents, bool saveOutput)
{
    std::unique_ptr<std::istream> file_stream;
    std::vector<uint8_t> byte_buffer;
    std::string filePath = fileName;
    try
    {
        if (preload)
        {
            byte_buffer = utils::read_file_binary(filePath);
            file_stream.reset(new utils::memory_stream((char *)byte_buffer.data(), byte_buffer.size()));
        }
        else
        {
            file_stream.reset(new std::ifstream(filePath, std::ios::binary));
        }

        if (!file_stream || file_stream->fail())
            throw std::runtime_error("file_stream failed to open " + filePath);

        file_stream->seekg(0, std::ios::end);
        const float size_mb = file_stream->tellg() * float(1e-6);
        file_stream->seekg(0, std::ios::beg);

        tinyply::PlyFile file;
        file.parse_header(*file_stream);

        if (verbose)
        {
            std::cout << "\t[ply_header] Type: " << (file.is_binary_file() ? "binary" : "ascii") << std::endl;
            for (const auto &c : file.get_comments())
                std::cout << "\t[ply_header] Comment: " << c << std::endl;
            for (const auto &c : file.get_info())
                std::cout << "\t[ply_header] Info: " << c << std::endl;

            for (const auto &e : file.get_elements())
            {
                std::cout << "\t[ply_header] element: " << e.name << " (" << e.size << ")" << std::endl;
                for (const auto &p : e.properties)
                {
                    std::cout << "\t[ply_header] \tproperty: " << p.name
                              << " (type=" << tinyply::PropertyTable[p.propertyType].str << ")";
                    if (p.isList)
                        std::cout << " (list_type=" << tinyply::PropertyTable[p.listType].str << ")";
                    std::cout << std::endl;
                }
            }
        }
        std::shared_ptr<tinyply::PlyData> positions, normals, colors, texcoords, faces, tripstrip;

        // // The header information can be used to programmatically extract properties on elements
        // // known to exist in the header prior to reading the data. For brevity of this sample, properties
        // // like vertex position are hard-coded:
        try
        {
            positions = file.request_properties_from_element("vertex", {"x", "y", "z"});
        }
        catch (const std::exception &e)
        {
            std::cerr << "tinyply exception: " << e.what() << std::endl;
        }

        try
        {
            normals = file.request_properties_from_element("vertex", {"nx", "ny", "nz"});
        }
        catch (const std::exception &e)
        {
            if (verbose)
                std::cerr << "tinyply exception: " << e.what() << std::endl;
        }
        try
        {
            normals = file.request_properties_from_element("vertex", {"normal_x", "normal_y", "normal_z"});
        }
        catch (const std::exception &e)
        {
            if (verbose)
                std::cerr << "tinyply exception: " << e.what() << std::endl;
        }

        utils::ManualTimer readTimer;
        readTimer.start();
        file.read(*file_stream);
        readTimer.stop();

        if (verbose)
        {
            const float parsingTime = static_cast<float>(readTimer.get()) / 1000.f;
            std::cout << "\tparsing " << size_mb << "mb in " << parsingTime << " seconds [" << (size_mb / parsingTime)
                      << " MBps]" << std::endl;

            if (positions)
                std::cout << "\tRead " << positions->count << " total vertices " << std::endl;
            if (normals)
                std::cout << "\tRead " << normals->count << " total vertex normals " << std::endl;
        }

        std::vector<utils::Vertex> vertices;
        vertices.reserve(positions->count);
        std::vector<unsigned int> indices;
        // std::vector<unsigned int> rootsIndices;

        if (positions)
        {
            // rootsIndices.push_back(0); // First index is certainly a root
            const float *posData = reinterpret_cast<const float *>(positions->buffer.get());
            const float *normData = reinterpret_cast<const float *>(normals->buffer.get());
            glm::vec3 color = {((float)rand()) / RAND_MAX, ((float)rand()) / RAND_MAX, ((float)rand()) / RAND_MAX};
            for (size_t i = 0; i < positions->count - 1; i++)
            {
                float x = posData[i * 3];
                float y = posData[i * 3 + 1];
                float z = posData[i * 3 + 2];
                glm::vec3 pos = {x, y, z};

                float nx = normData[i * 3];
                float ny = normData[i * 3 + 1];
                float nz = normData[i * 3 + 2];
                glm::vec3 normal = {nx, ny, nz};

                // Generate hair tangents
                float nextX = posData[(i + 1) * 3];
                float nextY = posData[(i + 1) * 3 + 1];
                float nextZ = posData[(i + 1) * 3 + 2];
                glm::vec3 nextPos = {nextX, nextY, nextZ};
                glm::vec3 tangent = glm::normalize(nextPos - pos);

                vertices.push_back({pos, normal, tangent, {0.0f, 0.0f}, color});
                if (i == positions->count - 2)
                    vertices.push_back({nextPos, normal, tangent, {0.0f, 0.0f}, color});

                // Change STRAND
                if ((i + 1) % 100 != 0)
                {
                    indices.push_back(i);
                    indices.push_back(i + 1);
                }
                else
                {
                    color = {((float)rand()) / RAND_MAX, ((float)rand()) / RAND_MAX, ((float)rand()) / RAND_MAX};
                    // rootsIndices.push_back(i + 1);
                }
            }
        }

        Geometry *g = new Geometry();
        g->fill(vertices, indices);
        // augmentDensity(g, 80000);
        mesh->set_geometry(g);
        mesh->setup_volume();

        return;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Caught tinyply exception: " << e.what() << std::endl;
    }
}