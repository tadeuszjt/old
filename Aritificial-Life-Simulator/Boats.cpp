#include "Boats.hpp"

static void
mutateNet(NeuralNet &net) {
	const int odds = randint(1, net.nw*4);
	for (int i=0; i<net.nw; i++) {
		if (diceroll(odds)) {
			float nwi = net.weight[i];
			nwi += randflt(-1.0, 1.0);
			if      (nwi < -1.0) nwi += 2.0;
			else if (nwi >  1.0) nwi -= 2.0;
			net.weight[i] = nwi;
		}
	}
}

static Colour
netColour(NeuralNet &net) {
	double sum[3] = {0., 0., 0.};
	for (int i=0; i<net.nw; i++)
		sum[i%3] += (net.weight[i] + 1.0)*10.0;
	
	int r = int(sum[0]) % 512;
	int g = int(sum[1]) % 512;
	int b = int(sum[2]) % 512;
	
	if (r > 255) r = 511 - r;
	if (g > 255) g = 511 - g;
	if (b > 255) b = 511 - b;
	
	return Colour(r, g, b);
}


Boater::Boater() :
mode(this),
gridder(this), 
net(13, 24, 3, 2) {
	iconOut.setCol(Colour(68, 68, 68));
}

void
Boater::setPos(Vec2f p, Vec2f d) {
	pos = p;
	dir = d.normal();
	gridder.setPos(pos);
	iconBack.setPos(pos, dir);
	iconOut.setPos(pos, dir);
}

void
Boater::setCol(Colour c) {
	col = c;
	iconBack.setCol(c);
}


void
Boater::update() {
	float sensors[13] = {
		8., 8., 8., 8., 8., 8., 8., 8., 8., 8., 8., 8., 8.
	};
	
	// assess locals
	Boater *d;
	gridder.resetLocals();
	while (gridder.pollLocals((void**)&d)) {
		if (d == this) continue;
		const Vec2f vec = d->pos - pos;
		const float len = vec.len();
		
		if (len > 8.0) continue; // out of range
		if (len < 1.0) {         // collision 
			die    = true;
			d->die = true;
			return;
		}
		
		const float heading = dir.thetaTo(vec);
		const int   index   = (int)((heading + M_PI)*2); // 0 to 12
		if (len < sensors[index])
			sensors[index] = len;
	}
	
	// process network
	for (int i=0; i<13; i++)
		net.input[i] = 1.0 - sensors[i] / 8.0;
	net.process();
	const float netOut = net.output[0] - net.output[1];
	
	// steer
	const float speed = 0.2;
	const float turn  = 0.06;
	steer = Vec2f(-dir.y, dir.x)*netOut;
	vel = dir + steer*turn;
	vel.normalise();
	vel *= speed;
}




Boats::Boats() : grid(Rectf(256, 144), 8) {
	boatBack.loadFile("Soup/Resources/Boat_Background.png");
	boatOut.loadFile("Soup/Resources/Boat_Outline.png");
}

Boats::~Boats() {
	for (int i=0; i<list.count; i++)
		delete list[i];
}

void
Boats::mouse(Vec2f pos, bool press, bool release) {
	mPos = pos;
	if (press) {
		held = true;
		while (selected.count > 0)
			selected[0]->mode.unGroup();
			
		Boater *d = nullptr;
		float dist;
		if (grid.nearest(pos, (void**)&d, &dist) && dist < 2.0) {
			d->mode.setGroup(selected);
		}
	}
	
	else if (release) {
		held = false;
	}
}

void
Boats::draw(sf::RenderTarget &target) {
	if (selected.count) {
		Boater *s = selected[0];
		sf::CircleShape circle(8.0);
		circle.setOrigin(8.0, 8.0);
		circle.setPosition(s->pos.x, s->pos.y);
		circle.setFillColor(sf::Color::Transparent);
		circle.setOutlineColor(sf::Color(68, 68, 68));
		circle.setOutlineThickness(0.1);
		target.draw(circle);
		
		if (s->steer.len() > 0.25) {
			Arrow arrow;
			arrow.setArrow(s->pos, s->steer*4);
			arrow.draw(target);
		}
	}
	
	
	boatBack.draw(target);
	boatOut.draw(target);
}

Boater*
Boats::newBoater() {
	Boater *b = new Boater;
	b->vel = Vec2f_randNorm()*0.1;
	b->setPos(Vec2f_rand(grid.rect()), Vec2f_randNorm());
	b->iconBack.setIcon(boatBack);
	b->iconOut.setIcon(boatOut);
	b->gridder.setGrid(grid);
	b->net.randomise();
	b->setCol(netColour(b->net));
	list.add(b);
	return b;
}

void
Boats::threadUpdate(int i0, int i1) {
	for (int i=i0; i<i1; i++) {
		Boater *b = list[i];
		b->update();
	}
}


void
Boats::update() {

	if (pause) {
		if (selected.count > 0)
			selected[0]->update();
	}
	
	else {
		/* spawn random */
		if (diceroll(spawnOdds))
			newBoater();
		
		/* threaded update */
		int n = list.count;
		int i[5] = {0, n/4, n/2, (3*n)/4, n};
		sf::Thread t1(std::bind(&Boats::threadUpdate, this, i[0], i[1]));
		sf::Thread t2(std::bind(&Boats::threadUpdate, this, i[1], i[2]));
		sf::Thread t3(std::bind(&Boats::threadUpdate, this, i[2], i[3]));
		sf::Thread t4(std::bind(&Boats::threadUpdate, this, i[3], i[4]));
		t1.launch();
		t2.launch();
		t3.launch();
		t4.launch();
		t1.wait();
		t2.wait();
		t3.wait();
		t4.wait();
		
		for (int i=0; i<list.count; i++) {
			Boater *d = list[i];
			
			/* death */
			if (d->die) {
				list.remove(i--);
				delete d;
				continue;
			}
	
			Vec2f pos = d->pos;
			Vec2f vel = d->vel;
			vel += Vec2f_randNorm()*0.004; // some noise
			pos += vel;
			if (!pos.inRect(grid.rect())) { // edge
				vel = Vec2f(-vel.y, vel.x);
				pos.clamp(grid.rect());
			}
			d->setPos(pos, vel);
			
			/* spawn child */
			if (diceroll(birthOdds)) {
				Boater *c  = newBoater();
				
				// position
				Vec2f cpos = d->pos + Vec2f_randNorm()*1.4;
				Vec2f cdir = d->dir;
				c->dir = d->dir;
				c->setPos(cpos, cdir);
				
				// network
				for (int i=0; i<c->net.nw; i++)
					c->net.weight[i] = d->net.weight[i];
				mutateNet(c->net);
				c->setCol(netColour(c->net));
			}
		}
	}
	
	if (held && selected.count > 0) {
		Boater *b = selected[0];
		b->setPos(mPos, b->dir);
	}
}

