#pragma once

typedef struct prTextureMultisampledData prTextureMultisampledData;

void i_prTextureMultisampledCreateOnGPU(prTextureMultisampledData* texture);

void i_prTextureMultisampledDestroyOnGPU(prTextureMultisampledData* texture);

void i_prTextureMultisampledUpdateOnGPU(prTextureMultisampledData* texture);
