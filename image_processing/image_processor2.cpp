#include <iostream>
#include <string>

#include <cstdlib> 
#include <ctime>

#define STB_IMAGE_IMPLEMENTATION // Place the implementation of stb_image in this file
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION // Place the implementation of stb_image_write in this file
#include "stb_image_write.h"

using std::cout;
using std::endl;

// Image found at https://pixabay.com/no/bloom-blomstre-bud-gjeng-farge-2518/
const std::string IMG_FNAME = "bloom_uncompressed.png";
const std::string IMG_FNAME_MODIFIED = "bloom_new.png";

void convert_to_grayscale(unsigned char* image, const unsigned width, const unsigned height)
{
	unsigned char r, g, b, new_value;
    const unsigned num_bytes = width * height * 3;
	for (size_t i = 0; i < num_bytes; i += 3) {
		r = image[i];
		g = image[i + 1];
		b = image[i + 2];
		// Luminosity method from http://docs.gimp.org/2.6/en/gimp-tool-desaturate.html
		new_value = 0.21 * r + 0.72 * g + 0.07 * b;

		image[i] = new_value;
		image[i + 1] = new_value;
		image[i + 2] = new_value;
	}
}

int main(int argc, char const *argv[])
{
	int width, height, num_channels;
	cout << "Starting decode" << endl;
	unsigned char* image = stbi_load(IMG_FNAME.c_str(), &width, &height, &num_channels, 0);

	if(image == NULL) {
		cout << "Decoder error" << endl;
		return -1;
	}
    if(num_channels != 3) {
        cout << "Wrong number of channels" << endl;
        return -1;
    }

	clock_t before = clock();
	cout << "Starting grayscale_conversion" << endl;
	convert_to_grayscale(image, width, height);
	clock_t after = clock();
	double elapsed_secs = double(after - before) / CLOCKS_PER_SEC;

	cout << "Starting encode" << endl;
	int success = stbi_write_png(IMG_FNAME_MODIFIED.c_str(), width, height, num_channels, image, 0);

	if(!success) {
		cout << "Encoder error" << endl;
		return -1;
	}

	cout << "Processor time used to convert to grayscale: " << elapsed_secs << endl;
	return 0;
}
