#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <random>
#include <unordered_set>

// Override base class with your custom functionality
class SeedGame : public olc::PixelGameEngine
{


private:
	olc::vi2d vBlockSize = { 32,32 };
	olc::vi2d vGridDim = { 20,15 };
	std::unique_ptr<olc::Sprite> sprTile;
	std::unique_ptr<std::pair<int, int>[]> aBlocks;
	olc::vi2d vBlockOrigin = { 32,32 };
	int iMineCount;

protected:

	void DrawSquare(int i, int j, int kind)
	{
		DrawPartialSprite(vBlockOrigin + olc::vi2d(i, j) * vBlockSize, 
			sprTile.get(),
			olc::vi2d(kind, 0) * vBlockSize, vBlockSize);
	}

	void DrawSquares()
	{
		for (int y = 0; y < vGridDim.y; y++)
		{
			for (int x = 0; x < vGridDim.x; x++)
			{
				DrawSquare(x, y, aBlocks[y * vGridDim.x + x].second);
			}
		}
	}

	int Get(int i, int j)
	{
		if (i >= 0 && j >= 0 && i < vGridDim.x && j < vGridDim.y)
		{
			return aBlocks[j * vGridDim.x + i].first;
		}
		return 0;
	}

	int AdjacientMines(olc::vi2d p)
	{
		int count = 0;
		int i = p.x;
		int j = p.y;
		count += Get(i + 1, j + 1);
		count += Get(i + 1, j);
		count += Get(i + 1, j - 1);
		count += Get(i, j + 1);
		count += Get(i, j - 1);
		count += Get(i - 1, j + 1);
		count += Get(i - 1, j);
		count += Get(i - 1, j - 1);
		return count;
	}

	void FloodMineless(int i, int j)
	{
		//std::unordered_set<olc::vi2d> seen;
		std::vector<olc::vi2d> v;
		//seen.insert({ i, j });
		v.push_back({ i, j });
		while (v.size() > 0)
		{
			olc::vi2d item = v.back();
			v.pop_back();
			int count = AdjacientMines(item);
			int index = item.y * vGridDim.x + item.x;
			if (count > 0)
			{
				aBlocks[index].second = 3 + count;
			}
			else
			{
				aBlocks[index].second = 1;
			}
		}
	}

	void ClickGrid(int i, int j, int button)
	{
		int index = j * vGridDim.x + i;
		std::pair<int, int> p = aBlocks[index];
		int val = p.second;
		switch (button + val * 3)
		{
		case 0:
			if (p.first == 0)
			{
				FloodMineless(i, j);
			}
			else
			{
				aBlocks[index].second = 2;
			}
			break;
		case 1:
			aBlocks[index].second = 3;
			break;
		case 10:
			aBlocks[index].second = 0;
			break;
		}
	}

	void Click(olc::vi2d mouse_pos, int button)
	{
		int i = mouse_pos.x - vBlockOrigin.x;
		int j = mouse_pos.y - vBlockOrigin.y;
		if (i >= 0 && j >= 0)
		{
			i = i / vBlockSize.x;
			j = j / vBlockSize.y;
			if (i < vGridDim.x && j < vGridDim.y)
			{
				ClickGrid(i, j, button);
			}
		}
	}

public:
	SeedGame()
	{
		// Name your application
		iMineCount = 40;
		sAppName = "SeedGame";
	}

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		sprTile = std::make_unique<olc::Sprite>("./gfx/square_32.png");
		int total = vGridDim.x * vGridDim.y;
		aBlocks = std::make_unique<std::pair<int, int>[]>(total);
		std::mt19937 gen(time(NULL));
		std::unordered_set<int> elems;
		for (int r = total - iMineCount; r < total; ++r) {
			int v = std::uniform_int_distribution<>(1, r)(gen);

			// there are two cases.
			// v is not in candidates ==> add it
			// v is in candidates ==> well, r is definitely not, because
			// this is the first iteration in the loop that we could've
			// picked something that big.

			if (!elems.insert(v).second) {
				v = r;
				elems.insert(v);
			}
			aBlocks[v] = { 1, 0 };
		}
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		if (GetMouse(1).bPressed)
		{
			Click(GetMousePos(), 1);
		}
		if (GetMouse(0).bPressed)
		{
			Click(GetMousePos(), 0);
		}
		DrawSquares();
		return true;
	}

};

int main()
{
	SeedGame demo = SeedGame();
	if (demo.Construct(896, 640, 1, 1))
		demo.Start();
	return 0;
}