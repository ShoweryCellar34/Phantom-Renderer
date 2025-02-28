#pragma once

#include <PR/texture.h>

void i_prTextureCreateOnGPUSide(prTextureData* texture);

void i_prTextureUpdateOnGPUSide(prTextureData* texture);

void i_prTextureDestroyOnGPUSide(prTextureData* texture);
