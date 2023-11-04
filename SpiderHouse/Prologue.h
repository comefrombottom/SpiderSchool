#pragma once
class Prologue {
	double time = 0;
	struct Chat {
		String string;
		bool isSister = true;
	};
	Array<Chat> chats;
	size_t chatIndex = 0;

	Font font{ 30 };

	double timeAterChatEnd = 0;
public:
	Prologue() {
		chats << Chat{U"お姉ちゃん…タスケテ…", false};
		chats << Chat{U"?", true};
		chats << Chat{U"怪我しちゃって", false};
		chats << Chat{U"歩けそうにないんだ", false};
		chats << Chat{U"大丈夫！？", true};
		chats << Chat{U"まずいかも", false};
		chats << Chat{U"今どこにいるの！", true};
		chats << Chat{U"廃校のどこか", false};
		chats << Chat{U"廃校!?", true};
		chats << Chat{U"あそこは危険だから", true};
		chats << Chat{U"入っちゃダメって", true};
		chats << Chat{U"お母さん言ってたでしょ？", true};
		chats << Chat{U"はい", false};
		chats << Chat{U"お姉ちゃん…", false};
		chats << Chat{U"tasukete？", false};
		chats << Chat{U"バカ！！", true};
		chats << Chat{U"今行くからちょっと待ってなさい！", true};
	}
	void update(double delta=Scene::DeltaTime()) {
		time += delta;
		{
			ScopedRenderStates2D sampler{ SamplerState::ClampNearest };
			if (time < 1)
			{
				TextureAsset(U"smartPhone").scaled(10).rotated(sin(time * 20) * exp(-time * 3) * 0.1).drawAt(Scene::CenterF() + Vec2{0, 300} *(1 - EaseOutCubic(time)));
			}
			else if (time < 2) {
				TextureAsset(U"smartPhone").scaled(EaseOutQuad(time - 1) * 40 + 10).drawAt(Scene::CenterF());
			}
			else if (time < 3) {
				TextureAsset(U"smartPhone").scaled(50).drawAt(Scene::CenterF());
			}
			else {
				TextureAsset(U"smartPhone").scaled(50).drawAt(Scene::CenterF());


				chatIndex = Min<size_t>(size_t((time - 3) / 1.5), chats.size() - 1);

				//Transformer2D tf{ Mat3x2::Translate({0.0,-EaseOutExpo(fmod(time - 3,1.5)/1.5 * 10) * 100}) };
				if (chatIndex == chats.size() - 1)timeAterChatEnd += delta;
				for (size_t i = 0; i <= chatIndex; i++) {
					auto da = font(chats[i].string);
					double chat_y = 500 - (chatIndex - i) * 100;
					if (chats[i].isSister) {
						RectF rect{ Arg::rightCenter(630,chat_y),da.region().size };
						RoundRect{ RectF{Arg::center(rect.center()),rect.w + 40,rect.h + 10},20 }.draw(Palette::Darkblue);
						da.draw(rect.pos);
						TextureAsset(U"sister_icon").scaled(3).mirrored().drawAt(700, chat_y);

					}
					else {
						RectF rect{ Arg::leftCenter(170,chat_y),da.region().size };
						RoundRect{ RectF{Arg::center(rect.center()),rect.w + 40,rect.h + 10},20 }.draw(Palette::Gray);
						da.draw(rect.pos);
						TextureAsset(U"brother_icon").scaled(3).mirrored().drawAt(100, chat_y);
					}
				}
			}

		}
	}
	bool sceneEnd() {
		return timeAterChatEnd > 2.5 or KeySpace.down();
	}
};
