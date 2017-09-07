#include <SFML/Graphics.hpp>
#include <vector>
#include <random>

typedef sf::Vector2f  VI;

sf::Vector2i right = { 1, 0 },
	left = { -1,0 }, 
	up = { 0, -1 },
	down = { 0, 1 };

float lowx = 100, hix = 900, lowy = 200, hiy = 800;

struct Game_Settings {
public:
	bool swap_balls = true;
} settings;


struct Maps {
public:
	const int H = 12;
	const int W = 45;

	int tileSizeW = 32;
	int tileSizeH = 32;
	sf::RectangleShape tilerect;

	std::string TileMap[12] = {
		"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
		"BB                                         BB",
		"BB                                         BB",
		"BB                  SS5      BB            BB",
		"BB                           BB            BB",
		"BB          BB                             BB",
		"BB          BB                             BB",
		"BB                  SS                     BB",
		"BB                  BB                     BB",
		"BB                              BB         BB",
		"BB                              BB         BB",
		"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB"
		 };

} game_maps;

class ball;


class ball
{
public:
	typedef sf::Vector2f  VI;
	typedef sf::Color  CC;
	typedef float V;
	typedef std::vector <CC> CVC;

	VI pos;
	VI vel;
	sf::CircleShape shape;
	V radius;
	int colorindex;
	CVC* colorsvec;

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

	ball(int ci, CVC &cv) :
		pos({ 10,10 }), vel({ 2.5f,2.1f }), shape(55.f), radius(55.f) {
		shape.setOrigin({ shape.getRadius(), shape.getRadius() });

		colorsvec = &cv;
		colorindex = ci;
		CC c0 = (*colorsvec)[colorindex];
		shape.setFillColor(c0);
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

	void next_color() {
		if (colorsvec) {
			++colorindex;
			CC c0 = (*colorsvec)[colorindex % (colorsvec->size())];
			shape.setFillColor(c0);
		}
	}

	bool check_in(VI dot) {
		return (len2(pos - dot) < sqr(radius));
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
		vel.y += 0.1f;

		//right
		if (check_x() == 1) {
			// .x > 400
			pos.x = hix-radius-1;
			vel.x = -vel.x;
			vel.y = vel.y/2.f;
		}
		//left
		if (check_x() == -1) {
			pos.x = lowx+radius+1;
			vel.x = -vel.x;
			vel.y = vel.y / 2.f;
		}
		// down
		if (check_y() == 1) {
			pos.y = hiy-radius-1;
			vel.y = -vel.y / 2.f;
			//vel.x *= 0.9;
		}
		// up
		if (check_y() == -1) {
			pos.y = lowy+radius+3;
			vel.y = -vel.y;
			//vel.x *= 0.9;
		}



		//if  (pos.x + radius )
		
		if (std::abs(vel.x) < 0.01)
			vel.x = 0;
		if (std::abs(vel.y) < 0.01)
			vel.y = 0;
		shape.setPosition(pos);
	}

};

template <typename T>
T operator* (sf::Vector2<T> a, sf::Vector2<T> b) {
	return a.x*b.x+a.y*b.y;
}

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

