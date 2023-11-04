#pragma once
struct RideSpider
{
	union {
		struct {

			Vec2 pos;
			double r;
		};
		Circle circle;
	};
	LineString wire;
	double wireLength;
	Vec2 prePos;
	double posAtWire = 0;
	double speed = 0;
	double speedV = 0;
	double speedMax = 500;
	bool grabbed = false;
	double cantGrabTime = 0;
	double afterGoTime = 0;
	double speedSave = 0;
	enum class State {
		wait,
		beforeGo,
		go,
		afterGo,
		back,
	};
	State state=State::wait;
	RideSpider(const LineString& wire) :wire(wire), wireLength(wire.calculateLength()){
		pos = wire.front();
		prePos = pos;
		r = 30;
		posAtWire = 0;
		state = State::wait;

	}
	void update(double delta) {
		switch (state)
		{
		case RideSpider::State::wait: {
			if (grabbed) {
				state = State::go;
				speed = 0;
				speedV = 100;
			}
		}
			break;
		case RideSpider::State::beforeGo:
			break;
		case RideSpider::State::go: {
			speedV += 3000 * delta;
			speed += speedV * delta;
			if (speed > speedMax)speed = speedMax;
			if (posAtWire >= wireLength) {
				posAtWire = wireLength;
				state = State::afterGo;
				afterGoTime = 0;
				speedSave = speed;
				speed = 0;
			}
		}

			break;
		case RideSpider::State::afterGo: {
			afterGoTime += delta;
			if (afterGoTime >=0.8) {
				state = State::back;
				speed = -150;
			}
		}
			break;
		case RideSpider::State::back:
			if (posAtWire <= 0) {
				posAtWire = 0;
				state = State::wait;
				speed = 0;
			}
			break;
		default:
			break;
		}

		posAtWire += speed * delta;
		prePos = pos;
		pos = wire.calculatePointFromOrigin(posAtWire);
		
		if (cantGrabTime > 0) {
			cantGrabTime -= delta;
			if (cantGrabTime < 0)cantGrabTime = 0;
		}
	}
	void draw() {
		wire.draw();
		//Circle(pos, r).draw(Palette::Orange);
		Vec2 tan = calcTangent();
		TextureAsset(U"RideSpider").rotated(tan.getAngle() + 180_deg).drawAt(pos);

	}
	Vec2 calcTangent() {

		const size_t n = wire.num_lines();
		const Vec2* pData = wire.data();
		double currentLength = 0.0;
		for (size_t i = 0; i < n; ++i)
		{
			const double length = (pData[i].distanceFrom(pData[i + 1]));

			if (posAtWire <= (currentLength + length))
			{
				return (pData[i + 1] - pData[i]).normalized();
			}

			currentLength += length;
		}
		return (pData[n] - pData[n-1]).normalized();
	}

};

