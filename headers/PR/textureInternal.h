#ifndef PR_TEXTURE_INTERNAL_H
#define PR_TEXTURE_INTERNAL_H

typedef struct prTextureData prTextureData;

void i_prTextureCreateOnGPU(prTextureData* texture);

void i_prTextureDestroyOnGPU(prTextureData* texture);

void i_prTextureUpdateOnGPU(prTextureData* texture);

void i_prTextureUpdateBorderColorOnGPU(prTextureData* texture);

#endif // PR_TEXTURE_INTERNAL_H
