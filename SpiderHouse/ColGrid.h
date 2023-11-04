#pragma once

//どんなに速い速度で来られても貫通しないcollider
class ColGrid {
	Vec2 mOffset;
	Vec2 mOneGrid;
	Size mGridSize;

	Grid<std::array<bool, 5>> mGrid;//0:right,1:down,2:left,3:up,4:body
public:
	ColGrid() = default;
	ColGrid(const Vec2& pos, const Vec2& one, const Size& size) {
		mOffset = pos;
		mOneGrid = one;
		mGridSize = size;
		mGrid = Grid<std::array<bool, 5>>{ size };
	}

	Grid<std::array<bool, 5>>& gridRef() noexcept {
		return mGrid;
	}
	std::array<bool, 5>& operator[](const Size& index) {
		return mGrid[index];
	}
	const Size& size() const noexcept {
		return mGridSize;
	}
	const Vec2& offset() const noexcept {
		return mOffset;
	}
	const Vec2& oneGrid() const noexcept {
		return mOneGrid;
	}
	void resize(const Size& size) {
		mGridSize = size;
		mGrid.resize(size);
	}
	Vec2 coordinate(const Size& index) {
		return mOffset + mOneGrid * index;
	}


	Optional<Size> indexAt(const Vec2& pos) const noexcept {
		Size index = Floor((pos - mOffset) / mOneGrid).asPoint();
		if (mGrid.inBounds(index)) {
			return index;
		}
		return {};
	}
	RectF getRect(const Size& index) const noexcept {
		return RectF{ mOffset + mOneGrid * index,mOneGrid };
	}
	bool right(const Size& index)const {
		return mGrid[index][0];
	}
	bool down(const Size& index)const {
		return mGrid[index][1];
	}
	bool left(const Size& index)const {
		return mGrid[index][2];
	}
	bool up(const Size& index)const {
		return mGrid[index][3];
	}
	bool body(const Size& index)const {
		return mGrid[index][4];
	}
	bool hasRight(const Size& index) const noexcept {
		if (not mGrid.inBounds(index)) {
			return false;
		}
		return mGrid[index][0];
	}
	bool hasLeft(const Size& index) const noexcept {
		if (not mGrid.inBounds(index)) {
			return false;
		}
		return mGrid[index][2];
	}
	bool hasUp(const Size& index) const noexcept {
		if (not mGrid.inBounds(index)) {
			return false;
		}
		return mGrid[index][3];
	}
	bool hasDown(const Size& index) const noexcept {
		if (not mGrid.inBounds(index)) {
			return false;
		}
		return mGrid[index][1];
	}
	bool hasBody(const Size& index) const noexcept {
		if (not mGrid.inBounds(index)) {
			return false;
		}
		return mGrid[index][4];
	}
	void setRight(const Size& index, bool b = true) {
		mGrid[index][0] = b;
	}
	void setLeft(const Size& index, bool b = true) {
		mGrid[index][2] = b;
	}
	void setDown(const Size& index, bool b = true) {
		mGrid[index][1] = b;
	}
	void setUp(const Size& index, bool b = true) {
		mGrid[index][3] = b;
	}
	void setBody(const Size& index, bool b = true) {
		mGrid[index][4] = b;
	}
	void setFull(const Size& index, bool b = true) {
		mGrid[index] = { b,b,b,b,b };
	}
	void setBox(const Size& index) {
		mGrid[index][4] = true;
		Size rIndex = { index.x + 1,index.y };
		if (hasBody(rIndex)) {
			mGrid[rIndex][2] = false;
		}
		else {
			mGrid[index][0] = true;
		}
		Size lIndex = { index.x - 1,index.y };
		if (hasBody(lIndex)) {
			mGrid[lIndex][0] = false;
		}
		else {
			mGrid[index][2] = true;
		}
		Size dIndex = { index.x,index.y + 1 };
		if (hasBody(dIndex)) {
			mGrid[dIndex][3] = false;
		}
		else {
			mGrid[index][1] = true;
		}
		Size uIndex = { index.x,index.y - 1 };
		if (hasBody(uIndex)) {
			mGrid[uIndex][1] = false;
		}
		else {
			mGrid[index][3] = true;
		}
	}
	void eraseBox(const Size& index) {
		mGrid[index] = { false,false,false,false,false };
		Size rIndex = { index.x + 1,index.y };
		if (hasBody(rIndex)) {
			mGrid[rIndex][2] = true;
		}
		Size lIndex = { index.x - 1,index.y };
		if (hasBody(lIndex)) {
			mGrid[lIndex][0] = true;
		}
		Size dIndex = { index.x,index.y + 1 };
		if (hasBody(dIndex)) {
			mGrid[dIndex][3] = true;
		}
		Size uIndex = { index.x,index.y - 1 };
		if (hasBody(uIndex)) {
			mGrid[uIndex][1] = true;
		}
	}


