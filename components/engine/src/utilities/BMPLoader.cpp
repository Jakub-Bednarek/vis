//
// Created by BlackFlage on 20.11.2021.
//

#include "BMPLoader.h"
#include <fstream>
#include "Logger.h"
#include <Wingdi.h>
#include "GL/glew.h"
#include "Macro.h"

#define BMP_ID 0x4D42

namespace vis
{
    uint8_t* BMPLoader::load_from_file(const std::string &a_file_path, int* a_width, int* a_height)
    {
        uint8_t* dataBuffer[2] = {nullptr, nullptr};
        uint8_t* bitmapImage = nullptr;
        BITMAPFILEHEADER* bitmapFileHeader;
        BITMAPINFOHEADER* bitmapInfoHeader;

        std::ifstream file(a_file_path, std::ios::binary);
        if(!file)
        {
            LOG_ERROR("Invalid file path: " + a_file_path);
            return nullptr;
        }

        dataBuffer[0] = new uint8_t[sizeof(BITMAPINFOHEADER)];
        dataBuffer[1] = new uint8_t[sizeof(BITMAPINFOHEADER)];

        file.read((char*)dataBuffer[0], sizeof(BITMAPFILEHEADER));
        file.read((char*)dataBuffer[1], sizeof(BITMAPINFOHEADER));
        bitmapFileHeader = (BITMAPFILEHEADER*)dataBuffer[0];
        bitmapInfoHeader = (BITMAPINFOHEADER*)dataBuffer[1];

        if(bitmapFileHeader->bfType != (int)BMP_ID)
        {
            LOG_ERROR("Invalid file type! Expected BMP file.");
            return nullptr;
        }

        bitmapImage = new uint8_t[bitmapInfoHeader->biSizeImage];

        file.seekg(bitmapFileHeader->bfOffBits, file.beg);
        file.read(reinterpret_cast<char*>(bitmapImage), bitmapInfoHeader->biSizeImage);

        *a_width = bitmapInfoHeader->biWidth;
        *a_height = std::abs(bitmapInfoHeader->biHeight);

        if(!bitmapInfoHeader->biSizeImage)
        {
            bitmapInfoHeader->biSizeImage = (*a_width) * *(a_height) * 3;
        }
        uint8_t tempPixel;
        for(int i = 0; i < bitmapInfoHeader->biSizeImage; i += 3) {
            tempPixel = bitmapImage[i];
            bitmapImage[i] = bitmapImage[i + 2];
            bitmapImage[i + 2] = tempPixel;
        }

        file.close();
        delete[] dataBuffer[0];
        delete[] dataBuffer[1];

        return bitmapImage;
    }
}
