#pragma once
struct SwingSpider
{
	union {
		struct {
			Vec2 pos;
			double r;
		};
		Circle circle;
	};
	Vec2 wireFrom;
	Vec2 waitPos;
	Vec2 prePos;
	bool grabbed = false;
	double cantGrabTime = 0;
	double wireLengthSq = 0;
	double vx = 0;
	enum class State {
		wait,
		swing,

	};
	State state=State::wait;
	SwingSpider(const Vec2& waitPos, const Vec2& wireFrom) :waitPos(waitPos), wireFrom(wireFrom) {
		pos = waitPos;
		prePos = pos;
		r = 30;
		state = State::wait;
		wireLengthSq = (waitPos - wireFrom).lengthSq();
	}
	void update(double delta) {
		prePos = pos;
		switch (state)
		{
		case SwingSpider::State::wait: {
			if (grabbed) {
				state = State::swing;
				vx = 0;
			}
		}
			break;
		case SwingSpider::State::swing: {
			vx += (wireFrom.x - pos.x) * delta * 15;
			pos.x += vx * delta;

			double x = wireFrom.x - pos.x;
			pos.y = sqrt(wireLengthSq - x * x) + wireFrom.y;
			if (not grabbed and (pos - waitPos).length() < 10) {
				state = State::wait;
				pos = waitPos;
			}
		}
			break;
		default:
			break;
		}

		if (cantGrabTime > 0) {
			cantGrabTime -= delta;
			if (cantGrabTime < 0)cantGrabTime = 0;
		}
	}

	void draw() {
		Line(wireFrom, pos).draw();
		//Circle(waitPos, 40).draw(ColorF(1, 0.5));
		{
			ScopedRenderStates2D sampler{ SamplerState::ClampNearest };
			TextureAsset(U"swingHoldPlace").scaled(3).drawAt(waitPos);
		}
		//Circle(pos, r).draw(Palette::Violet);
		TextureAsset(U"SwingSpider").rotated((wireFrom - pos).getAngle()).drawAt(pos);

	}

};
