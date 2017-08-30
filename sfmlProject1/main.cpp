#include <SFML/Graphics.hpp>
#include <vector>
#include <random>

sf::Vector2i right = { 1, 0 },
	left = { -1,0 }, 
	up = { 0, -1 },
	down = { 0, 1 };

float lowx = 100, hix = 900, lowy = 200, hiy = 800;




class ball;


class ball
{
public:
	typedef sf::Vector2f  VI;
	typedef sf::Color  CC;
	typedef float V;
	
	VI pos;
	VI vel;
	sf::CircleShape shape;
	V radius;
	
	ball(VI a, VI b, V r, CC c) : pos(a), vel(b), radius(r), shape(r) {
		shape.setFillColor(c);
		shape.setOrigin(r, r);
	}

	ball() : pos({ 5,5 }), vel({ 1,2 }), shape(40.f), radius(40.f) {
		shape.setFillColor(sf::Color::Cyan);
		shape.setOrigin({ 40.f, 40.f });
	};

	ball(VI inpos, VI invel, V inradius) :
		pos(inpos), vel(invel), shape(inradius), radius(inradius) {
		shape.setOrigin({ inradius, inradius });
		shape.setFillColor(sf::Color::Yellow);
	}

	ball(sf::Color incolor) :
		pos({ 10,10 }), vel({ 2.5f,2.1f }), shape(15.f), radius(15.f) 
	{
		shape.setOrigin({ shape.getRadius(), shape.getRadius() });
		shape.setFillColor ( incolor);
	}

	int check_x() {
		if (pos.x - radius < lowx) return -1;
		else if (pos.x + radius > hix) return 1;
		else return 0;
	}

	int check_y() {
		if (pos.y - radius < lowy) return -1;
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

	int check_collision(ball & ball2) {//depricated
		if (dist2(pos, ball2.pos) < sqr(radius + ball2.radius))
			return 1;
		else
			return 0;
	}


	int operator^(ball & ball2) {
		if (dist2(pos, ball2.pos) < sqr(radius + ball2.radius))
			return 1;
		else
			return 0;
	}


	void swapcolor(ball & oball) {
			auto cmy = shape.getFillColor();
			auto co = oball.shape.getFillColor();
			shape.setFillColor(co);
			oball.shape.setFillColor(cmy);
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

struct balls {
	std::vector <ball*> ballrefs;

	void draw(sf::RenderWindow &window) {
		for (auto bref : ballrefs)
			window.draw(bref->shape);
	}

	void add(ball *bref) {
		ballrefs.push_back(bref);
	}

	void update() {
		for (ball* bref : ballrefs)
			bref->update();
	}

	void clash() {
		ball* bref1;
		ball* bref2;

		for (int i = 0; i < ballrefs.size(); ++i) {
			bref1 = ballrefs[i];

			for (int j = i + 1; j < ballrefs.size(); ++j) {
				bref2 = ballrefs[j];
				
				if (bref1->check_collision(*bref2) == 1) {
					auto c1 = bref1->shape.getFillColor();
					auto c2 = bref2->shape.getFillColor();
					bref1->shape.setFillColor(c2);
					bref2->shape.setFillColor(c1);
				}
			}
		}
	}

};


int main()
{
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	sf::RectangleShape board_bg({ hix - lowx, hiy - lowy });
	board_bg.setSize({ hix - lowx, hiy - lowy });
	board_bg.setPosition({ lowx,lowy });
	board_bg.setFillColor(sf::Color::Black);
	board_bg.setOutlineThickness(5);
	board_bg.setOutlineColor(sf::Color::Red);

	window.setFramerateLimit(50);

	ball myball;
	ball oball({ 259,257 }, { 1,-1 }, 25.f);
	ball magball(sf::Color::Magenta);
	
	balls game_balls;
	game_balls.ballrefs = { &myball, &oball, &magball };

	std::vector <sf::Color> prettycolors = { sf::Color::Black , sf::Color::White ,
		sf::Color::Red , sf::Color::Green ,sf::Color::Blue ,
		sf::Color::Yellow , sf::Color::Magenta , sf::Color::Cyan , sf::Color::Transparent };

	for (int i = 0; i < 15; ++i) {
		ball* x = new ball(sf::Color::Cyan);
		x->shape.setFillColor(prettycolors[i%9]);
		x->pos = { float(i*50+lowx),float(lowy) };
		x->vel = { float(i ),float(i+1) };
		game_balls.ballrefs.push_back(x);
	}

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::TouchBegan)
				shape.setFillColor(sf::Color::Blue);
			if (event.type == sf::Event::TouchMoved)
				shape.setFillColor(sf::Color::Yellow);
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

		if (sf::Touch::isDown(1)) {
			auto c = (game_balls.ballrefs[0])->shape.getFillColor();
			for (auto br : game_balls.ballrefs)
				br->shape.setFillColor(c);
		}

		// get global position of touch 1
		sf::Vector2i globalPos = sf::Touch::getPosition(1);

		// get position of touch 1 relative to a window
		sf::Vector2i relativePos = sf::Touch::getPosition(1, window);
		/*
		if (oball.check_collision(myball) == 1) {
			//oball.grow();
			//if (oball.radius > 100) oball.radius = 10;
			auto cmy = myball.shape.getFillColor();
			auto co = oball.shape.getFillColor();
			myball.shape.setFillColor(co);
			oball.shape.setFillColor(cmy);
		}
		*/
		game_balls.clash();

		game_balls.update();

		window.clear();
		
		window.draw(board_bg);

		game_balls.draw(window);
		
		window.draw(shape);

		/*
		window.draw(myball.shape);
		window.draw(oball.shape);
		window.draw(magball.shape);
		*/

		window.display();
	}

	return 0;
}
