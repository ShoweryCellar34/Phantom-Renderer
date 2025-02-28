#include <PR/texture.hpp>

#include <stb_image.h>
#include <cstring>

PR::textureData::~textureData() {
    if(!i_textureData) {
        delete[] i_textureData;
    }
}

PR::textureData::textureData(const textureData& original) : i_width(original.i_width), i_height(original.i_height), i_channels(original.i_channels) {
    if(!original.i_textureData) {
        unsigned int textureDataCount = original.i_width * original.i_height * original.i_channels;
        i_textureData = new unsigned char[textureDataCount];
        for(unsigned int i = 0; i < textureDataCount; i++) {
            i_textureData[i] = original.i_textureData[i];
        }
    }
}

PR::textureData& PR::textureData::operator=(const textureData& original) {
    i_width = original.i_width;
    i_height = original.i_height;
    i_channels = original.i_channels;
    if(!original.i_textureData) {
        unsigned int textureDataCount = original.i_width * original.i_height * original.i_channels;
        i_textureData = new unsigned char[textureDataCount];
        for(unsigned int i = 0; i < textureDataCount; i++) {
            i_textureData[i] = original.i_textureData[i];
        }
    }

    return *this;
}

void PR::textureData::updateTexture(unsigned char rawTextureData[], unsigned int rawTextureDataCount) {
    if(i_textureData) {
        delete[] i_textureData;
    }

    // unsigned char* textureData = stbi_load_from_memory(rawTextureData, rawTextureDataCount, &i_width, &i_height, &i_channels, 0);
    i_textureData = stbi_load("awesomeface.png", &i_width, &i_height, &i_channels, 0);
}

void PR::textureData::updateTexture(const std::vector<unsigned char>& rawTextureData) {
    if(!i_textureData) {
        delete[] i_textureData;
    }

    unsigned char* textureData = stbi_load_from_memory(rawTextureData.data(), rawTextureData.size(), &i_width, &i_height, &i_channels, 0);
}
