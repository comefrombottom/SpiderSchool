# include <Siv3D.hpp> // Siv3D v0.6.12
#include"Game.h"
#include"Title.h"
#include"Prologue.h"
#include"Ending.h"

void Main()
{
	Game game;
	FontAsset::Register(U"24", 24);
	FontAsset::Register(U"forEnding", FontMethod::MSDF, 40,Typeface::Black);
	TextureAsset::Register(U"sister", Resource(U"sister.png"));
	TextureAsset::Register(U"brother", Resource(U"brother.png"));
	TextureAsset::Register(U"TrampolineSpider", Resource(U"TrampolineSpiderx3.png"));
	TextureAsset::Register(U"RideSpider", Resource(U"RideSpiderx3.png"));
	TextureAsset::Register(U"SwingSpider", Resource(U"SwingSpiderx3.png"));
	TextureAsset::Register(U"Tiles", Resource(U"Tiles.png"));
	TextureAsset::Register( U"smartPhone", Resource(U"smartphone.png"));
	TextureAsset::Register(U"brother_icon", Resource(U"brother_icon.png"));
	TextureAsset::Register(U"sister_icon", Resource(U"sister_icon.png"));
	TextureAsset::Register(U"responPos", Resource(U"responPos.png"));
	TextureAsset::Register(U"textureObjects", Resource(U"textureObjects.png"));
	TextureAsset::Register(U"swingHoldPlace", Resource(U"swingHoldPlace.png"));


	constexpr double stepSec = (1.0 / 200.0);

	
	double accumulatorSec = 0.0;

	enum class EScene {
		title,
		prologue,
		game,
		pause,
		ending,

	};
	EScene escene=EScene::title;
	Title title;
	Prologue prologue;

	GameResultInfo resultInfo;

	Ending ending;

	while (System::Update())
	{
		ClearPrint();
		switch (escene)
		{
		case EScene::title: {
			title.update();
			title.draw();
			if (title.sceneEnd()) {
				escene = EScene::prologue;
			}
		}
			break;
		case EScene::prologue: {
			prologue.update();
			if (prologue.sceneEnd()) {
				escene = EScene::game;
			}

		}
			break;
		case EScene::game: {

			game.setFirstUpdate(true);
			for (accumulatorSec += Scene::DeltaTime(); stepSec <= accumulatorSec; accumulatorSec -= stepSec)
			{
				game.update(stepSec);
				game.setFirstUpdate(false);
			}
			game.draw();

			if (auto result = game.getResult()) {
				resultInfo = *result;
				escene = EScene::ending;
				ending.setResult(result->timer, result->deadCount);
			}

		}
			break;
		case EScene::pause:
			break;
		case EScene::ending: {
			ending.update();
			game.draw();
			ending.draw();
		}
			break;
		default:
			break;
		}

		
	}
}

//
// - Debug ビルド: プログラムの最適化を減らす代わりに、エラーやクラッシュ時に詳細な情報を得られます。
//
// - Release ビルド: 最大限の最適化でビルドします。
//
// - [デバッグ] メニュー → [デバッグの開始] でプログラムを実行すると、[出力] ウィンドウに詳細なログが表示され、エラーの原因を探せます。
//
// - Visual Studio を更新した直後は、プログラムのリビルド（[ビルド]メニュー → [ソリューションのリビルド]）が必要な場合があります。
//