	bool collideBody(const RectF& rect) const noexcept {
		Vec2 pos = rect.pos - mOffset;

		for (int32 i = (int32)floor(pos.x / mOneGrid.x); i < ceil((pos.x + rect.w) / mOneGrid.x); i++) {
			for (int32 j = (int32)floor(pos.y / mOneGrid.y); j < ceil((pos.y + rect.h) / mOneGrid.y); j++) {
				if (hasBody({ i,j })) {
					return true;
				}
			}
		}
		return false;
	}
	Array<Size> listUpCollideBodies(const RectF& rect) const noexcept {
		Array<Size> indices;
		Vec2 pos = rect.pos - mOffset;

		for (int32 i = (int32)floor(pos.x / mOneGrid.x); i < ceil((pos.x + rect.w) / mOneGrid.x); i++) {
			for (int32 j = (int32)floor(pos.y / mOneGrid.y); j < ceil((pos.y + rect.h) / mOneGrid.y); j++) {
				Size index = { i,j };
				if (hasBody(index)) {
					indices << index;
				}
			}
		}
		return indices;
	}
	Array<Size> listUpIndicesOn(const RectF& rect)const noexcept {
		Array<Size> indices;
		Vec2 pos = rect.pos - mOffset;

		for (int32 i = (int32)floor(pos.x / mOneGrid.x); i < ceil((pos.x + rect.w) / mOneGrid.x); i++) {
			for (int32 j = (int32)floor(pos.y / mOneGrid.y); j < ceil((pos.y + rect.h) / mOneGrid.y); j++) {
				Size index = { i,j };
				if (mGrid.inBounds(index)) {
					indices << index;
				}
			}
		}
		return indices;
	}

