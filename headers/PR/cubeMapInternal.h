#pragma once

typedef struct prCubeMapData prCubeMapData;

void i_prCubeMapCreateOnGPU(prCubeMapData* cubeMap);

void i_prCubeMapDestroyOnGPU(prCubeMapData* cubeMap);

void i_prCubeMapUpdateAllOnGPU(prCubeMapData* cubeMap);

void i_prCubeMapUpdateOnGPU(prCubeMapData* cubeMap, int side);

void i_prCubeMapUpdateBorderColorOnGPU(prCubeMapData* cubeMap);
