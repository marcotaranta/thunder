#include "postprocess/antialiasing.h"

#include "engine.h"

#include "pipeline.h"
#include "material.h"

AntiAliasing::AntiAliasing() {
    Material *material = Engine::loadResource<Material>(".embedded/AntiAliasing.mtl");
    if(material) {
        m_pMaterial = material->createInstance();
    }
}
