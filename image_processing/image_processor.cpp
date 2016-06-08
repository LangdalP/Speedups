#include <iostream>
#include <string>

#include "lodepng.h"

using std::cout;
using std::endl;

const std::string IMG_FNAME = "digitalis_purpurea.png";
const std::string IMG_FNAME_MODIFIED = "digitalis_purpurea_new.png";

void convert_to_grayscale(std::vector<unsigned char>& image)
{
	unsigned char r, g, b, new_value;
	for (size_t i = 0; i < image.size(); i += 3) {
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
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, IMG_FNAME, LCT_RGB);

	if(error) {
		cout << "Decoder error " << error << ": " << lodepng_error_text(error) << endl;
		return -1;
	}

	convert_to_grayscale(image);

	error = lodepng::encode(IMG_FNAME_MODIFIED, image, width, height, LCT_RGB);

	if(error) {
		cout << "Encoder error " << error << ": "<< lodepng_error_text(error) << endl;
	}
	return 0;
}
