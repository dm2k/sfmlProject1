#include <SFML/Graphics.hpp>

sf::Vector2i right = { 1, 0 },
left = { -1,0 }, 
up = { 0, -1 },
down = { 0, 1 };

int lowx = 200, hix = 800, lowy = 200, hiy = 800;

class ball 
{
public:
	typedef sf::Vector2f  VI;
	typedef float V;

	VI pos;
	VI vel;
	sf::CircleShape shape;
	V radius;

	ball() : pos({ 5,5 }), vel({ 1,2 }), shape(10.f), radius(10.f) {
		shape.setFillColor(sf::Color::Cyan);
	};

	ball(VI inpos, VI invel, V inradius) : 
		pos (inpos), vel(invel), shape(inradius), radius(inradius) { 
		shape.setOrigin({ inradius, inradius });
		shape.setFillColor(sf::Color::Yellow);
	}

	int check_x() {
		if (pos.x - radius <= lowx) return -1;
		else if (pos.x + radius > hix) return 1;
		else return 0;
	}

	int check_y() {
		if (pos.y - radius <= lowy) return -1;
		else if (pos.y + radius > hiy) return 1;
		else return 0;
	}

	V len2(VI v) {
		return v.x*v.x + v.y*v.y;
	}
	V dist2(VI a, VI b) {
		return len2(a - b);
	}
	V sqr(V x) {
		return x*x;
	}

	int check_collision(ball & ball2) {
		if (dist2(pos, ball2.pos) < sqr(radius + ball2.radius))
			return 1;
		else
			return 0;
	}

	void grow() {
		++radius;
		shape.setRadius(radius);
	}

	void update() {
		pos += vel;

		if (check_x() == 1) {
			// .x > 400
			pos.x = hix-radius;
			vel.x = -vel.x;
		}
		if (check_x() == -1) {
			pos.x = lowx+radius;
			vel.x = -vel.x;
		}
		if (check_y() == 1) {
			pos.y = hiy-radius;
			vel.y = -vel.y;
		}
		if (check_y() == -1) {
			pos.y = lowy+radius;
			vel.y = -vel.y;
		}

		shape.setPosition(pos);
	}
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);
	window.setFramerateLimit(100);

	ball myball;
	ball oball({ 259,257 }, { 1,-1 }, 25.f);
	



	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::TouchBegan)
				shape.setFillColor(sf::Color::Blue);
		}

		

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			window.setPosition(window.getPosition() + left);
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			window.setPosition(window.getPosition() + right);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			window.setPosition(window.getPosition() + up);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			window.setPosition(window.getPosition() + down);
		}
	

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			shape.setFillColor(sf::Color::Red);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			window.close();
		}
		if (sf::Touch::isDown(0))
		{
			shape.setFillColor(sf::Color::Green);
		}

		// get global position of touch 1
		sf::Vector2i globalPos = sf::Touch::getPosition(1);

		// get position of touch 1 relative to a window
		sf::Vector2i relativePos = sf::Touch::getPosition(1, window);
		
		if (oball.check_collision(myball) == 1) {
			oball.grow();
		}

		if (myball.check_collision(oball) == 1) {
			myball.grow();
		}

		myball.update();
		oball.update();

		window.clear();
		window.draw(shape);
		window.draw(myball.shape);
		window.draw(oball.shape);

		window.display();
	}

	return 0;
}
