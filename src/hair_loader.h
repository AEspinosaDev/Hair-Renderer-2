
#ifndef __HAIR_LOADERS__
#define __HAIR_LOADERS__

#include <engine/tools/loaders.h>

USING_VULKAN_ENGINE_NAMESPACE

namespace hair_loaders {
void load_neural_hair(Core::Mesh* const mesh,
                      const char*       fileName,
                      Core::Mesh* const skullMesh,
                      bool              preload           = true,
                      bool              verbose           = false,
                      bool              calculateTangents = false,
                      bool              saveOutput        = false);
}

#endif