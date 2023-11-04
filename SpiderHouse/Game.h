#pragma once

#include"Player.h"
#include"D8Input.h"

#include"StageMap.h"
#include"Pulldown.h"
#include"Brother.h"
struct GameResultInfo {
	double timer;
	int32 deadCount;
};

class Game
{
	static constexpr bool BUILDABLE = false;
	bool firstUpdate = false;

	Camera2D camera{ Scene::CenterF(),1,CameraControl::None_ };

	Player player;
	D8Input d8Input;

	Array<StageMap> stageMaps;
	size_t stageMapIndex = 0;
	Point stageRectUnitIndex{};

	bool buildMode = false;
	Pulldown pulldown;
	Pulldown pulldown2;
	Pulldown pulldown3;
	TextEditState editText{ U"0" };

	Array<Vec2> addposs;
	bool addingPos;

	size_t responIndex = 0;

	RenderTexture renderTexture{ Scene::Size() ,Palette::White };
	RenderTexture renderTextureBack{ Scene::Size() ,Palette::White };

	double afterPrologueTime = 0;

	Brother brother;

	//Polygon spiderPolygon;
	//MultiPolygon spiderPolygon_not;

	double dyingTime = 0;
	bool deadSwitch = true;

	double timer = 0;
	int32 deadCount = 0;
	bool gameClear = false;

