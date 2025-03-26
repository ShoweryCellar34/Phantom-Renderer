#pragma once

#include <PR/texture.h>

void i_prTextureCreateOnGPU(prTextureData* texture);

void i_prTextureUpdateOnGPU(prTextureData* texture);

void i_prTextureDestroyOnGPU(prTextureData* texture);
