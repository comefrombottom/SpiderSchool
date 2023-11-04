#pragma once
struct Brother {
	Vec2 pos;
	Vec2 size;
	double timer = 0;
	Brother() = default;
	Brother(const Vec2& bottomP) {
		size = { 30,48 };
		pos.x = bottomP.x - size.x / 2;
		pos.y = bottomP.y - size.y;
	}
	
	RectF rect() {
		return RectF{ pos,size };
	}
	Vec2 center() {
		return pos + size / 2;
	}
	void update(double delta) {
		timer += delta;
	}
	void draw() {
		ScopedRenderStates2D sampler{ SamplerState::ClampNearest };
		uint16 page = int32(timer / 0.7) % 2;
		TextureAsset(U"brother")(0, page * 32, 32, 32).scaled(2).drawAt(center());
	}
};
