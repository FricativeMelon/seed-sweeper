#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

// Override base class with your custom functionality
class SeedGame : public olc::PixelGameEngine
{


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
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
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