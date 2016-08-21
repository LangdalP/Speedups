#include <iostream>
#include <string>
#include <chrono>

#include "lodepng.h"

using std::cout;
using std::endl;

// Image found at https://pixabay.com/no/bloom-blomstre-bud-gjeng-farge-2518/
// const std::string IMG_FNAME = "bloom_uncompressed.png";
// const std::string IMG_FNAME = "bloom_small_compressed.png";
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
	// unsigned int blur_sum = 0;
	unsigned int start_coordinate = filterSize / 2;
	unsigned int end_x = width - start_coordinate;
	unsigned int end_y = height - start_coordinate;

	for (unsigned int x = start_coordinate; x < end_x; ++x) {
		for (unsigned int y = start_coordinate; y < end_y; ++y) {
			const auto i = (y * width + x) * 3;
			unsigned int blur_sum_r = 0;
			unsigned int blur_sum_g = 0;
			unsigned int blur_sum_b = 0;
			const unsigned int sum_start_x = x - start_coordinate;
			const unsigned int sum_start_y = y - start_coordinate;
			const unsigned int sum_end_x = x + start_coordinate;
			const unsigned int sum_end_y = y + start_coordinate;

			for (unsigned int i = sum_start_x; i <= sum_end_x; ++i) {
				for (unsigned int j = sum_start_y; j <= sum_end_y; ++j) {
					const auto sum_i = (j * width + i) * 3;
					blur_sum_r += imageIn[sum_i];
					blur_sum_g += imageIn[sum_i + 1];
					blur_sum_b += imageIn[sum_i + 2];
				}
			}

			imageOut[i] = blur_sum_r / (filterSize * filterSize);
			imageOut[i + 1] = blur_sum_g / (filterSize * filterSize);
			imageOut[i + 2] = blur_sum_b / (filterSize * filterSize);
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