	//CollideSideIndices(up),pos,edge collide
	Optional<std::tuple<Array<Size>, Vec2, bool>> calcMoveXLineDownCol(const Vec2& pre_pos, double pre_length, const Vec2& next_pos, double next_length, bool neglectNonBody = false)const {
		int32 firstYIndex = (int32)ceil((pre_pos.y - mOffset.y) / mOneGrid.y);
		int32 lastYIndex = (int32)ceil((next_pos.y - mOffset.y) / mOneGrid.y);
		Vec2 subPos = next_pos - pre_pos;
		double slope_begin = subPos.x / subPos.y;
		double slope_end = (subPos.x + next_length - pre_length) / subPos.y;
		for (int32 i = firstYIndex; i < lastYIndex; i++) {
			if (i < 0) {
				continue;
			}
			if (mGridSize.y <= i) {
				break;
			}

			double y_begin = mOffset.y + mOneGrid.y * i;
			double y_sub = y_begin - pre_pos.y;
			double x_begin = pre_pos.x + slope_begin * y_sub;
			double x_end = pre_pos.x + pre_length + slope_end * y_sub;
			int32 firstXIndex = (int32)floor((x_begin - mOffset.x) / mOneGrid.x);
			int32 lastXIndex = (int32)ceil((x_end - mOffset.x) / mOneGrid.x) - 1;

			Array<Size> colIndices;
			for (int32 j = firstXIndex; j <= lastXIndex; j++) {
				if (j < 0) {
					continue;
				}
				if (mGridSize.x <= j) {
					break;
				}
				if (neglectNonBody && not mGrid[{j, i}][4]) {
					continue;
				}
				if (mGrid[{j, i}][3]) {
					colIndices << Size{j, i};
				}
			}
			if (colIndices) {
				return { { colIndices ,Vec2{x_begin,y_begin},false } };
			}
			//edge case
			else if (subPos.x > 0) {
				if (int32 j = lastXIndex + 1; j == (x_end - mOffset.x) / mOneGrid.x) {
					if (j < 0 || mGridSize.x <= j) {
						continue;
					}
					if (neglectNonBody && not mGrid[{j, i}][4]) {
						continue;
					}
					if (mGrid[{j, i}][3]) {
						colIndices << Size{j, i};
						return { { colIndices ,Vec2{x_begin,y_begin},true } };
					}
				}
			}
			else if (subPos.x < 0) {
				if (firstXIndex == (x_begin - mOffset.x) / mOneGrid.x) {
					int32 j = firstXIndex - 1;
					if (j < 0) {
						continue;
					}
					if (mGridSize.x <= j) {
						continue;
					}
					if (neglectNonBody && not mGrid[{j, i}][4]) {
						continue;
					}
					if (mGrid[{j, i}][3]) {
						colIndices << Size{j, i};
						return { { colIndices ,Vec2{x_begin,y_begin},true } };
					}
				}
			}
		}
		return {};
	}
	Optional<std::tuple<Array<Size>, Vec2, bool>> calcMoveXLineUpCol(const Vec2& pre_pos, double pre_length, const Vec2& next_pos, double next_length, bool neglectNonBody = false)const {
		int32 firstYIndex = (int32)floor((pre_pos.y - mOffset.y) / mOneGrid.y) - 1;
		int32 lastYIndex = (int32)floor((next_pos.y - mOffset.y) / mOneGrid.y) - 1;
		Vec2 subPos = next_pos - pre_pos;
		double slope_begin = subPos.x / subPos.y;
		double slope_end = (subPos.x + next_length - pre_length) / subPos.y;
		for (int32 i = firstYIndex; i > lastYIndex; i--) {
			if (i < 0) {
				break;
			}
			if (mGridSize.y <= i) {
				continue;
			}

			double y_begin = mOffset.y + mOneGrid.y * (i + 1);
			double y_sub = y_begin - pre_pos.y;
			double x_begin = pre_pos.x + slope_begin * y_sub;
			double x_end = pre_pos.x + pre_length + slope_end * y_sub;
			int32 firstXIndex = (int32)floor((x_begin - mOffset.x) / mOneGrid.x);
			int32 lastXIndex = (int32)ceil((x_end - mOffset.x) / mOneGrid.x) - 1;
			Array<Size> colIndices;
			for (int32 j = firstXIndex; j <= lastXIndex; j++) {
				if (j < 0) {
					continue;
				}
				if (mGridSize.x <= j) {
					break;
				}
				if (neglectNonBody && not mGrid[{j, i}][4]) {
					continue;
				}
				if (mGrid[{j, i}][1]) {
					colIndices << Size{j, i};
				}
			}
			if (colIndices) {
				return { { colIndices ,Vec2{x_begin,y_begin} ,false} };
			}
			//edge case
			else if (subPos.x > 0) {
				if (int32 j = lastXIndex + 1; j == (x_end - mOffset.x) / mOneGrid.x) {
					if (j < 0 || mGridSize.x <= j) {
						continue;
					}
					if (neglectNonBody && not mGrid[{j, i}][4]) {
						continue;
					}
					if (mGrid[{j, i}][1]) {
						colIndices << Size{j, i};
						return { { colIndices ,Vec2{x_begin,y_begin},true } };
					}
				}
			}
			else if (subPos.x < 0) {
				if (firstXIndex == (x_begin - mOffset.x) / mOneGrid.x) {
					int32 j = firstXIndex - 1;
					if (j < 0) {
						continue;
					}
					if (mGridSize.x <= j) {
						continue;
					}
					if (neglectNonBody && not mGrid[{j, i}][4]) {
						continue;
					}
					if (mGrid[{j, i}][1]) {
						colIndices << Size{j, i};
						return { { colIndices ,Vec2{x_begin,y_begin},true } };
					}
				}
			}
		}
		return {};
	}

