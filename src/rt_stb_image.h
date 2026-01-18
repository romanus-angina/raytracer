#ifndef RT_STB_IMAGE_H
#define RT_STB_IMAGE_H

#ifdef _MSC_VER
    #pragma warning(push, 0)
#endif

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_FAILURE_USERMSG
#include "external/stb_image.h"

#include <cstdlib>
#include <iostream>

class rt_image {
    public:
        rt_image() {}

        rt_image(const char* img_filename) {
            // Loads image by searching sequentially through directories starting with RT_IMAGES/ environment,
            // then the current directory, then in the images/ subdirectory, 
            // then the parent's images/ subdirectory and said parent itself, on and so on upwards
            // for six levels. If no image is found, an error message is printed to stderr(width and height are zero).
            auto filename = std::string(img_filename);
            auto imagedir = getenv("RT_IMAGES");
            
            if(imagedir && load(std::string(imagedir) + "/" + filename)) return;
            if(load(filename)) return;
            if(load("images/" + filename)) return;
            if(load("../images/" + filename)) return;
            if(load("../" + filename)) return;
            if(load("../../images/" + filename)) return;
            if(load("../../" + filename)) return;
            if(load("../../../images/" + filename)) return;
            if(load("../../../" + filename)) return;
            if(load("../../../../images/" + filename)) return;
            if(load("../../../../" + filename)) return;
            if(load("../../../../../images/" + filename)) return;
            if(load("../../../../../" + filename)) return;
            if(load("../../../../../../images/" + filename)) return;
            if(load("../../../../../../" + filename)) return;

            std::cerr << "ERROR: Failed to load image file " << filename << "\n";
        }

        ~rt_image() {
            delete[] bdata;
            STBI_FREE(fdata);
        }

        bool load(const std::string& filename) {
            // Loads the linear(gamma = 1) image data from the given file and returns true on success
            // Data buffer contains three [0.0, 1.0] floats per pixel in RGB order
            // Pixels are contiguosly stored row-wise from top-left to bottom-right
            auto n = bytes_per_pixel;
            fdata = stbi_loadf(filename.c_str(), &img_width, &img_height, &n, bytes_per_pixel);
            if(fdata == nullptr) return false;

            bytes_per_scanline = img_width * bytes_per_pixel;
            convert_to_bytes();
            return true;

        }

        int width() const { return (fdata == nullptr) ? 0 : img_width; }
        int height() const { return (fdata == nullptr) ? 0 : img_height;}

        const unsigned char* pixel_data(int x, int y) const {
            // Returns the address of the three-byte RGB pixel data at (x, y)
            // If there is no data returns magenta
            static unsigned char magenta[] = {255, 0, 255};
            if(bdata == nullptr) return magenta;

            x = clamp(x, 0, img_width);
            y = clamp(y, 0, img_height);
            return bdata + y*bytes_per_scanline + x*bytes_per_pixel;
        }


    private:
        const int bytes_per_pixel = 3;
        int img_width = 0;
        int img_height = 0;
        int bytes_per_scanline = 0;
        float* fdata = nullptr; // Linear floating point pixel data
        unsigned char* bdata = nullptr; // Linear 8-bit pixel data

        static int clamp(int x, int min, int max){
            if(x<min) return min;
            if(x>max) return max-1;
            return x;
        }

        static unsigned char float_to_byte(float value){
            if(value <= 0.0f) return 0;
            if(value >= 1.0f) return 255;
            return static_cast<unsigned char>(value * 256.0f);
        }

        void convert_to_bytes(){
            int total_bytes = img_width * img_height * bytes_per_pixel;
            bdata = new unsigned char[total_bytes];

            auto *fptr = fdata;
            auto *bptr = bdata;

            for(auto i=0; i<total_bytes; i++, bptr++, fptr++){
                *bptr = float_to_byte(*fptr);
            }
        }


};

#ifdef _MSC_VER
    #pragma warning(pop)
#endif

#endif // RT_STB_IMAGE_H
