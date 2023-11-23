#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>
#include <complex>
#include <cmath>
#include "ComplexPlane.h"

using namespace sf;
using namespace std;

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight)
{
	m_pixelWidth = pixelWidth;
	m_pixelHeight = pixelHeight;
	m_aspectRatio = static_cast<float>(pixelHeight) / pixelWidth;
	m_plane_center = { 0,0 };
	m_plane_size = { BASE_WIDTH, BASE_HEIGHT * m_aspectRatio };
	m_zoomCount = 0;
	m_state = State::CALCULATING;
	m_vArray = VertexArray(Points, pixelWidth * pixelHeight);
}
void ComplexPlane::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(m_vArray);
}
void ComplexPlane::zoomIn()
{
	//increment zoomCount
	m_zoomCount++;
	//find new size
	float newX = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
	float newY = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
	// update plane size
	m_plane_size = Vector2f(newX, newY);
	//set state to calculating
	m_state = State::CALCULATING;
}
void ComplexPlane::zoomOut()
{
	m_zoomCount--;
	//find new size
	float newX = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
	float newY = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
	// update plane size
	m_plane_size = Vector2f(newX, newY);
	//set state to calculating
	m_state = State::CALCULATING;
}
void ComplexPlane::setCenter(Vector2i mousePixel)
{
	//get vector of mouse postion
	Vector2f newCenter = mapPixelToCoords(mousePixel);
	//set to center
	m_plane_center = newCenter;
	// set state to calculating
	m_state = State::CALCULATING;

}
void ComplexPlane::setMouseLocation(Vector2i mousePixel)
{
	//setting mouse locationg
	m_mouseLocation = mapPixelToCoords(mousePixel);
}
void ComplexPlane::loadText(Text& text)
{
	ostringstream out;
	out << "Mandelbrot Set" << endl;
	out << "Center: (" << m_plane_center.x << ", " << m_plane_center.y << ")" << endl;
	out << "Cursor: (" << m_mouseLocation.x << ", " << m_mouseLocation.y << ")" << endl;
	out << "Left-click to Zoom in" << endl;
	out << "Reft-click to Zoom out" << endl;
	text.setString(out.str());
}
int ComplexPlane::countIterations(Vector2f coord)
{
	//set up varaibles
	complex<float> z(0, 0);
	complex<float> c(coord.x, coord.y);
	int i = 0;
	int maxIterations = 64;
	//formula
	while (abs(z) <= 2.0 && i < maxIterations)
	{
		z = z * z + c;
		i++;
	}
	return i;
}
void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b)
{
	const int maxIterations = 64; 

	// set the iteration count to a range [0, 1]
	double defaultCount = static_cast<double>(count) / maxIterations;

	if (count >= maxIterations) {
		r = g = b = 0; //set to black at maxIterations
	}
	Uint8 gray = static_cast<Uint8>((255.0 * count) / maxIterations); //setting to grey
	r = g = b = gray;
	//dividing into 5 regions
	const size_t regionSize = maxIterations / 5;
	size_t region = count / regionSize;
	//color adjustment
	Uint8 colorAdjustment = static_cast<Uint8>((count % regionSize) * (255 / regionSize));
	switch (region) {
	case 0: // black 44,38,36
		r = 44;
		g = 38;
		b = colorAdjustment;
		break;
	case 1: // blueish green 37,107,110
		r = 37;
		g = colorAdjustment;
		b = 110;
		break;
	case 2: // orange 242,99,27
		r = 242;
		g = 99; 
		b = 27;
		break;
	case 3: // Yellow
		r = colorAdjustment; 
		g = 0;
		b = 0;
		break;
	case 4: // Red.
		r = gray;
		g = 0;
		b = 0;
		break;
	}
}
Vector2f ComplexPlane :: mapPixelToCoords(Vector2i mousePixel)
{
	// The screen size
	float width = VideoMode::getDesktopMode().width;
	float height = VideoMode::getDesktopMode().height;
	Vector2f screenSize = { width, height};

	// The default range of the display pixels
	Vector2f pixelRange = { 0, 0 };
	Vector2f pixelRangeMax = { screenSize.x, screenSize.y };

	// Map the pixel location on the screen to a coordinate in the complex plane
	Vector2f complexCoords;
	complexCoords.x = ((mousePixel.x - pixelRange.x) / (pixelRangeMax.x - pixelRange.x)) * (m_plane_size.x) + (m_plane_center.x - m_plane_size.x / 2.0);
	complexCoords.y = ((mousePixel.y - pixelRange.y) / (pixelRangeMax.y - pixelRange.y)) * (m_plane_size.y) + (m_plane_center.y - m_plane_size.y / 2.0);

	return complexCoords;
}
void ComplexPlane::updateRender()
{
	if (m_state == State::CALCULATING)
	{
		for (int i = 0; i < m_pixelHeight; i++)
		{
			for (int j = 0; j < m_pixelWidth; j++)
			{
				//set postion based on coordinates
				m_vArray[j + i * m_pixelWidth].position = { (float)j,(float)i };
				//get complex plane coord
				Vector2i pixelCoords(j, i);
				Vector2f complexCoordinate = mapPixelToCoords(pixelCoords);
				//get number of iteration
				int iterations = countIterations(complexCoordinate);
				//Calculate RGB color
				Uint8 r, g, b;
				iterationsToRGB(iterations, r, g, b);
				// set color to vertex
				m_vArray[j + i * m_pixelWidth].color = { r,g,b };

			}
		}
		//state is now displaying
		m_state = State::DISPLAYING;
	}
}
