using namespace std;

class Pixel
{
	int x;
	int y;
	int rgb[3]; // r = 0; g = 1; b = 2
public:
	Pixel(int x = 0, int y = 0) {
			this->x = x;
			this->y = y;
			rgb[0] = 0;
			rgb[1] = 0;
			rgb[2] = 0;
	}

	void setColor(int red, int green, int blue) {
		rgb[0] = red;
		rgb[1] = green;
		rgb[2] = blue;
	}

	void setCordinates(int x, int y) {
		this->x = x;
		this->y = y;
	}
};