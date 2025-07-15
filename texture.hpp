#ifndef RAYCASTER_TEXTURE
#define RAYCASTER_TEXTURE

struct RGBA {
public:
	RGBA() :r(0), b(0), g(0), a(0) {};
	RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a) :r(r), b(b), g(g), a(a) {
	};
	void clear() {
		r = 0, b = 0, g = 0, a = 0;
	}

private:
	uint8_t r, g, b, a;
};

class Texture {
public:
	Texture();
	~Texture();
private:
	std::vector<RGBA> mTextureData;
	unsigned int stride = 0;
};

#endif