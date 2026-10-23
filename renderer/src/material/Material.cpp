#include "Material.h"

void Material::bind() const {
    shader.setActive();
}