	Optional<std::tuple<Array<Size>, Vec2, bool>> calcMoveYLineRightCol(const Vec2& pre_pos, double pre_length, const Vec2& next_pos, double next_length, bool neglectNonBody = false)const {
		int32 firstYIndex = (int32)ceil((pre_pos.x - mOffset.x) / mOneGrid.x);
		int32 lastYIndex = (int32)ceil((next_pos.x - mOffset.x) / mOneGrid.x);
		Vec2 subPos = next_pos - pre_pos;
		double slope_begin = subPos.y / subPos.x;
		double slope_end = (subPos.y + next_length - pre_length) / subPos.x;
		for (int32 i = firstYIndex; i < lastYIndex; i++) {
			if (i < 0) {
				continue;
			}
			if (mGridSize.x <= i) {
				break;
			}

			double y_begin = mOffset.x + mOneGrid.x * i;
			double y_sub = y_begin - pre_pos.x;
			double x_begin = pre_pos.y + slope_begin * y_sub;
			double x_end = pre_pos.y + pre_length + slope_end * y_sub;
			int32 firstXIndex = (int32)floor((x_begin - mOffset.y) / mOneGrid.y);
			int32 lastXIndex = (int32)ceil((x_end - mOffset.y) / mOneGrid.y) - 1;
			Array<Size> colIndices;
			for (int32 j = firstXIndex; j <= lastXIndex; j++) {
				if (j < 0) {
					continue;
				}
				if (mGridSize.y <= j) {
					break;
				}
				if (neglectNonBody && not mGrid[{i, j}][4]) {
					continue;
				}
				if (mGrid[{i, j}][2]) {
					colIndices << Size{i, j};
				}
			}
			if (colIndices) {
				return { { colIndices ,Vec2{y_begin,x_begin},false } };
			}
			//edge case
			else if (subPos.y > 0) {
				if (int32 j = lastXIndex + 1; j == (x_end - mOffset.y) / mOneGrid.y) {
					if (j < 0 || mGridSize.y <= j) {
						continue;
					}
					if (neglectNonBody && not mGrid[{i, j}][4]) {
						continue;
					}
					if (mGrid[{i, j}][2]) {
						colIndices << Size{i, j};
						return { { colIndices ,Vec2{y_begin,x_begin},true } };
					}
				}
			}
			else if (subPos.y < 0) {
				if (firstXIndex == (x_begin - mOffset.y) / mOneGrid.y) {
					int32 j = firstXIndex - 1;
					if (j < 0) {
						continue;
					}
					if (mGridSize.y <= j) {
						continue;
					}
					if (neglectNonBody && not mGrid[{i, j}][4]) {
						continue;
					}
					if (mGrid[{i, j}][2]) {
						colIndices << Size{i, j};
						return { { colIndices ,Vec2{y_begin,x_begin},true } };
					}
				}
			}
		}
		return {};
	}
	Optional<std::tuple<Array<Size>, Vec2, bool>> calcMoveYLineLeftCol(const Vec2& pre_pos, double pre_length, const Vec2& next_pos, double next_length, bool neglectNonBody = false)const {
		int32 firstYIndex = (int32)floor((pre_pos.x - mOffset.x) / mOneGrid.x) - 1;
		int32 lastYIndex = (int32)floor((next_pos.x - mOffset.x) / mOneGrid.x) - 1;
		Vec2 subPos = next_pos - pre_pos;
		double slope_begin = subPos.y / subPos.x;
		double slope_end = (subPos.y + next_length - pre_length) / subPos.x;
		for (int32 i = firstYIndex; i > lastYIndex; i--) {
			if (i < 0) {
				break;
			}
			if (mGridSize.x <= i) {
				continue;
			}

			double y_begin = mOffset.x + mOneGrid.x * (i + 1);
			double y_sub = y_begin - pre_pos.x;
			double x_begin = pre_pos.y + slope_begin * y_sub;
			double x_end = pre_pos.y + pre_length + slope_end * y_sub;
			int32 firstXIndex = (int32)floor((x_begin - mOffset.y) / mOneGrid.y);
			int32 lastXIndex = (int32)ceil((x_end - mOffset.y) / mOneGrid.y) - 1;
			Array<Size> colIndices;
			for (int32 j = firstXIndex; j <= lastXIndex; j++) {
				if (j < 0) {
					continue;
				}
				if (mGridSize.y <= j) {
					break;
				}
				if (neglectNonBody && not mGrid[{i, j}][4]) {
					continue;
				}
				if (mGrid[{i, j}][0]) {
					colIndices << Size{i, j};
				}
			}
			if (colIndices) {
				return { { colIndices ,Vec2{y_begin,x_begin},false } };
			}
			//edge case
			else if (subPos.y > 0) {
				if (int32 j = lastXIndex + 1; j == (x_end - mOffset.y) / mOneGrid.y) {
					if (j < 0 || mGridSize.y <= j) {
						continue;
					}
					if (neglectNonBody && not mGrid[{i, j}][4]) {
						continue;
					}
					if (mGrid[{i, j}][0]) {
						colIndices << Size{i, j};
						return { { colIndices ,Vec2{y_begin,x_begin},true } };
					}
				}
			}
			else if (subPos.y < 0) {
				if (firstXIndex == (x_begin - mOffset.y) / mOneGrid.y) {
					int32 j = firstXIndex - 1;
					if (j < 0) {
						continue;
					}
					if (mGridSize.y <= j) {
						continue;
					}
					if (neglectNonBody && not mGrid[{i, j}][4]) {
						continue;
					}
					if (mGrid[{i, j}][0]) {
						colIndices << Size{i, j};
						return { { colIndices ,Vec2{y_begin,x_begin},true } };
					}
				}
			}
		}
		return {};
	}


