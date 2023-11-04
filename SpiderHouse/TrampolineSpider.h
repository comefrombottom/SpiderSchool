#pragma once
struct TrampolineSpider
{
	union {
		struct {

			Vec2 pos;
			double r;
		};
		Circle circle;
	};
	Vec2 wireFrom;
	double vx = 0;
	double originX;
	TrampolineSpider() = default;
	TrampolineSpider(const Vec2& pos) :pos(pos) {
		r = 30;
		wireFrom = pos + Vec2{0, -200};
		originX = pos.x;
		vx = 1;
	}
	TrampolineSpider(const Vec2& pos,const Vec2& wireFrom) :pos(pos),wireFrom(wireFrom) {
		r = 30;
		originX = pos.x;
		vx = 1;
	}
	void update(double delta) {
		vx -= (pos.x - originX) * delta * 5;
		vx -= vx * delta * 0.1;
		pos.x += vx * delta;
	}
	void draw() const{
		Line{ wireFrom,pos }.draw();
		TextureAsset(U"TrampolineSpider").rotated((wireFrom - pos).getAngle()).drawAt(pos);
		//circle.drawFrame(1,Palette::Yellow);
	}

};

