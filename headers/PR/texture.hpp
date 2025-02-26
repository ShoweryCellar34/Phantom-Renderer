#pragma once

#include <vector>

namespace PR {
    struct textureData {
    public:
        unsigned char* i_textureData = nullptr;
        int i_width = 0, i_height = 0, i_channels = 0;

        textureData() = default;
        ~textureData();
        textureData(const textureData& original);
        textureData& operator=(const textureData& original);

        void updateTexture(unsigned char rawTextureData[], unsigned int rawTextureDataCount);

        void updateTexture(const std::vector<unsigned char>& rawTextureData);
    };
}