	//movedRectPos,movedV,collisionIndices{r,d,l,u}
	std::tuple<Vec2, Vec2, std::array<Array<Size>, 4>> calcColMovedRectF(const RectF& pre_rect, const Vec2& v) const {
		RectF moveRect = pre_rect;
		Vec2 moveV = v;

		bool yCol = false;
		bool xCol = false;

		//保険の厚み。
		constexpr double dd = 0.01;

		Array<Size> indices_xCol;
		Array<Size> indices_yCol;
		Vec2 colPos_xCol{};
		Vec2 colPos_yCol{};
		double xCol_lenSq = INFINITY;
		double yCol_lenSq = INFINITY;
		bool edge_xCol = false;
		bool edge_yCol = false;

		std::array<Array<Size>, 4> colSides;


		if (moveV.x > 0) {
			if (auto calcResult = calcMoveYLineRightCol(moveRect.tr(), moveRect.h, moveRect.tr() + moveV, moveRect.h)) {
				auto& [indices, colPos, edgeCol] = *calcResult;
				colPos_xCol = { colPos.x - moveRect.w,colPos.y };
				xCol_lenSq = (colPos_xCol - moveRect.pos).lengthSq();
				colPos_xCol.x -= dd;
				xCol = true;
				edge_xCol = edgeCol;
				colSides[2] = std::move(indices);
			}
		}
		else if (moveV.x < 0) {
			if (auto calcResult = calcMoveYLineLeftCol(moveRect.pos, moveRect.h, moveRect.pos + moveV, moveRect.h)) {
				auto& [indices, colPos, edgeCol] = *calcResult;
				colPos_xCol = { colPos.x,colPos.y };
				xCol_lenSq = (colPos_xCol - moveRect.pos).lengthSq();
				colPos_xCol.x += dd;
				xCol = true;
				edge_xCol = edgeCol;
				colSides[0] = std::move(indices);
			}
		}
		if (moveV.y > 0) {
			if (auto calcResult = calcMoveXLineDownCol(moveRect.bl(), moveRect.w, moveRect.bl() + moveV, moveRect.w)) {
				auto& [indices, colPos, edgeCol] = *calcResult;
				colPos_yCol = { colPos.x,colPos.y - moveRect.h };
				yCol_lenSq = (colPos_yCol - moveRect.pos).lengthSq();
				colPos_yCol.y -= dd;
				yCol = true;
				edge_yCol = edgeCol;
				colSides[3] = std::move(indices);
			}
		}
		else if (moveV.y < 0) {
			if (auto calcResult = calcMoveXLineUpCol(moveRect.pos, moveRect.w, moveRect.pos + moveV, moveRect.w)) {
				auto& [indices, colPos, edgeCol] = *calcResult;
				colPos_yCol = { colPos.x,colPos.y };
				yCol_lenSq = (colPos_yCol - moveRect.pos).lengthSq();
				colPos_yCol.y += dd;
				yCol = true;
				edge_yCol = edgeCol;
				colSides[1] = std::move(indices);
			}
		}

		if (xCol and yCol) {
			if (xCol_lenSq < yCol_lenSq) {
				yCol = false;
			}
			else if (xCol_lenSq > yCol_lenSq) {
				xCol = false;
			}
			else {
				if (edge_xCol) {
					if (edge_yCol) {
						xCol = false;
					}
					else {
						xCol = false;
					}
				}
				else {
					if (edge_yCol) {
						yCol = false;
					}
					else {

					}
				}
			}
		}
		if (xCol && yCol) {
			moveV = { 0,0 };
			moveRect.pos = colPos_xCol;
			Print << U"ColBOth";
		}
		else if (xCol) {
			colSides[1].clear();
			colSides[3].clear();

			moveV.x = 0;
			Vec2 smoveV = { 0,moveRect.pos.y + moveV.y - colPos_xCol.y };
			moveRect.pos = colPos_xCol;
			if (smoveV.y > 0) {
				if (auto calcResult = calcMoveXLineDownCol(moveRect.bl(), moveRect.w, moveRect.bl() + smoveV, moveRect.w)) {
					auto& [indices, colPos, edgeCol] = *calcResult;
					moveRect.pos.y = colPos.y - moveRect.h - dd;
					moveV.y = 0;
					yCol = true;

					colSides[3] = std::move(indices);
				}
			}
			else if (smoveV.y < 0) {
				if (auto calcResult = calcMoveXLineUpCol(moveRect.pos, moveRect.w, moveRect.pos + smoveV, moveRect.w)) {
					auto& [indices, colPos, edgeCol] = *calcResult;
					moveRect.pos.y = colPos.y + dd;
					moveV.y = 0;
					yCol = true;

					colSides[1] = std::move(indices);
				}
			}
			if (not yCol) {
				moveRect.pos.y += smoveV.y;
			}
		}

		else if (yCol) {
			colSides[0].clear();
			colSides[2].clear();

			moveV.y = 0;
			Vec2 smoveV = { moveRect.pos.x + moveV.x - colPos_yCol.x,0 };
			moveRect.pos = colPos_yCol;
			if (smoveV.x > 0) {
				if (auto calcResult = calcMoveYLineRightCol(moveRect.tr(), moveRect.h, moveRect.tr() + smoveV, moveRect.h)) {
					auto& [indices, colPos, edgeCol] = *calcResult;
					moveRect.pos.x = colPos.x - moveRect.w - dd;
					moveV.x = 0;
					xCol = true;

					colSides[2] = std::move(indices);
				}
			}
			else if (smoveV.x < 0) {
				if (auto calcResult = calcMoveYLineLeftCol(moveRect.pos, moveRect.h, moveRect.pos + smoveV, moveRect.h)) {
					auto& [indices, colPos, edgeCol] = *calcResult;
					moveRect.pos.x = colPos.x + dd;
					moveV.x = 0;
					xCol = true;

					colSides[0] = std::move(indices);
				}
			}
			if (not xCol) {
				moveRect.pos.x += smoveV.x;
			}
		}
		else {
			moveRect.pos += moveV;
		}
		return { moveRect.pos ,moveV ,colSides };

	}

