#ifndef ComplexPlane_H
#define ComplexPlane_H
#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>
#include <complex>
#include <cmath>

using namespace std;
const unsigned int MAX_ITER = 64;
const float BASE_WIDTH = 4.0;
const float BASE_HEIGHT = 4.0;
const float BASE_ZOOM = 0.5;
class ComplexPlane : public sf:: Drawable
{
public:
	ComplexPlane(int pixelWidth, int pixelHeight); //constructor
	void draw(RenderTarget& target, RenderStates states) const;
	void zoomIn();
	void zoomOut();
	void setCenter(Vector2i mousePixel);
	void setMouseLocation(Vector2i mousePixel);
	void loadText(Text& text);
	void updateRender();
private:
	int countIterations(Vector2f coord);
	void iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b);
	Vector2f mapPixelToCoords(Vector2i mousePixel);
	
	//variables
	VertexArray m_vArray;
	State m_state;
	Vector2f m_mouseLocation;
	Vector2f m_plane_center;
	Vector2f m_plane_size;
	int m_zoomCount;
	float m_aspectRatio;
	float m_pixelWidth;
	float m_pixelHeight;
};
enum class State
{
	CALCULATING, DISPLAYING
};
#endif
