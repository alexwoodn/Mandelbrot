#include <SFML/Graphics.hpp>
#include "ComplexPlane.h"

using namespace sf;
using namespace std;

int main() {
    // Get desktop resolution and construct the RenderWindow
    VideoMode desktop = VideoMode::getDesktopMode();
    RenderWindow window(desktop, "Mandelbrot Set Viewer", Style::Default);

    // Construct the ComplexPlane
    ComplexPlane complexPlane(desktop.width / 2, desktop.height / 2); // Using half resolution for performance

    // Construct the Font and Text objects
    Font font;
    if (!font.loadFromFile("fonts/KOMIKAP_.ttf")) {
        // Handle font loading error
        return EXIT_FAILURE;
    }

    Text text;
    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(Color::White);

    // Begin the main loop
    while (window.isOpen()) {
        // Handle Input segment
        Event event;
        while (window.pollEvent(event)) {
            // Handle Event::Closed
            if (event.type == Event::Closed)
                window.close();

            // Handle Event::MouseButtonPressed
            if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Right) {
                    // Right-click to zoom out
                    complexPlane.zoomOut();
                } else if (event.mouseButton.button == Mouse::Left) {
                    // Left-click to zoom in
                    complexPlane.zoomIn();
                }
            }

            // Handle Event::MouseMoved
            if (event.type == Event::MouseMoved) {
                // Set the cursor position
                complexPlane.setMouseLocation(Mouse::getPosition(window));
            }
        }

        // Check for Escape key
        if (Keyboard::isKeyPressed(Keyboard::Escape))
            window.close();

        // Update Scene segment
        complexPlane.updateRender();

        // Load the text into the sf::Text object
        complexPlane.loadText(text);

        // Draw Scene segment
        window.clear();
        window.draw(complexPlane);
        window.draw(text);  // Draw the text
        window.display();
    }

    return 0;
}
