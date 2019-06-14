#include "olcConsoleGameEngine.h"
using namespace std;

class game : public olcConsoleGameEngine {
public:
	game() {
		m_sAppName = L"Daniel Wright - Computer Science Culminating";
	}
private:
	wstring gLevel;
	int gLevelW;
	int gLevelH;
	float gCamX = 0.0f;
	float gCamY = 0.0f;
	float gPlayerX = 0.0f;
	float gPlayerY = 0.0f;
	float gPlayerVelX = 0.0f;
	float gPlayerVelY = 0.0f;
	bool gOnGround = false;
	bool isJumping = false;
	float jumpspeed = 24.0f;
	float currentJumpSpeed = jumpspeed;
	int gDirModX = 0;
	int gDirModY = 0;
public:
	bool OnUserCreate() override {

		gLevelW = 64;
		gLevelH = 16;
		gLevel += L"................................................................";
		gLevel += L"................................................##..............";
		gLevel += L"..............................................##................";
		gLevel += L"............................................##..................";
		gLevel += L"..........................................##....................";
		gLevel += L"..............##############..########################..........";
		gLevel += L"..............######.......#..#.................................";
		gLevel += L".............#######.......#..#.................................";
		gLevel += L"############################..##############.....###############";
		gLevel += L".......................#####...................##...............";
		gLevel += L".....................##......................##.................";
		gLevel += L"...................##......##################...................";
		gLevel += L".....##############......##.....................................";
		gLevel += L".....#.................##.......................................";
		gLevel += L".....#...............##.........................................";
		gLevel += L".....################...........................................";

		return true;
	}

	bool OnUserUpdate(float gElapsedTime) override {
		auto GetTile = [&](int x, int y) {
			if (x >= 0 && x < gLevelW && y >= 0 && y < gLevelH)
				return gLevel[y * gLevelW + x];
			else
				return L' ';
		};

		auto SetTile = [&](int x, int y, wchar_t a) {
			if (x >= 0 && x < gLevelW && y >= 0 && y < gLevelH)
				gLevel[y * gLevelW + x] = a;
		};

		gPlayerVelX = 0.0f;
		gPlayerVelY = 0.0f;
		if (IsFocused()) {
			if (GetKey(VK_UP).bHeld) {
				gPlayerVelY = -6.0f;
			}
			if (GetKey(VK_DOWN).bHeld) {
				gPlayerVelY = 6.0f;
			}
			if (GetKey(VK_LEFT).bHeld) {
				gPlayerVelX += (gOnGround ? -75.0f : -75.0f) * gElapsedTime;
				gDirModY = 1;
			}
			if (GetKey(VK_RIGHT).bHeld) {
				gPlayerVelX += (gOnGround ? 75.0f : 75.0f) * gElapsedTime;
				gDirModY = 0;
			}
			if (GetKey(VK_SPACE).bHeld)
			{
				if (gPlayerVelY == 0) {
					gPlayerVelY = -12.0f;
					gDirModY = 0;
				}
			}
		}

		

		gPlayerVelY += 100.0f * gElapsedTime;

		if (gOnGround) {
			gPlayerVelX += -3.0f * gPlayerVelX * gElapsedTime;
			if (fabs(gPlayerVelX) < 0.01f)
				gPlayerVelX = 0.0f;
		}

		if (gPlayerVelX > 10.0f)
			gPlayerVelX = 10.0f;

		if (gPlayerVelX < -10.0f)
			gPlayerVelX = -10.0f;

		if (gPlayerVelY > 100.0f)
			gPlayerVelY = 100.0f;

		if (gPlayerVelY < -100.0f)
			gPlayerVelY = -100.0f;

		float gNewPlayerPosX = gPlayerX + gPlayerVelX * gElapsedTime;
		float gNewPlayerPosY = gPlayerY + gPlayerVelY * gElapsedTime;

		if (gPlayerVelX <= 0) {
			if (GetTile(gNewPlayerPosX + 0.0f, gPlayerY + 0.0f) != L'.' || GetTile(gNewPlayerPosX + 0.0f, gPlayerY + 0.9f) != L'.') {
				gNewPlayerPosX = (int)gNewPlayerPosX + 1;
				gPlayerVelX = 0;
			}
		}
		else {
			if (GetTile(gNewPlayerPosX + 1.0f, gPlayerY + 0.0f) != L'.' || GetTile(gNewPlayerPosX + 1.0f, gPlayerY + 0.9f) != L'.') {
				gNewPlayerPosX = (int)gNewPlayerPosX;
				gPlayerVelX = 0;

			}
		}

		gOnGround = false;
		if (gPlayerVelY <= 0) {
			if (GetTile(gNewPlayerPosX + 0.0f, gNewPlayerPosY) != L'.' || GetTile(gNewPlayerPosX + 0.9f, gNewPlayerPosY) != L'.') {
				gNewPlayerPosY = (int)gNewPlayerPosY + 1;
				gPlayerVelY = 0;
			}
		}
		else {
			if (GetTile(gNewPlayerPosX + 0.0f, gNewPlayerPosY + 1.0f) != L'.' || GetTile(gNewPlayerPosX + 0.9f, gNewPlayerPosY + 1.0f) != L'.') {
				gNewPlayerPosY = (int)gNewPlayerPosY;
				gPlayerVelY = 0;
				gOnGround = true;
				gDirModX = 0;
			}
		}

		gPlayerX = gNewPlayerPosX;
		gPlayerY = gNewPlayerPosY;

		gCamX = gPlayerX;
		gCamY = gPlayerY;


		int gTileW = 16;
		int gTileH = 16;
		int gVisibleTilesX = ScreenWidth() / gTileW;
		int gVisibleTilesY = ScreenHeight() / gTileH;
		float gOffsetX = gCamX - (float)gVisibleTilesX / 2.0f;
		float gOffsetY = gCamY - (float)gVisibleTilesY / 2.0f;
		if (gOffsetX < 0) gOffsetX = 0;
		if (gOffsetY < 0) gOffsetY = 0;
		if (gOffsetX > gLevelW - gVisibleTilesX) gOffsetX = gLevelW - gVisibleTilesX;
		if (gOffsetY > gLevelH - gVisibleTilesY) gOffsetY = gLevelH - gVisibleTilesY;

		for (int x = 0; x < gVisibleTilesX; x++) {
			for (int y = 0; y < gVisibleTilesY; y++) {
				wchar_t gTileID = GetTile(x + gOffsetX, y + gOffsetY);
				switch (gTileID) {
				case L'.':
					Fill(x * gTileW, y * gTileH, (x + 1) * gTileW, (y + 1) * gTileH, PIXEL_SOLID, FG_CYAN);
					break;
				case L'#':
					Fill(x * gTileW, y * gTileH, (x + 1) * gTileW, (y + 1) * gTileH, PIXEL_SOLID, FG_RED);
					break;
				default:
					break;
				}
			}
		}

		Fill((gPlayerX - gOffsetX) * gTileW, (gPlayerY - gOffsetY) * gTileW, (gPlayerX - gOffsetX + 1.0f) * gTileW, (gPlayerY - gOffsetY + 1.0f) * gTileH, PIXEL_SOLID, FG_GREEN);
		return true;
		
	}


};


int main()
{
	game game;
	if (game.ConstructConsole(256, 240, 4, 4))
		game.Start();
	return 0;

	
	
}