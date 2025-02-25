#include <PR/texture.hpp>

#include <stb_image.h>
#include <cstring>

PR::textureData::~textureData() {
    if(!i_textureData) {
        delete[] i_textureData;
    }
}

PR::textureData::textureData(const textureData& original) : i_textureDataCount(original.i_textureDataCount) {
    if(!original.i_textureData) {
        i_textureData = new unsigned char[original.i_textureDataCount];
        for(unsigned int i = 0; i < original.i_textureDataCount; i++) {
            i_textureData[i] = original.i_textureData[i];
        }
    }
}

PR::textureData& PR::textureData::operator=(const textureData& original) {
    if(!original.i_textureData) {
        i_textureData = new unsigned char[original.i_textureDataCount];
        for(unsigned int i = 0; i < original.i_textureDataCount; i++) {
            i_textureData[i] = original.i_textureData[i];
        }
        i_textureDataCount = original.i_textureDataCount;
    }

    return *this;
}

void PR::textureData::updateTexture(unsigned char rawTextureData[], unsigned int rawTextureDataCount) {
    if(!i_textureData) {
        delete[] i_textureData;
    }

    // unsigned char* textureData = stbi_load_from_memory(rawTextureData, rawTextureDataCount, &i_width, &i_height, &i_channels, 3);
    unsigned char* textureData = stbi_load("awesomeface.png", &i_width, &i_height, &i_channels, 3);
    if(textureData[0] == '\0') {
        printf("WE'RE FUCKED!!!!!!!!!!!!!");
        exit(69420);
    }
    unsigned int textureDataCount = strlen((const char*)textureData);

    i_textureData = new unsigned char[textureDataCount];
    for(unsigned int i = 0; i < textureDataCount; i++) {
        i_textureData[i] = textureData[i];
    }
    i_textureDataCount = textureDataCount;
}

void PR::textureData::updateTexture(const std::vector<unsigned char>& rawTextureData) {
    if(!i_textureData) {
        delete[] i_textureData;
    }

    unsigned char* textureData = stbi_load_from_memory(rawTextureData.data(), rawTextureData.size(), &i_width, &i_height, &i_channels, 3);
    unsigned int textureDataCount = strlen((const char*)textureData);

    i_textureData = new unsigned char[textureDataCount];
    for(unsigned int i = 0; i < textureDataCount; i++) {
        i_textureData[i] = textureData[i];
    }
    i_textureDataCount = textureDataCount;
}
