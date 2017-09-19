#include <SFML/Graphics.hpp>
#include <vector>
#include <random>

#define tileSizeX tileRadius;
#define tileSizeY tileRadius;


typedef sf::Vector2f  VI;

template <typename T>
T operator* (sf::Vector2<T> a, sf::Vector2<T> b) {
	return a.x*b.x + a.y*b.y;
}


sf::Vector2i right = { 1, 0 },
	left = { -1,0 }, 
	up = { 0, -1 },
	down = { 0, 1 };

float lowx = 100, hix = 900, lowy = 200, hiy = 800;

struct Game_Settings {
public:
	bool swap_balls = false;
} settings;


struct Maps {
public:
	static const int map_sizeY = 30;
	// const int map_sizeX = 45;
	int tileRadius = 512;
	// int tileSizeX = 512;
	// int tileSizeY = 512;
	sf::CircleShape tilerect;
	std::vector <sf::CircleShape> Tilemap_bounds;

	int map_lenX() {
		return TileMap[0].length();
	}

	std::string TileMap[map_sizeY] = {
		" BB                       BBB           BBBBB",
		"  BB                    BBBB              BBB",
		"                                             ",
		"                    SSS                      ",
		" B                           BBBB          BB",
		"            BB               BBBB          BB",
		"B                                            ",
		"                                             ",
		"                                             ",
		" B                                         BB",
		"                                           BB",
		"                                           BB",
		"BB                                           ",
		"                                             ",
		"            BB                               ",
		"B                   SSS                    BB",
		"                                           BB",
		"B                                          BB",
		"                                           BB",
		"                                           BB",
		"                    BBB        BBBB          ",
		"                               BBBB          ",
		"                                             ",
		"BB                                           ",
		"BB                                           ",
		"                                             ",
		"BB                                           ",
		"BB                                           ",
		"                    BBBBBB                   ",
		"                    B                 BBBBBBB"
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
	
	sf::Vector2i cell(sf::Vector2f pos) {
		int c = pos.x / game_maps.tileSizeX;
		int d = pos.y / game_maps.tileSizeY;
		c < 0 ? 0 : c;
		c >= game_maps.map_lenX() ? game_maps.map_lenX() - 1 : game_maps.map_lenX();

		d < 0 ? 0 : d;
		d >= game_maps.map_sizeY ? game_maps.map_sizeY - 1 : game_maps.map_sizeY;

		return { c,d };
	}

	char cell_type(sf::Vector2i cell_pos) {
		return game_maps.TileMap[cell_pos.y][cell_pos.x];
	}

	char cell_type(sf::Vector2f pos) {
		return cell_type(cell (pos));
	}
	
	bool is_B (sf::Vector2f pos) {
		return cell_type(pos) == 'B';
	}

	bool Collide(sf::CircleShape a, sf::RectangleShape b) {
		return a.getGlobalBounds().intersects(b.getGlobalBounds());
	}

	void update() {
		pos += {vel.x, 0.f};
		shape.setPosition(pos);
		/*
		for (auto b : game_maps.Tilemap_bounds) {
			if (Collide(shape, b)) {
				pos -= {vel.x, 0.f};
				vel.x = -vel.x;
				shape.setPosition(pos);
			}
		}
		*/

		//auto thecellx = cell(pos


		pos += {0.f, vel.y};
		shape.setPosition(pos);
		/*
		for (auto b : game_maps.Tilemap_bounds) {
			if (Collide(shape, b)) {
				pos -= {0.f, vel.y};
				vel.y = -vel.y;
				shape.setPosition(pos);
			}
		}
		*/

		//pos += vel;
		/*
		auto p_l = pos + sf::Vector2f(left) * radius; 
		auto p_r = pos + sf::Vector2f(right) * radius;

		auto p_u = pos + sf::Vector2f(up) * radius;
		auto p_d = pos + sf::Vector2f(down) * radius;

		auto p_ul = pos + (sf::Vector2f(left) + sf::Vector2f(up)) * radius;
		auto p_ur = pos + (sf::Vector2f(right) + sf::Vector2f(up)) * radius;
		auto p_dl = pos + (sf::Vector2f(left) + sf::Vector2f(down)) * radius;
		auto p_dr = pos + (sf::Vector2f(right) + sf::Vector2f(down)) * radius;



		if (is_B(p_ul) || is_B(p_l) || is_B(p_ur) || is_B(p_r) ||
			is_B(p_dl) || is_B(p_l) || is_B(p_dr) || is_B(p_r) )
		{
			//pos = pos - sf::Vector2f(left) * radius;
			//pos.x / game_maps.tileSizeX;
			pos.x -= vel.x;
			vel.x = -vel.x;
		}


		if (is_B(p_ul) || is_B(p_u) || is_B(p_ur) || is_B(p_u) ||
			is_B(p_dl) || is_B(p_d) || is_B(p_dr) || is_B(p_d))
		{
			pos.y -= vel.y;
			vel.y = -vel.y;
			
			//pos = pos - sf::Vector2f(down) * radius;
		}
		

		shape.setPosition(pos);

		vel *= 0.95f;
		//vel.y += 0.1f;
		if (vel * vel < 0.01f) {
			vel = { 0.f, 0.f };
		}
		*/
	}

};


struct balls {
	std::vector <ball*> ballrefs;


