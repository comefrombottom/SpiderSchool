#pragma once

class Title {
	bool m_sceneEnd = false;
	double timeAfterSpacePressed = 100;
public:
	void update(double delta=Scene::DeltaTime()) {
		if (KeySpace.down()) {
			timeAfterSpacePressed = 0;
		}
		if (timeAfterSpacePressed < 2) {
			timeAfterSpacePressed += delta;
			if (timeAfterSpacePressed > 1) {
				m_sceneEnd = true;
			}
		}
		
	}
	void draw() {
		
		ScopedColorMul2D scm{ ColorF{1,timeAfterSpacePressed < 2 ? 1 - 2 * timeAfterSpacePressed : 1} };
		
		Scene::Rect().draw(Palette::Gray);
		FontAsset(U"forEnding")(U"Spider\nSchool").drawAt(80, Vec2{ 500,250 }, ColorF(0.3));
		{
			ScopedRenderStates2D sampler{ SamplerState::ClampNearest };
			ScopedColorAdd2D sca{ 0.3 };
			TextureAsset(U"TrampolineSpider").scaled(4).drawAt(Vec2{ 50,500 }, ColorF(0, 0, 0, 1));
		}
		Line{ 50,0,50,356 }.draw(4,ColorF(0.3));
		FontAsset(U"forEnding")(U"すぱいだあ\nすくうる").drawAt(80, Vec2{ 400,200 }, ColorF(1,0.7));
		FontAsset(U"forEnding")(U"space to start").drawAt(Vec2{ 400,500 }, ColorF(1, 0.7));

		
		
	}
	bool sceneEnd() {
		return m_sceneEnd;
		
	}
};