		for (size_t i = 0; i < ballrefs.size(); ++i) {
			bref1 = ballrefs[i];

			for (size_t j = i + 1; j < ballrefs.size(); ++j) {
				bref2 = ballrefs[j];
				
				if (bref1->check_collision(*bref2) == 1) {
					//auto c1 = bref1->shape.getFillColor();
					//auto c2 = bref2->shape.getFillColor();
					
					//bref1->shape.setFillColor(c2);
					//bref2->shape.setFillColor(c1);
					
					auto pos1 = bref1->pos;
					auto pos2 = bref2->pos;

					auto r1 = bref1->radius;
					auto r2 = bref2->radius;

					auto dpos = (pos2 - pos1) ;
					auto center = pos1 + dpos * 0.5f;
					
					//auto dpos_len2 = dpos.x*dpos.x + dpos.y*dpos.y;
					auto dpos_len2 = dpos * dpos;
					auto dpos_len = std::sqrt(dpos_len2);
					//if (dpos_len2 > 0.01f) 
					{
						auto dshift_koeff = (r1 + r2) / dpos_len;
						
						auto vel_save_k = 0.5f;

						auto v1 = bref1->vel;
						auto v2 = bref2->vel;
						////  float            (v1^2 + v2^2) = 2 e^2, 
						auto energy = std::sqrt((v1*v1 + v2*v2) / 2.f);
						//auto energy = std::sqrt(((v1.x*v1.x + v1.y*v1.y) + (v2.x*v2.x+v2.y*v2.y)) / 2.f);

						auto norm_dpos = dpos / dpos_len;
						auto real_dpos =  dpos * dshift_koeff * 0.5f;

						auto np1  = center + real_dpos;
						auto np2 = center - real_dpos;

						auto nv1 = v1 * vel_save_k + norm_dpos * energy * (1 - vel_save_k);
						auto nv2 = v2 * vel_save_k - norm_dpos * energy * (1 - vel_save_k);


						if (settings.swap_balls) {
							bref1->pos = np1;
							bref2->pos = np2;

							bref1->vel = nv1;
							bref2->vel = nv2;
						}
						else {
							bref1->pos = np2;
							bref2->pos = np1;

							bref1->vel = nv2;
							bref2->vel = nv1;
						}

					}

					//auto v1 = bref1->vel;
					//auto v2 = bref2->vel;
					
					//bref1->vel = v1 * 0.5f + v2 * 0.5f;
					//bref2->vel = v1 * 0.5f + v2 * 0.5f;

					//bref1->vel = float(-2)*bref1->vel;
					//bref2->vel = float(-2)*bref2->vel;
				}
			}
		}
	}

	void interact_dot(VI dot) {
		for (ball* bref : ballrefs)
			if (bref->check_in(dot))
				bref->next_color();
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

	//ball myball;
	//ball oball({ 259,257 }, { 1,-1 }, 25.f);
	//ball magball(sf::Color::Magenta);
	
	balls game_balls;
	//game_balls.ballrefs = { &myball, &oball, &magball };

	std::vector <sf::Color> prettycolors = { sf::Color::Black , sf::Color::White ,
		sf::Color::Red , sf::Color::Green ,sf::Color::Blue ,
		sf::Color::Yellow , sf::Color::Magenta , sf::Color::Cyan , sf::Color::Transparent };

	for (int i = 0; i < 3; ++i) {
		//ball* x = new ball(sf::Color::Cyan);
		//x->shape.setFillColor(prettycolors[i%9]);
		ball* x = new ball(i%8+1, prettycolors);
		x->pos = { float(i*150+lowx),float(lowy) };
		x->vel = { float(i ),float(i+1) };
		game_balls.ballrefs.push_back(x);
	}
	ball* my_ball = game_balls.ballrefs[0];

	bool mouse_flag = false;

	bool touch_flag = false;

	while (window.isOpen())
	{
		
		
		VI dot;

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::MouseButtonPressed) {
				mouse_flag = true;
			}
			if (event.type == sf::Event::MouseButtonReleased) {
				mouse_flag = false;
			}

			if (event.type == sf::Event::TouchBegan) {
				touch_flag = true;
			}
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::P)) {
				settings.swap_balls = !settings.swap_balls;
			}
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Num1)) {
				game_maps.tileSizeW *= 2;
				game_maps.tileSizeH *= 2;
			}
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Num2)) {
				game_maps.tileSizeW /= 2;
				game_maps.tileSizeH /= 2;
			}

		}


		/*
		if (sf::Touch::isDown(0) || sf::Touch::isDown(1) || sf::Touch::isDown(2)) {
			shape.setRadius(50.f);
			shape.setFillColor(sf::Color::Magenta);
		}
		*/
		

		if (mouse_flag) {
			dot = VI(sf::Mouse::getPosition(window));
			game_balls.interact_dot(dot);
			shape.setFillColor(sf::Color::Green);
		}
		else {
			if (settings.swap_balls)
				shape.setFillColor(sf::Color::Red);
			else 
				shape.setFillColor(sf::Color::White);
		}

		if (touch_flag) {
			board_bg.setFillColor(sf::Color::Magenta);
		}


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			//window.setPosition(window.getPosition() + left);
			my_ball->vel += VI(left);
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			//window.setPosition(window.getPosition() + right);
			my_ball->vel += VI(right);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			//window.setPosition(window.getPosition() + up);
			my_ball->vel += VI(up);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			//window.setPosition(window.getPosition() + down);
			my_ball->vel += VI(down);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			game_balls.ballrefs.erase(game_balls.ballrefs.begin(), game_balls.ballrefs.end());
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			window.close();
		}
		if (sf::Touch::isDown(0))
		{
			shape.setFillColor(sf::Color::Green);
		}
		/*
		if (sf::Touch::isDown(1)) {
			auto c = (game_balls.ballrefs[0])->shape.getFillColor();
			for (auto br : game_balls.ballrefs)
				br->shape.setFillColor(c);
		}
		*/
		// get global position of touch 1
		//sf::Vector2i globalPos = sf::Touch::getPosition(1);

		// get position of touch 1 relative to a window
		//sf::Vector2i relativePos = sf::Touch::getPosition(1, window);

		my_ball->shape.setOutlineThickness ( 4.f);
		my_ball->shape.setOutlineColor(sf::Color::Magenta);

		game_balls.clash();

		game_balls.update();

		window.clear();
		
		window.draw(board_bg);

		game_maps.tilerect.setSize(sf::Vector2f(game_maps.tileSizeW, game_maps.tileSizeH));
		
		for (int i = 0; i < game_maps.W; ++i) 
			for (int j = 0; j < game_maps.H; ++j) 
			{
				if (game_maps.TileMap[j][i] == 'B')
					game_maps.tilerect.setFillColor(sf::Color::White);
				else if (game_maps.TileMap[j][i] == 'S') 
					game_maps.tilerect.setFillColor(sf::Color::Blue);
				else 
					game_maps.tilerect.setFillColor(sf::Color::Black);
				game_maps.tilerect.setPosition(i*game_maps.tileSizeW,  j * game_maps.tileSizeH);

				window.draw(game_maps.tilerect);
			}

		game_balls.draw(window);
		
		window.draw(shape);

		window.display();
	}

	return 0;
}