	void draw(sf::RenderWindow &window) {
		for (auto bref : ballrefs) {
			window.draw(bref->shape);
			
			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(bref->pos)),
				sf::Vertex(sf::Vector2f((bref->pos) + 8.f*(bref->vel)))
			};
			window.draw(line, 2, sf::Lines);
		}
			
		
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
	sf::RenderWindow window(sf::VideoMode(1800, 1000), "SFML works!");
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
		x->pos = { float(i*150+lowx+200),float(lowy) };
		x->vel = { float(i ),float(i+1) };
		game_balls.ballrefs.push_back(x);
	}
	/*
	game_maps.tilerect.setRadius(game_maps.tileRadius);
	for (int j = 0; j < game_maps.map_sizeY; ++j) {
		for (int i = 0; i < game_maps.map_lenX(); ++i) {
			if (game_maps.TileMap[j][i] == 'B') {
				game_maps.tilerect.setPosition(game_maps.tileRadius*i, game_maps.tileRadius*j);
				game_maps.Tilemap_bounds.push_back(game_maps.tilerect);
			}
		}
	}
	*/

	ball* my_ball = game_balls.ballrefs[0];

	bool mouse_flag = false;

	bool touch_flag = false;

	sf::View game_view(sf::Vector2f( 500,500), sf::Vector2f( 1800,1000));
	game_view.zoom(4.f);
	window.setView(game_view);


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
				game_view.zoom(2.f);
				//game_maps.tileSizeX *= 2;
				//game_maps.tileSizeY *= 2;
			}
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Num2)) {
				game_view.zoom(0.5f);
				//game_maps.tileSizeX /= 2;
				//game_maps.tileSizeY /= 2;
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

		game_maps.tilerect.setRadius(game_maps.tileRadius);
		
		for (int i = 0; i < game_maps.map_lenX(); ++i) 
			for (int j = 0; j < game_maps.map_sizeY; ++j) 
			{
				if (game_maps.TileMap[j][i] == 'B')
					game_maps.tilerect.setFillColor(sf::Color::White);
				else if (game_maps.TileMap[j][i] == 'S') 
					game_maps.tilerect.setFillColor(sf::Color::Blue);
				else 
					game_maps.tilerect.setFillColor(sf::Color::Black);
				game_maps.tilerect.setPosition(sf::Vector2f(2*i*game_maps.tileRadius,  2*j * game_maps.tileRadius));

				window.draw(game_maps.tilerect);
			}

		game_balls.draw(window);
		
		window.draw(shape);

		game_view.setCenter(my_ball->pos);
		window.setView(game_view);

		window.display();
	}

	return 0;
}
