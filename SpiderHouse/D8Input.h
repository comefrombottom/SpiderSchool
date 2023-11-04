#pragma once
class D8Input
{
	InputGroup input_Right = KeyD;
	InputGroup input_Down = KeyS;
	InputGroup input_Left = KeyA;
	InputGroup input_Up = KeyW;


	/*
		5	6	7

		4	8	0

		3	2	1
	*/

	int32 d8Num = 8;
	int32 pre8Num = 8;
	double time_ = 0;
	Vec2 dVec = {};
public:
	void update(double delta = Scene::DeltaTime()) {

		pre8Num = d8Num;
		dVec = {};
		if (input_Right.pressed()) {
			dVec.x += 1;
		}
		if (input_Left.pressed()) {
			dVec.x -= 1;
		}
		if (input_Down.pressed()) {
			dVec.y += 1;
		}
		if (input_Up.pressed()) {
			dVec.y -= 1;
		}

		if (dVec.isZero()) {
			d8Num = 8;
		}
		else if (dVec == Vec2{1, 0}) {
			d8Num = 0;
		}
		else if (dVec == Vec2{1, 1}) {
			d8Num = 1;
		}
		else if (dVec == Vec2{0, 1}) {
			d8Num = 2;
		}
		else if (dVec == Vec2{-1, 1}) {
			d8Num = 3;
		}
		else if (dVec == Vec2{-1, 0}) {
			d8Num = 4;
		}
		else if (dVec == Vec2{-1, -1}) {
			d8Num = 5;
		}
		else if (dVec == Vec2{0, -1}) {
			d8Num = 6;
		}
		else if (dVec == Vec2{1, -1}) {
			d8Num = 7;
		}


		if (d8Num == pre8Num) {
			time_ += delta;
		}
		else {
			time_ = 0;
		}
	}

	void setInputGroup(const InputGroup& R, const InputGroup& D, const InputGroup& L, const InputGroup& U) {
		input_Right = R;
		input_Down = D;
		input_Left = L;
		input_Up = U;
	}
	int32 num() {
		return d8Num;
	}
	double time() {
		return time_;
	}
	Vec2 vec() {
		return dVec;
	}
	bool timeZero() {
		return time_ == 0;
	}

	bool neutral() {
		return d8Num == 8;
	}

	bool rWide() {
		return d8Num == 7 or d8Num == 0 or d8Num == 1;
	}
	bool dWide() {
		return d8Num == 1 or d8Num == 2 or d8Num == 3;
	}
	bool lWide() {
		return d8Num == 3 or d8Num == 4 or d8Num == 5;
	}
	bool uWide() {
		return d8Num == 5 or d8Num == 6 or d8Num == 7;
	}
	bool r() {
		return d8Num == 0;
	}
	bool rd() {
		return d8Num == 1;
	}
	bool d() {
		return d8Num == 2;
	}
	bool ld() {
		return d8Num == 3;
	}
	bool l() {
		return d8Num == 4;
	}
	bool lu() {
		return d8Num == 5;
	}
	bool u() {
		return d8Num == 6;
	}
	bool ru() {
		return d8Num == 7;
	}

};
