/*Load images Texture from the disk*/
#include "GL/freeglut.h"
class TextureLoader {
public:
	/*Recive the texture file Name*/
	TextureLoader(const char* fileName);
	/*Return the texture number loaded*/
	int loadTexture();
	/*Return the image Width*/
	int getWidth();
	/*Return the image Height*/
	int getHeight();
	/*Return the image data*/
	unsigned char* getData();
	/*Set the texture fileName*/
	void setFileName(const char* fileName);
private:
	const char* fileName = "";
	unsigned char* data = NULL;
	int width = 1024;
	int height = 512;
	GLuint texture = 0;
};