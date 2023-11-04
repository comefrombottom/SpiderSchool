#pragma once
struct Player
{
	Vec2 pos;
	Vec2 size{ 24,28*2 };
	Vec2 v{};
	bool onGround = false;
	bool onRightWall = false;
	bool onLeftWall = false;
	bool isRight = true;
	double walkTime = 0;

	double afterKickRightWallTime = 0;
	double afterKickLeftWallTime = 0;

	std::weak_ptr<class RideSpider> ridingSpider;
	std::weak_ptr<class SwingSpider> swingingSpider;
	bool ridingRight = false;

	bool damaged = false;

	bool wallSlide = false;

	RectF rect() const{
		return RectF{ pos,size };
	}
	Vec2 center() const{
		return pos + size / 2;
	}
	const Player& setCenter(const Vec2& set_pos) {
		pos = set_pos - size / 2;
		return *this;
	}
	void draw()const{
		ScopedRenderStates2D sampler{ SamplerState::ClampNearest };
		ColorF scmc = ColorF(1,1);
		ColorF scac=ColorF(0,0);
		if (damaged) {
			scmc = ColorF(0);
			scac = ColorF(1, 0.5, 0.5, 0);
		}
		ScopedColorMul2D scm{ scmc };
		ScopedColorAdd2D sca{ scac };
		if (ridingSpider.expired() and swingingSpider.expired()) {
			if (onGround) {
				if (walkTime == 0) {
					uint16 page = 0;
					TextureAsset(U"sister")(page % 3 * 32, page / 3 * 32, 32, 32).scaled(2).mirrored(not isRight).drawAt(center());
				}
				else {
					uint16 page = int32(walkTime / 0.0625) % 4 + 1;
					TextureAsset(U"sister")(page % 3 * 32, page / 3 * 32, 32, 32).scaled(2).mirrored(not isRight).drawAt(center());
				}
			}
			else {
				if (wallSlide) {

					uint16 page = 6;
					TextureAsset(U"sister")(page % 3 * 32, page / 3 * 32, 32, 32).scaled(2).mirrored(not isRight).drawAt(center());
				}
				else {

					uint16 page = 5;
					TextureAsset(U"sister")(page % 3 * 32, page / 3 * 32, 32, 32).scaled(2).mirrored(not isRight).drawAt(center());
				}
			}
		}
		else {
			uint16 page = 6;
			TextureAsset(U"sister")(page % 3 * 32, page / 3 * 32, 32, 32).scaled(2).mirrored(ridingRight).drawAt(center());
		}

		//rect().drawFrame();
		
	}
};
