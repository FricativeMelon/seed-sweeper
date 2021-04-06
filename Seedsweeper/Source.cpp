#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

// Override base class with your custom functionality
class SeedGame : public olc::PixelGameEngine
{


private:
	olc::vi2d vBlockSize = { 32,32 };
	olc::vi2d vGridDim = { 20,15 };
	std::unique_ptr<olc::Sprite> sprTile;
	std::unique_ptr<int[]> aBlocks;
	olc::vi2d vBlockOrigin = { 32,32 };

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
				DrawSquare(x, y, aBlocks[y * vGridDim.x + x]);
			}
		}
	}

	void ClickGrid(int i, int j, int button)
	{
		int index = j * vGridDim.x + i;
		int val = aBlocks[index];
		switch (button + val * 3)
		{
		case 0:
			aBlocks[index] = 1;
			break;
		case 1:
			aBlocks[index] = 3;
			break;
		case 10:
			aBlocks[index] = 0;
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
		sAppName = "SeedGame";
	}

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		sprTile = std::make_unique<olc::Sprite>("./gfx/square_32.png");
		aBlocks = std::make_unique<int[]>(vGridDim.x * vGridDim.y);
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