	std::pair<Vec2, std::array<Array<Size>, 4>> calcFirstColMovedRectF(const RectF& pre_rect, const Vec2& v)const {
		RectF moveRect = pre_rect;
		Vec2 moveV = v;

		bool yCol = false;
		bool xCol = false;

		//保険の厚み。
		constexpr double dd = 0.01;

		Array<Size> indices_xCol;
		Array<Size> indices_yCol;
		Vec2 colPos_xCol{};
		Vec2 colPos_yCol{};
		double xCol_lenSq = INFINITY;
		double yCol_lenSq = INFINITY;
		bool edge_xCol = false;
		bool edge_yCol = false;

		std::array<Array<Size>, 4> colSides;


		if (v.x > 0) {
			if (auto calcResult = calcMoveYLineRightCol(pre_rect.tr(), pre_rect.h, pre_rect.tr() + v, pre_rect.h)) {
				auto& [indices, colPos, edgeCol] = *calcResult;
				colPos_xCol = { colPos.x - pre_rect.w,colPos.y };
				xCol_lenSq = (colPos_xCol - pre_rect.pos).lengthSq();
				colPos_xCol.x -= dd;
				xCol = true;
				edge_xCol = edgeCol;
				colSides[2] = std::move(indices);
			}
		}
		else if (v.x < 0) {
			if (auto calcResult = calcMoveYLineLeftCol(pre_rect.pos, pre_rect.h, pre_rect.pos + v, pre_rect.h)) {
				auto& [indices, colPos, edgeCol] = *calcResult;
				colPos_xCol = { colPos.x,colPos.y };
				xCol_lenSq = (colPos_xCol - pre_rect.pos).lengthSq();
				colPos_xCol.x += dd;
				xCol = true;
				edge_xCol = edgeCol;
				colSides[0] = std::move(indices);
			}
		}
		if (v.y > 0) {
			if (auto calcResult = calcMoveXLineDownCol(pre_rect.bl(), pre_rect.w, pre_rect.bl() + v, pre_rect.w)) {
				auto& [indices, colPos, edgeCol] = *calcResult;
				colPos_yCol = { colPos.x,colPos.y - pre_rect.h };
				yCol_lenSq = (colPos_yCol - pre_rect.pos).lengthSq();
				colPos_yCol.y -= dd;
				yCol = true;
				edge_yCol = edgeCol;
				colSides[3] = std::move(indices);
			}
		}
		else if (v.y < 0) {
			if (auto calcResult = calcMoveXLineUpCol(pre_rect.pos, pre_rect.w, pre_rect.pos + v, pre_rect.w)) {
				auto& [indices, colPos, edgeCol] = *calcResult;
				colPos_yCol = { colPos.x,colPos.y };
				yCol_lenSq = (colPos_yCol - pre_rect.pos).lengthSq();
				colPos_yCol.y += dd;
				yCol = true;
				edge_yCol = edgeCol;
				colSides[1] = std::move(indices);
			}
		}

		if (xCol and yCol) {
			if (xCol_lenSq < yCol_lenSq) {
				yCol = false;
			}
			else if (xCol_lenSq > yCol_lenSq) {
				xCol = false;
			}
			else {
				if (edge_xCol) {
					if (edge_yCol) {
						xCol = false;
					}
					else {
						xCol = false;
					}
				}
				else {
					if (edge_yCol) {
						yCol = false;
					}
					else {
						xCol = false;
					}
				}
			}
		}
		if (xCol) {
			colSides[1].clear();
			colSides[3].clear();
			return { colPos_xCol,colSides };
		}
		else if (yCol) {
			colSides[0].clear();
			colSides[2].clear();
			return { colPos_yCol,colSides };
		}
		else {
			return { pre_rect.pos + v,colSides };
		}
	}

