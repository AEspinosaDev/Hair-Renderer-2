
#ifndef __HAIR_LOADERS__
#define __HAIR_LOADERS__

#include <engine/utilities/loaders.h>

USING_VULKAN_ENGINE_NAMESPACE

namespace hair_loaders
{
    void load_neural_hair(Mesh *const mesh, const char *fileName, Mesh *const skullMesh, bool preload = true, bool verbose = false, bool calculateTangents = false, bool saveOutput = false);
}

#endif