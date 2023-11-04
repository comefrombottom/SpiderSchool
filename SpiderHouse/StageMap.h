#pragma once
#include"ColGrid.h"
#include"TrampolineSpider.h"
#include"RideSpider.h"
#include"SwingSpider.h"
struct StageMap
{
	Vec2 size;
	static constexpr Vec2 oneCellSize{32, 32};
	ColGrid colGrid;
	ColGrid damageGrid;
	Grid<size_t> textureGrid;
	Grid<size_t> backtextureGrid;
	static constexpr Vec2 oneRectUnitSize{800, 600};
	Grid<std::shared_ptr<RectF>> stageRectUnitGrid;

	Array<std::shared_ptr<struct TrampolineSpider>> trampolineSpiders;
	Array<std::shared_ptr<struct RideSpider>> rideSpiders;
	Array<std::shared_ptr<struct SwingSpider>> swingSpiders;

	Array<std::pair<Vec2, Vec2>> trampolineSpidersSave;
	Array<LineString> rideSpidersSave;
	Array<std::pair<Vec2, Vec2>> swingSpidersSave;

	Array<std::shared_ptr<Vec2>> responPoss;

	Array<std::pair<Vec2, size_t>> textureObjects;
	Array<std::pair<Vec2, size_t>> textureObjectsBack;

	Vec2 brotherPos{300,500};

	StageMap() = default;
	StageMap(const Vec2& size):size(size) {
		Size gridsize = Ceil(size / oneCellSize).asPoint();
		colGrid = ColGrid({ 0,0 }, oneCellSize, gridsize);
		damageGrid = ColGrid({ 0,0 }, oneCellSize,gridsize);
		textureGrid = Grid<size_t>(gridsize);
		backtextureGrid = Grid<size_t>(gridsize);
		stageRectUnitGrid = Grid<std::shared_ptr<RectF>>(Ceil(size / oneRectUnitSize).asPoint());
	}
	static Point getStageRectUnitIndex(const Vec2& pos) {
		return Floor(pos / oneRectUnitSize).asPoint();
	}
	static Vec2 RectUnitCenter(const Point& index) {
		return index * oneRectUnitSize + oneRectUnitSize / 2;
	}

	//void addTrampolineSpider(const Vec2& pos) {
	//	auto o= std::make_shared<TrampolineSpider>(pos);
	//	trampolineSpiders << o;
	//	Point index = Floor(pos / oneRectUnitSize).asPoint();
	//	for (int32 i = -1; i <= 1; i++) {
	//		for (int32 j = -1; j <= 1; j++) {
	//			Point see = index + Point{i, j};
	//			if (stageRectUnitGrid.inBounds(see)) {
	//				//stageRectUnitGrid[see].trampolineSpidersWPtr << o;
	//			}
	//		}
	//	}
	//}
	//void addRideSpider(const LineString& wire) {
	//	const Vec2& pos = wire.front();
	//	auto o = std::make_shared<RideSpider>(wire);
	//	rideSpiders << o;
	//	Point index = Floor(pos / oneRectUnitSize).asPoint();
	//	for (int32 i = -1; i <= 1; i++) {
	//		for (int32 j = -1; j <= 1; j++) {
	//			Point see = index + Point{i, j};
	//			if (stageRectUnitGrid.inBounds(see)) {
	//				stageRectUnitGrid[see].rideSpidersWPtr << o;
	//			}
	//		}
	//	}
	//}
	//void addSwingSpider(const Vec2& pos,const Vec2& wireFrom) {
	//	auto o = std::make_shared<SwingSpider>(pos,wireFrom);
	//	swingSpiders << o;
	//	Point index = Floor(pos / oneRectUnitSize).asPoint();
	//	for (int32 i = -1; i <= 1; i++) {
	//		for (int32 j = -1; j <= 1; j++) {
	//			Point see = index + Point{i, j};
	//			if (stageRectUnitGrid.inBounds(see)) {
	//				stageRectUnitGrid[see].swingSpidersWPtr << o;
	//			}
	//		}
	//	}
	//}

	void reloadSpiders() {
		trampolineSpiders.clear();
		for (auto& save : trampolineSpidersSave) {
			trampolineSpiders << std::make_shared<TrampolineSpider>(save.first, save.second);
		}
		rideSpiders.clear();
		for (auto& save : rideSpidersSave) {
			rideSpiders << std::make_shared<RideSpider>(save);
		}
		swingSpiders.clear();
		for (auto& save : swingSpidersSave) {
			swingSpiders << std::make_shared<SwingSpider>(save.first, save.second);
		}
	}

	void initStageUnitRectGrid() {
		for (auto& p : step(stageRectUnitGrid.size())) {
			stageRectUnitGrid[p] = std::make_shared<RectF>( p * oneRectUnitSize,oneRectUnitSize );
		}
	}

	template <class Archive>
	void SIV3D_SERIALIZE(Archive& archive)
	{
		archive(size, colGrid,damageGrid, textureGrid, backtextureGrid, stageRectUnitGrid,trampolineSpidersSave,rideSpidersSave,swingSpidersSave,responPoss,textureObjects,textureObjectsBack,brotherPos);
	}
};