	bool howControlShow = true;
	double howControlFade = 0;
public:
	Game() {
		d8Input.setInputGroup(KeyD | KeyRight, KeyS | KeyDown, KeyA | KeyLeft, KeyW | KeyUp);
		
		if(true){
			Deserializer<BinaryReader> reader{ Resource(U"stageMaps.bin") };

			if (not reader) // もしオープンに失敗したら
			{
				throw Error{ U"Failed to open `stageMaps.bin`" };
			}

			reader(stageMaps);

			

		}
		else {
			stageMaps << StageMap({ 800 * 20, 600 * 20 });

			ColGrid& colGrid = stageMaps[0].colGrid;
			for (size_t i = 0; i < 31; i++)
			{
				colGrid.setUp({ i,10 }, true);
			}
			for (auto& stageMap : stageMaps) {
				stageMap.stageRectUnitGrid = Grid<std::shared_ptr<RectF>>(Ceil(stageMap.size / StageMap::oneRectUnitSize).asPoint());
				stageMap.initStageUnitRectGrid();
			}
			/*stageMaps[0].addTrampolineSpider({ 300,280 });
			stageMaps[0].addRideSpider(LineString{ {400,380},{400,100} });
			stageMaps[0].addSwingSpider(Vec2{ 500,430 }, Vec2{ 600,100 });*/
		}
		for (auto& stage : stageMaps) {
			stage.reloadSpiders();
		}

		brother = Brother(stageMaps[stageMapIndex].brotherPos);
		player = Player{ {100,100} };
		Font font24{ 24 };
		Font font12{ 8 };
		pulldown = Pulldown{ Array<String>{U"tram",U"ride",U"swing",U"rideYAxis",U"rideXAxis",U"respon",U"RectUnit",U"texObj",U"texObjBack",U"firstRespon"},font24,{120,10} };
		pulldown2 = Pulldown{ Array<String>{U"col",U"damage",U"tex",U"backTex"},font24,{320,10} };
		pulldown3 = Pulldown{ Array<String>{U"wood",U"window",U"halfstep",U"whitewall",U"black",U"backwood",U"dirt",U"grass_dirt",U"thorn▲",U"thorn▶",U"thorn▼",U"thorn◀",U"shueBox",U"bookBox",U"thorn■",U"bwoodx",U"bioletWool",
			U"bb0",U"bb1",U"bb2",U"bb3",U"bb4",U"bb5",U"bb6",U"bb7",U"bb8",U"log",U"rockrenga"},font12,{520,10}};

		auto& stageMap = stageMaps[stageMapIndex];
		if (stageMaps[stageMapIndex].responPoss) {
			player.setCenter(*stageMaps[stageMapIndex].responPoss.front());

			stageRectUnitIndex = StageMap::getStageRectUnitIndex(player.center());
			if (stageMap.stageRectUnitGrid.inBounds(stageRectUnitIndex) and stageMap.stageRectUnitGrid[stageRectUnitIndex]) {
				RectF sRect = *stageMap.stageRectUnitGrid[stageRectUnitIndex];
				Vec2 half = camera.getRegion().size / 2;
				Vec2 center = player.center();
				if (center.x - half.x < sRect.x)center.x = sRect.x + half.x;
				if (center.y - half.y < sRect.y)center.y = sRect.y + half.y;
				if (center.x + half.x > sRect.rightX())center.x = sRect.rightX() - half.x;
				if (center.y + half.y > sRect.bottomY())center.y = sRect.bottomY() - half.y;
				camera.setCenter(center);
			}
			else {
				camera.setCenter(player.center());
			}
		}
		/*spiderPolygon = Image(U"siruetto_spider.png").alphaToPolygonCentered().simplified();
		spiderPolygon_not = Geometry2D::Subtract(Rect{ Arg::center(0,0),1000,1000 }.asPolygon(), spiderPolygon);*/
	}
	~Game() {

		if constexpr(BUILDABLE) {
			// バイナリファイルをオープン
			Serializer<BinaryWriter> writer{ U"stageMaps.bin" };

			if (not writer) // もしオープンに失敗したら
			{
				throw Error{ U"Failed to open `stageMaps.bin`" };
			}

			// シリアライズに対応したデータを記録
			writer(stageMaps);
			
			
		}
		


	}
	void update(double delta=Scene::DeltaTime()) {

		stageRectUnitIndex = StageMap::getStageRectUnitIndex(player.center());

		auto& stageMap = stageMaps[stageMapIndex];
		ColGrid& colGrid = stageMaps[stageMapIndex].colGrid;
		auto& trampolineSpiders = stageMaps[stageMapIndex].trampolineSpiders;
		auto& rideSpiders = stageMaps[stageMapIndex].rideSpiders;
		auto& swingSpiders = stageMaps[stageMapIndex].swingSpiders;

		if (BUILDABLE and firstUpdate and KeyControl.pressed() and KeyB.down()) {
			if (buildMode) {
				buildMode = false;
				camera.setParameters(Camera2DParameters::NoControl());
				camera.setTargetScale(1);
				
				
			}
			else {
				buildMode = true;
				camera.setParameters(Camera2DParameters::Make(CameraControl::Default));
			}
			
		}
		if (buildMode) {
			if(firstUpdate)Print << U"buildMode";
		}
		d8Input.update(delta);
		
		if (not buildMode) {
			if (stageMap.stageRectUnitGrid.inBounds(stageRectUnitIndex) and stageMap.stageRectUnitGrid[stageRectUnitIndex]) {
				RectF sRect = *stageMap.stageRectUnitGrid[stageRectUnitIndex];
				Vec2 half = camera.getRegion().size / 2;
				Vec2 center = player.center();
				if (center.x - half.x < sRect.x)center.x = sRect.x + half.x;
				if (center.y - half.y < sRect.y)center.y = sRect.y + half.y;
				if (center.x + half.x > sRect.rightX())center.x = sRect.rightX() - half.x;
				if (center.y + half.y > sRect.bottomY())center.y = sRect.bottomY() - half.y;
				camera.setTargetCenter(center);
			}
			else {
				camera.setTargetCenter(player.center());
			}
			
		}
		//if (not buildMode)camera.setTargetCenter({ player.pos.x,StageMap::RectUnitCenter(stageRectUnitIndex).y });
		if(not buildMode or not KeySpace.pressed())camera.update(delta);
		{
			auto debugTf = camera.createTransformer();

			if (not buildMode) {
				if (afterPrologueTime < 2) {
					afterPrologueTime += delta;
				}
				if (firstUpdate and KeySpace.down()) {
					if (player.onGround) {
						player.v.y -= 11 * 60;
					}
					else if (player.onRightWall) {
						player.isRight = false;
						player.v.y = -10 * 60;
						player.v.x = -4 * 60;
						player.afterKickRightWallTime = 0.21;
					}
					else if (player.onLeftWall) {
						player.isRight = true;
						player.v.y = -10 * 60;
						player.v.x = 4 * 60;
						player.afterKickLeftWallTime = 0.21;
					}
				}
				bool pushRunKey = false;
				player.wallSlide = false;
				if (not player.damaged) {
					if (d8Input.rWide()) {
						pushRunKey = true;
						if (player.onGround) {
							player.isRight = true;
							constexpr double maxSpeed = 4 * 60;
							if (player.v.x < maxSpeed) {
								player.v.x += 60 * delta * 60;
								if (player.v.x > maxSpeed) {
									player.v.x = maxSpeed;
								}
							}
						}
						else {
							if (player.afterKickRightWallTime == 0) {
								player.isRight = true;
								constexpr double maxSpeed = 4 * 60;
								if (player.v.x < maxSpeed) {
									player.v.x += 60 * delta * 30;
									if (player.v.x > maxSpeed) {
										player.v.x = maxSpeed;
									}
								}
							}
							if (player.onRightWall && player.v.y > 0) {
								player.v.y += -player.v.y * 0.2 * delta * 60;
								player.wallSlide = true;
							}
						}
					}
					if (d8Input.lWide()) {

						pushRunKey = true;
						if (player.onGround) {
							player.isRight = false;
							constexpr double maxSpeed = 4 * 60;
							if (player.v.x > -maxSpeed) {
								player.v.x -= 1 * 60 * delta * 60;
								if (player.v.x < -maxSpeed) {
									player.v.x = -maxSpeed;
								}
							}
						}
						else {
							if (player.afterKickLeftWallTime == 0) {
								player.isRight = false;
								constexpr double maxSpeed = 4 * 60;
								if (player.v.x > -maxSpeed) {
									player.v.x -= 1 * 60 * delta * 30;
									if (player.v.x < -maxSpeed) {
										player.v.x = -maxSpeed;
									}
								}
							}
							if (player.onLeftWall && player.v.y > 0) {
								player.v.y += -player.v.y * 0.2 * delta * 60;
								player.wallSlide = true;
							}
						}
					}
				}
				
				if (pushRunKey) {
					player.walkTime += delta;
				}
				else {
					player.walkTime = 0;
				}

				if (player.onGround) {
					player.v.x += -player.v.x * 0.2 * delta * 60;
				}

				if (player.afterKickRightWallTime > 0) {
					player.afterKickRightWallTime -= delta;
					if (player.afterKickRightWallTime < 0)player.afterKickRightWallTime = 0;
				}
				if (player.afterKickLeftWallTime > 0) {
					player.afterKickLeftWallTime -= delta;
					if (player.afterKickLeftWallTime < 0)player.afterKickLeftWallTime = 0;
				}
				//gravity
				player.v.y += 30 * 60 * delta;

				if (player.damaged) {
					
					if (dyingTime < 0.7) {
						if (deadSwitch) {
							deadCount++;
							deadSwitch = false;
							if (auto spider = player.ridingSpider.lock()) {
								spider->grabbed = false;
								player.ridingSpider.reset();
							}
							if (auto spider = player.swingingSpider.lock()) {
								spider->grabbed = false;
								player.swingingSpider.reset();
							}
						}
					}
					else {
						if (responIndex < stageMap.responPoss.size()) {
							player.setCenter(*stageMap.responPoss[responIndex]);
							camera.setCenter(camera.getTargetCenter());
						}
						if (not deadSwitch) {
							deadSwitch = true;

							stageMap.reloadSpiders();
						}
					}
					dyingTime += delta;
					player.v = { 0,0 };
					if (dyingTime > 1) {
						player.damaged = false;
						
					}
				}

				for (auto& spider : trampolineSpiders) {
					spider->update(delta);
				}
				for (auto& spider : rideSpiders) {
					spider->update(delta);
				}
				for (auto& spider : swingSpiders) {
					spider->update(delta);
				}
				brother.update(delta);


				bool spiderGrabbedNow = false;
				if (not player.damaged) {
					//trampolineSpider intersects
					for (auto& spider : trampolineSpiders) {
						if (spider->circle.intersects(player.rect())) {
							if (player.v.y > 0 and player.rect().bottomY() <= spider->pos.y) {//playerが上
								player.v.y = -12 * 60;
							}
							spider->vx = -player.center().x + spider->pos.x;
						}
					}



					//rideSpider intersects
					if (auto spider = player.ridingSpider.lock()) {
						player.v = (spider->pos - (player.center() + Vec2{player.ridingRight ? -20 : 20, 10})) / delta;
						if (d8Input.rWide() && d8Input.timeZero()) {
							player.ridingRight = true;
						}
						if (d8Input.lWide() && d8Input.timeZero()) {
							player.ridingRight = false;
						}
						if (firstUpdate and KeySpace.down()) {
							spider->grabbed = false;

							double speed = spider->speed * 0.8;
							if (spider->state == RideSpider::State::afterGo) {
								if (spider->afterGoTime < 0.1) {
									speed = spider->speedSave * 0.8;
								}
							}
							player.v = spider->calcTangent() * speed;
							player.v.y = Min(player.v.y, 0.0);
							player.v.y -= 11 * 60;
							spider->cantGrabTime = 0.5;


							player.ridingSpider.reset();
						}
					}
					else {
						if (player.swingingSpider.expired()) {
							for (auto& spider : rideSpiders) {
								if (spider->circle.intersects(player.rect()) and spider->cantGrabTime == 0) {

									player.ridingRight = player.center().x > spider->pos.x;
									player.ridingSpider = spider;
									spider->grabbed = true;
									spiderGrabbedNow = true;
									break;
								}
							}
						}
					}

					//swingSpider intersects
					if (auto spider = player.swingingSpider.lock()) {
						player.v = (spider->pos - (player.center() + Vec2{player.ridingRight ? -20 : 20, 10})) / delta;
						if (d8Input.rWide() && d8Input.timeZero()) {
							player.ridingRight = true;
						}
						if (d8Input.lWide() && d8Input.timeZero()) {
							player.ridingRight = false;
						}
						if (firstUpdate and KeySpace.down()) {
							spider->grabbed = false;

							player.v = (spider->pos - spider->prePos) / delta;
							player.v.y = -11 * 60;
							spider->cantGrabTime = 0.5;


							player.swingingSpider.reset();
						}
					}
					else {
						if (player.swingingSpider.expired()) {
							for (auto& spider : swingSpiders) {
								if (spider->circle.intersects(player.rect()) and spider->cantGrabTime == 0) {

									player.ridingRight = player.center().x > spider->pos.x;
									player.swingingSpider = spider;
									spider->grabbed = true;
									spiderGrabbedNow = true;
									break;
								}
							}
						}
					}
				}

				if (player.rect().intersects(brother.rect())) {
					gameClear = true;
				}
				

				for (size_t i = 1; i < stageMap.responPoss.size(); i++) {
					if (stageMap.responPoss[i]->asCircle(10).intersects(RectF{Arg::center = player.rect().center(),40,player.size.y})) {
						responIndex = i;
					}
				}
				

				
				{
					auto [movedRectPos, movedV, colSides] = stageMap.damageGrid.calcColMovedRectF(player.rect(), player.v * delta);
					if (colSides[0] or colSides[1] or colSides[2] or colSides[3] and not player.damaged) {
						player.damaged = true;
						dyingTime = 0;
					}
				}
				if ((not player.damaged) and not player.rect().intersects(RectF{ 0,0,StageMap::oneRectUnitSize * stageMap.stageRectUnitGrid.size() })) {

					player.damaged = true;
					dyingTime = 0;

				}
				{
					auto [movedRectPos, movedV, colSides] = colGrid.calcColMovedRectF(player.rect(), player.v * delta);
					//playerColSide = colSides;
					bool collision = false;
					if (colSides[0] or colSides[2]) {
						collision = true;
						player.v.x = 0;
					}
					if (colSides[1] or colSides[3]) {
						collision = true;
						player.v.y = 0;
					}


					//v = movedV;
					player.pos = movedRectPos;
					if (collision and not spiderGrabbedNow) {
						if (auto spider = player.ridingSpider.lock()) {
							spider->cantGrabTime = 0.5;
							spider->grabbed = false;
							player.ridingSpider.reset();
							player.v = { 0,0 };
						}
						if (auto spider = player.swingingSpider.lock()) {
							spider->cantGrabTime = 0.5;
							spider->grabbed = false;
							player.swingingSpider.reset();
							player.v = { 0,0 };

						}
					}
				}
				

				{
					auto [movedRectPos, movedV, colSides] = colGrid.calcColMovedRectF(player.rect(), { 0,3 });
					player.onGround = (bool)colSides[3];
				}
				{
					auto [movedRectPos, movedV, colSides] = colGrid.calcColMovedRectF(player.rect(), { 10,0 });
					player.onRightWall = (bool)colSides[2];
				}
				{
					auto [movedRectPos, movedV, colSides] = colGrid.calcColMovedRectF(player.rect(), { -10,0 });
					player.onLeftWall = (bool)colSides[0];
				}
				
			}
			else {

				if (firstUpdate)Print << Cursor::PosF();
				if (firstUpdate and KeyP.down()) {
					player.setCenter(Cursor::PosF());
				}
				if (firstUpdate and KeyO.down()) {
					brother = Brother(Cursor::Pos());
					stageMap.brotherPos = Cursor::Pos();
				}
				//Build grid
				if(KeySpace.pressed()){
					switch (pulldown2.getIndex())
					{
					case 0: {

						ColGrid& cg = colGrid;
						Optional<Size> index = cg.indexAt(Cursor::Pos());
						if (MouseL.pressed()) {
							if (index) {
								cg.setBox(*index);
							}
						}
						if (MouseR.pressed()) {
							if (index) {
								cg.eraseBox(*index);
							}
						}
						if (firstUpdate and KeyRight.down()) {
							if (index) {
								cg.setRight(*index);
							}
						}
						if (firstUpdate and KeyLeft.down()) {
							if (index) {
								cg.setLeft(*index);
							}
						}
						if (firstUpdate and KeyDown.down()) {
							if (index) {
								cg.setDown(*index);
							}
						}
						if (firstUpdate and KeyUp.down()) {
							if (index) {
								cg.setUp(*index);
							}
						}
					}
						break;
					case 1: {
						ColGrid& cg = stageMap.damageGrid;
						Optional<Size> index = cg.indexAt(Cursor::Pos());
						if (MouseL.pressed()) {
							if (index) {
								cg.setBox(*index);
							}
						}
						if (MouseR.pressed()) {
							if (index) {
								cg.eraseBox(*index);
							}
						}
						if (firstUpdate and KeyRight.down()) {
							if (index) {
								cg.setRight(*index);
							}
						}
						if (firstUpdate and KeyLeft.down()) {
							if (index) {
								cg.setLeft(*index);
							}
						}
						if (firstUpdate and KeyDown.down()) {
							if (index) {
								cg.setDown(*index);
							}
						}
						if (firstUpdate and KeyUp.down()) {
							if (index) {
								cg.setUp(*index);
							}
						}
					}
						  break;
					case 2: {
						Size index = Floor(Cursor::PosF() / StageMap::oneCellSize).asPoint();
						if(stageMap.textureGrid.inBounds(index)) {
							if (MouseL.pressed()) {
								stageMap.textureGrid[index] = pulldown3.getIndex() + 1;
							}
							if (MouseR.pressed()) {
								stageMap.textureGrid[index] = 0;
							}
						}
					}
						  break;
					case 3: {
						Size index = Floor(Cursor::PosF() / StageMap::oneCellSize).asPoint();
						if (stageMap.backtextureGrid.inBounds(index)) {
							if (MouseL.pressed()) {
								stageMap.backtextureGrid[index] = pulldown3.getIndex() + 1;
							}
							if (MouseR.pressed()) {
								stageMap.backtextureGrid[index] = 0;
							}
						}
					}
						  break;
					default:
						break;
					}


					
				}
				else {
					if (firstUpdate and KeyF.down()) {
						addingPos = true;
						addposs.clear();
					}
					if (addingPos) {
						if (firstUpdate and MouseL.down()) {
							addposs << Cursor::PosF();
						}
					}
					if (firstUpdate and KeyF.up()) {
						addingPos = false;
						switch (pulldown.getIndex())
						{
						case 0: {
							if (addposs.size() == 2) {
								stageMap.trampolineSpidersSave.emplace_back(addposs[0], Vec2{ addposs[0].x,addposs[1].y });
								stageMap.trampolineSpiders << std::make_shared<TrampolineSpider>(addposs[0], Vec2{ addposs[0].x,addposs[1].y });
							}
						}
							  break;
						case 1: {
							if (addposs.size() >= 2) {
								stageMap.rideSpidersSave.emplace_back(addposs);
								stageMap.rideSpiders << std::make_shared<RideSpider>(LineString(addposs));
							}
						}
							  break;
						case 2: {
							if (addposs.size() == 2) {
								stageMap.swingSpidersSave.emplace_back(addposs[0], addposs[1]);
								stageMap.swingSpiders << std::make_shared<SwingSpider>(addposs[0], addposs[1]);
							}
						}
							  break;
						case 3: {
							if (addposs.size() == 2) {
								LineString ls = { addposs[0], Vec2{ addposs[0].x,addposs[1].y } };
								stageMap.rideSpidersSave.emplace_back(ls);
								stageMap.rideSpiders << std::make_shared<RideSpider>(ls);
							}
						}
							  break;
						case 4: {
							if (addposs.size() == 2) {
								LineString ls = { addposs[0], Vec2{ addposs[1].x,addposs[0].y } };
								stageMap.rideSpidersSave.emplace_back(ls);
								stageMap.rideSpiders << std::make_shared<RideSpider>(ls);
							}
						}
							  break;
						case 5: {
							if (addposs.size() == 1) {
								stageMap.responPoss << std::make_shared<Vec2>(addposs[0]);
							}
						}
							  break;
						case 6: {
							if (addposs.size() == 2) {
								
								Point index0 = StageMap::getStageRectUnitIndex(addposs[0]);
								Point index1 = StageMap::getStageRectUnitIndex(addposs[1]);
								if (index0.x <= index1.x and index0.y <= index1.y) {
									Size size = (index1 - index0) + Point{1, 1};
									auto share = std::make_shared<RectF>(index0 * StageMap::oneRectUnitSize, size * StageMap::oneRectUnitSize);
									for (auto& p : step(index0, size)) {
										if (stageMap.stageRectUnitGrid.inBounds(p)) {
											stageMap.stageRectUnitGrid[p] = share;
										}
									}
								}
								
							}
						}
							  break;
						case 7: {
							
							if (addposs.size() == 1) {
								size_t page = ParseOr<size_t>(editText.text, 0);
								stageMap.textureObjects.emplace_back(addposs[0], page);
							}
						}
							  break;
						case 8: {
							if (addposs.size() == 1) {
								size_t page = ParseOr<size_t>(editText.text, 0);
								stageMap.textureObjectsBack.emplace_back(addposs[0], page);
							}
						}
							  break;
						case 9: {
							if (addposs.size() == 1) {
								stageMap.responPoss.push_front( std::make_shared<Vec2>(addposs[0]));
							}
						}
							  break;
						default:
							break;
						}
					}

					if (firstUpdate and KeyControl.pressed() and KeyR.down()) {
						stageMap.reloadSpiders();
					}
					if (firstUpdate and KeyV.pressed() and MouseL.down()) {
						stageMap.trampolineSpidersSave.remove_if([](const auto& save) {
							return save.first.asCircle(30).intersects(Cursor::PosF());
						});
						stageMap.rideSpidersSave.remove_if([](const auto& save) {
							return save.front().asCircle(30).intersects(Cursor::PosF());
						});
						stageMap.swingSpidersSave.remove_if([](const auto& save) {
							return save.first.asCircle(30).intersects(Cursor::PosF());
						});
						stageMap.textureObjects.remove_if([](const auto& save) {
							return save.first.asCircle(30).intersects(Cursor::PosF());
						});
						stageMap.textureObjectsBack.remove_if([](const auto& save) {
							return save.first.asCircle(30).intersects(Cursor::PosF());
						});
						stageMap.responPoss.remove_if([](const auto& save) {
							return save->asCircle(10).intersects(Cursor::PosF());
						});

						stageMap.reloadSpiders();
					}
				}

			}
		}
		
		
		if (not buildMode) {

		}
		else {
			if (firstUpdate)pulldown.update();
			if (firstUpdate)pulldown2.update();
			if (firstUpdate)pulldown3.update();
		}


		if (howControlShow and player.pos.x > 1300) {
			howControlShow = false;
		}
		if (howControlShow and howControlFade<1) {
			howControlFade += delta;
		}
		if (not howControlShow and howControlFade>0) {
			howControlFade -= delta;
		}
		
		timer += delta;
		

	}
	void draw() {
		{
			

			
			
			auto& stageMap = stageMaps[stageMapIndex];
			ColGrid& colGrid = stageMaps[stageMapIndex].colGrid;
			auto& trampolineSpiders = stageMaps[stageMapIndex].trampolineSpiders;
			auto& rideSpiders = stageMaps[stageMapIndex].rideSpiders;
			auto& swingSpiders = stageMaps[stageMapIndex].swingSpiders;
			auto list = colGrid.listUpIndicesOn(camera.getRegion());
			Point renderTPosIndex = Floor(RectF{ Arg::center(camera.getCenter()),Scene::Size()*2}.pos / StageMap::oneCellSize).asPoint();
			auto drawTexture = [&](const Grid<size_t>& grid) {
				Transformer2D tf{ Mat3x2::Translate(-renderTPosIndex * Size(16,16)) };
				for (auto& index : list) {
					auto& page = grid[index];
					if (page == 0) {
						continue;
					}
					TextureAsset(U"Tiles")(page % 16 * 16, page / 16 * 16, 16, 16).draw(index * Size{16,16});
					
				}


			};
			
			{
				ScopedRenderStates2D sampler{ SamplerState::ClampNearest };
				renderTexture.clear(Palette::Steelblue);
				ScopedRenderTarget2D srt{ renderTexture };
				drawTexture(stageMap.backtextureGrid);
				{
					Transformer2D tf{ Mat3x2::Translate(-renderTPosIndex * Size(16,16)) };
					for (auto& o : stageMap.textureObjectsBack) {
						size_t page = o.second;

						size_t w = TextureAsset(U"textureObjects").width() / 64;
						TextureAsset(U"textureObjects")(page % w * 64, page / w * 64, 64, 64).drawAt(o.first / 2);
					}
				}
				Rect(renderTexture.size()).draw(ColorF(0, 0, 1, 0.2));
				drawTexture(stageMap.textureGrid);
				{
					Transformer2D tf{ Mat3x2::Translate(-renderTPosIndex * Size(16,16)) };
					for (auto& o : stageMap.textureObjects) {
						size_t page = o.second;

						size_t w = TextureAsset(U"textureObjects").width() / 64;
						TextureAsset(U"textureObjects")(page % w * 64, page / w * 64, 64, 64).drawAt(o.first / 2);
					}
				}

			}
			auto debugTf = camera.createTransformer();
			{
				ScopedRenderStates2D sampler{ SamplerState::ClampNearest };
				renderTexture.scaled(2).draw(renderTPosIndex * StageMap::oneCellSize);
			}
			if (BUILDABLE and (buildMode or KeyQ.pressed())) {
				for (auto& index : list) {
					colGrid.draw(index);
				}
				for (auto& index : list) {
					stageMap.damageGrid.draw(index, ColorF(1, 0, 0, 0.5), ColorF(1, 0, 0, 0.2));
				}
			}
		
			{
				ScopedRenderStates2D sampler{ SamplerState::ClampNearest };
				if (buildMode) {
					if (stageMap.responPoss) {
						TextureAsset(U"responPos")(0, 0, 16, 16).scaled(3).drawAt(*stageMap.responPoss.front(), ColorF(1, 0.5));
					}
				}
				for (size_t i = 1; i < stageMap.responPoss.size(); i++) {
					TextureAsset(U"responPos")(0, 0, 16, 16).scaled(3).drawAt(*stageMap.responPoss[i]);
				}
				if (responIndex == 0) {
					if (buildMode) {
						if (stageMap.responPoss) {

							TextureAsset(U"responPos")(0, 16, 16, 16).scaled(3).drawAt(*stageMap.responPoss[responIndex], ColorF(1, 0.5));
						}
					}
				}
				else {
					if (responIndex < stageMap.responPoss.size()) {
						TextureAsset(U"responPos")(0, 16, 16, 16).scaled(3).drawAt(*stageMap.responPoss[responIndex]);
					}
				}
			}
			
			for (auto& o : trampolineSpiders) {
				o->draw();
			}
			for (auto& o : rideSpiders) {
				o->draw();
			}
			for (auto& o : swingSpiders) {
				o->draw();
			}

			player.draw();
			//player.rect().drawFrame(1, Palette::Lightblue);
			//RectF{ Arg::center = player.rect().center(),40,player.size.y }.drawFrame();

			brother.draw();
			//brother.rect().drawFrame();
			if (howControlFade>0) {
				FontAsset(U"forEnding")(U"AD or ←→to move\nSPACE to jump").draw(1000, 11000, ColorF(1, howControlFade));
			}


			if (afterPrologueTime < 0.5) {
				ScopedRenderStates2D sampler{ SamplerState::ClampNearest };
				Vec2 dir = player.center() - camera.getCenter();
				double t = afterPrologueTime * 2;
				TextureAsset(U"smartPhone").scaled(EaseInCubic(1-t) * 40).rotated(-t*10).drawAt(camera.getCenter() + Vec2{dir.x* EaseInCubic(t), dir.y* EaseOutBack(t)});
			}

			if (buildMode) {

				for (auto& p : step(stageMap.stageRectUnitGrid.size())) {
					RectF{ p * StageMap::oneRectUnitSize,StageMap::oneRectUnitSize }.drawFrame(1, ColorF(0, 1, 1, 0.5));
				}
				for (auto& unit:stageMap.stageRectUnitGrid) {
					if (unit) {
						unit->drawFrame(2, ColorF(0.5, 0, 1, 0.5));
					}
				}
				RectF{ 0,0,StageMap::oneRectUnitSize * stageMap.stageRectUnitGrid.size() }.drawFrame(1, ColorF(1, 0, 0));

				if (addingPos) {
					for (auto& pos : addposs) {
						pos.asCircle(10).draw(Palette::Lightgrey);
					}
				}
			}
		}
		
		if (player.damaged) {
			ColorF color{ Palette::Dimgray };
			if(dyingTime<0.5){
				Rect{ 0,0,800,static_cast<int32>(EaseOutQuad(dyingTime-0.2)*3 * 600) }.draw(color);
			}
			else{
				Rect{ Arg::bottomLeft(0,600),800,static_cast<int32>(EaseOutQuad(1 - dyingTime)*3 * 600) }.draw(color);
			}
		}

		if (buildMode) {
			
			pulldown.draw();
			pulldown2.draw();
			pulldown3.draw();
			SimpleGUI::TextBox(editText, { 650,10 });
			if (KeyV.pressed()) {
				Scene::Rect().draw(ColorF(1, 0, 0, 0.1));
			}
			LineString{ Vec2{0,50},Vec2{0,0}, Vec2{50,0} }.moveBy(10, 10).draw(10, ColorF(1, 0.5));
			LineString{ Vec2{0,50},Vec2{0,0}, Vec2{-50,0} }.moveBy(800-10, 10).draw(10, ColorF(1, 0.5));
			LineString{ Vec2{0,-50},Vec2{0,0}, Vec2{50,0} }.moveBy(10, 600-10).draw(10, ColorF(1, 0.5));
			LineString{ Vec2{0,-50},Vec2{0,0}, Vec2{-50,0} }.moveBy(800-10, 600-10).draw(10, ColorF(1, 0.5));
		}
	}

	void setFirstUpdate(bool b) {
		firstUpdate = b;
	}
	void setAfterPrologueTimeZero() {
		afterPrologueTime = 0;
	}

	Optional<GameResultInfo> getResult() {
		if (gameClear) {
			return GameResultInfo{ timer,deadCount };
		}
		return none;
	}
};

