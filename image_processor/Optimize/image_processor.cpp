#include <iostream>
#include <string>
#include <chrono>

#include "lodepng.h"

using std::cout;
using std::endl;

// Image found at https://pixabay.com/no/bloom-blomstre-bud-gjeng-farge-2518/
const std::string IMG_FNAME = "bloom_small.png";
const std::string IMG_FNAME_MODIFIED = "bloom_new.png";

struct image_data {
    std::vector<unsigned char> pixels;
    const unsigned int width;
    const unsigned int height;

    image_data(std::vector<unsigned char> pixels, const unsigned int width, const unsigned int height)
            : width(width), height(height) {
        this->pixels = pixels;
    }
};

image_data load_image_from_png_file(std::string filename)
{
    std::vector<unsigned char> image;
    unsigned width, height;
    unsigned error = lodepng::decode(image, width, height, filename, LCT_RGB);

    if(error) {
        cout << "Decoder error " << error << ": " << lodepng_error_text(error) << endl;
        width = 0; height = 0;
    }
    image_data img_data(image, width, height);
    return img_data;
}

bool write_image_to_png_file(std::string filename, image_data image)
{
    unsigned error = lodepng::encode(filename, image.pixels, image.width, image.height, LCT_RGB);

    if(error) {
        cout << "Encoder error " << error << ": "<< lodepng_error_text(error) << endl;
        return false;
    }
    return true;
}


void apply_blur_filter(
        const std::vector<unsigned char>& imageIn,
        std::vector<unsigned char>& imageOut,
        const unsigned int width,
        const unsigned int height,
        const unsigned int filterSize)
{
    const int start_coordinate = filterSize / 2;
    const int end_x = width - start_coordinate;
    const int end_y = height - start_coordinate;

    std::vector<unsigned int> line_buffer(width * 3);

    // Fill the line buffer
    for (int y = 0; y < (int)filterSize - 1; ++y) {
        for (int x = 0; x < (int)line_buffer.size(); x += 3) {
            const int i = (y * width + x) * 3;
            line_buffer[x] += imageIn[i];
            line_buffer[x + 1] += imageIn[i + 1];
            line_buffer[x + 2] += imageIn[i + 2];
        }
    }

    // Iterate all pixels, one line of pixels at a time
    for (int y = start_coordinate; y < end_y; ++y) {
        for (int x = start_coordinate; x < end_x; ++x) {
            // Add new row to line buffer
            for (int idx = 0; idx < (int)line_buffer.size(); idx += 3) {
                const int i = ((y + filterSize / 2) * width + x) * 3;
                line_buffer[idx] += imageIn[i];
                line_buffer[idx + 1] += imageIn[i + 1];
                line_buffer[idx + 2] += imageIn[i + 2];
            }
            const int i = (y * width + x) * 3;
            unsigned int blur_sum_r = line_buffer[x * 3 - 3] + line_buffer[x * 3] + line_buffer[x * 3 + 3];
            unsigned int blur_sum_g = line_buffer[x * 3 - 2] + line_buffer[x * 3 + 1] + line_buffer[x * 3 + 4];
            unsigned int blur_sum_b = line_buffer[x * 3 - 1] + line_buffer[x * 3 + 2] + line_buffer[x * 3 + 5];

            imageOut[i] = blur_sum_r / (filterSize * filterSize);
            imageOut[i + 1] = blur_sum_g / (filterSize * filterSize);
            imageOut[i + 2] = blur_sum_b / (filterSize * filterSize);

            // Remove last row from line buffer
            for (int idx = 0; idx < (int)line_buffer.size(); idx += 3) {
                const int i = ((y - filterSize / 2) * width + x) * 3;
                line_buffer[idx] -= imageIn[i];
                line_buffer[idx + 1] -= imageIn[i + 1];
                line_buffer[idx + 2] -= imageIn[i + 2];
            }
        }
    }

}

int main(int argc, char const *argv[])
{
    image_data image = load_image_from_png_file(IMG_FNAME);
    if (image.width == 0) return -1;

    std::vector<unsigned char> image2_pixels(image.pixels.size());
    image_data image2(image2_pixels, image.width, image.height);

    using namespace std::chrono;
    auto start_time = high_resolution_clock::now();
    apply_blur_filter(image.pixels, image2.pixels, image.width, image.height, 11);
    auto end_time = high_resolution_clock::now();

    bool success = write_image_to_png_file(IMG_FNAME_MODIFIED, image2);	
    if (!success) return -1;

    cout << "Processor time used to process image: " <<
        duration_cast<microseconds>(end_time - start_time).count() << " microseconds" << endl;
    return 0;
}