	void draw(const Color& color = Palette::White, const Color& color2 = ColorF(1, 0.5))const {
		Transformer2D tf{ Mat3x2::Translate(mOffset) };
		for (auto index : step(mGrid.size())) {
			RectF outRect{ index * mOneGrid,mOneGrid };
			RectF innerRect = outRect.scaled(0.8);
			if (mGrid[index][0]) {
				Quad(outRect.br(), innerRect.br(), innerRect.tr(), outRect.tr()).draw(color);
			}
			if (mGrid[index][2]) {
				Quad(outRect.tl(), innerRect.tl(), innerRect.bl(), outRect.bl()).draw(color);
			}
			if (mGrid[index][1]) {
				Quad(outRect.bl(), innerRect.bl(), innerRect.br(), outRect.br()).draw(color);
			}
			if (mGrid[index][3]) {
				Quad(outRect.tr(), innerRect.tr(), innerRect.tl(), outRect.tl()).draw(color);
			}
			if (mGrid[index][4]) {
				outRect.draw(color2);
			}
		}
	}
	void draw(const Size& index, const Color& color = Palette::White, const Color& color2 = ColorF(1, 0.5))const {
		Transformer2D tf{ Mat3x2::Translate(mOffset) };

		RectF outRect{ index * mOneGrid,mOneGrid };
		RectF innerRect = outRect.scaled(0.8);
		if (mGrid[index][0]) {
			Quad(outRect.br(), innerRect.br(), innerRect.tr(), outRect.tr()).draw(color);
		}
		if (mGrid[index][2]) {
			Quad(outRect.tl(), innerRect.tl(), innerRect.bl(), outRect.bl()).draw(color);
		}
		if (mGrid[index][1]) {
			Quad(outRect.bl(), innerRect.bl(), innerRect.br(), outRect.br()).draw(color);
		}
		if (mGrid[index][3]) {
			Quad(outRect.tr(), innerRect.tr(), innerRect.tl(), outRect.tl()).draw(color);
		}
		if (mGrid[index][4]) {
			outRect.draw(color2);
		}

	}

	void drawGrid(const Color& color = Palette::White)const {
		Transformer2D tf{ Mat3x2::Translate(mOffset) };
		double y_len = mOneGrid.y * mGridSize.y;
		double x_len = mOneGrid.x * mGridSize.x;
		for (size_t i = 0; i < mGridSize.x + 1; i++)
		{
			Line{ i * mOneGrid.x,0,Arg::direction = Vec2{0,y_len} }.draw(color);
		}
		for (size_t i = 0; i < mGridSize.y + 1; i++)
		{
			Line{ 0,i * mOneGrid.y,Arg::direction = Vec2{x_len,0} }.draw(color);
		}
	}

	// シリアライズに対応させるためのメンバ関数を定義する
	template <class Archive>
	void SIV3D_SERIALIZE(Archive& archive)
	{
		archive(mOffset, mOneGrid, mGridSize, mGrid);
	}

};
