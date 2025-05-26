#pragma once

typedef struct prTextureData prTextureData;

void i_prTextureCreateOnGPU(prTextureData* texture);

void i_prTextureDestroyOnGPU(prTextureData* texture);

void i_prTextureUpdateOnGPU(prTextureData* texture);
