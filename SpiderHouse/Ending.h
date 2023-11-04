#pragma once
class Ending {
	double timer = 0;
	double clearTime = 0;
	double deadCount = 0;
public:
	void setResult(double clearTime_, int32 deadCount_) {
		clearTime = clearTime_;
		deadCount = deadCount_;
	}
	void update(double delta = Scene::DeltaTime()) {
		timer += delta;
	}
	void draw() {
		Scene::Rect().draw(ColorF(1, timer * 0.5));
		if (timer > 2) {
			FontAsset(U"forEnding")(U"ThankYouForPlaying!").drawAt(60,Scene::CenterF() + Vec2{0, -200}, Palette::Black);
			FontAsset(U"forEnding")(U"ClearTime:",clearTime,U"s").drawAt(50, Scene::CenterF(), Palette::Black);
			FontAsset(U"forEnding")(U"DeathCount:", deadCount).drawAt(40,Scene::CenterF() + Vec2{0,200}, Palette::Black);
		}
	}
};
