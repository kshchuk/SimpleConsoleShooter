#include "Rendering.h"


void Rendering::Collision(Player& player, float fElapsedTime, char key, const std::vector<std::vector<bool>>& map)
{
	if (key == 'W')
	{
		if (sinf(player.rotation) > 0 && cosf(player.rotation) > 0) {
			player.x += sinf(player.rotation) * player.speed * fElapsedTime;
			if (map[(int)player.x][(int)player.y]) {
				player.x -= sinf(player.rotation) * player.speed * fElapsedTime;
				player.y += cosf(player.rotation) * player.speed * fElapsedTime;
			}
			if (map[(int)player.x][(int)player.y])
				player.y -= cosf(player.rotation) * player.speed * fElapsedTime;

		}
		else if (sinf(player.rotation) > 0 && cosf(player.rotation) < 0) {
			player.x += sinf(player.rotation) * player.speed * fElapsedTime;
			if (map[(int)player.x][(int)player.y]) {
				player.x -= sinf(player.rotation) * player.speed * fElapsedTime;
				player.y += cosf(player.rotation) * player.speed * fElapsedTime;
			}
			if (map[(int)player.x][(int)player.y])
				player.y -= cosf(player.rotation) * player.speed * fElapsedTime;
		}
		else if (sinf(player.rotation) < 0 && cosf(player.rotation) < 0) {
			player.y += sinf(player.rotation) * player.speed * fElapsedTime;
			if (map[(int)player.x][(int)player.y]) {
				player.y -= sinf(player.rotation) * player.speed * fElapsedTime;
				player.x += cosf(player.rotation) * player.speed * fElapsedTime;
			}
			if (map[(int)player.x][(int)player.y])
				player.x -= cosf(player.rotation) * player.speed * fElapsedTime;
		}
		else if (sinf(player.rotation) < 0 && cosf(player.rotation) > 0) {
			player.x += sinf(player.rotation) * player.speed * fElapsedTime;
			if (map[(int)player.x][(int)player.y]) {
				player.x -= sinf(player.rotation) * player.speed * fElapsedTime;
				player.y += cosf(player.rotation) * player.speed * fElapsedTime;
			}
			if (map[(int)player.x][(int)player.y])
				player.y -= cosf(player.rotation) * player.speed * fElapsedTime;
		}

	}
	else
	{ // key == 'S'
		if (sinf(player.rotation) > 0 && cosf(player.rotation) > 0) {
			player.x -= sinf(player.rotation) * player.speed * fElapsedTime;
			if (map[(int)player.x][(int)player.y]) {
				player.x += sinf(player.rotation) * player.speed * fElapsedTime;
				player.y -= sinf(player.rotation) * player.speed * fElapsedTime;
			}
			if (map[(int)player.x][(int)player.y])
				player.y += sinf(player.rotation) * player.speed * fElapsedTime;
		}
		else if (sinf(player.rotation) > 0 && cosf(player.rotation) < 0) {
			player.x -= sinf(player.rotation) * player.speed * fElapsedTime;
			if (map[(int)player.x][(int)player.y]) {
				player.x += sinf(player.rotation) * player.speed * fElapsedTime;
				player.y -= cosf(player.rotation) * player.speed * fElapsedTime;
			}
			if (map[(int)player.x][(int)player.y])
				player.y += cosf(player.rotation) * player.speed * fElapsedTime;
		}
		else if (sinf(player.rotation) < 0 && cosf(player.rotation) < 0) {
			player.y -= sinf(player.rotation) * player.speed * fElapsedTime;
			if (map[(int)player.x][(int)player.y]) {
				player.y += sinf(player.rotation) * player.speed * fElapsedTime;
				player.x -= cosf(player.rotation) * player.speed * fElapsedTime;
			}
			if (map[(int)player.x][(int)player.y])
				player.x += cosf(player.rotation) * player.speed * fElapsedTime;
		}
		else if (sinf(player.rotation) < 0 && cosf(player.rotation) > 0) {
			player.x -= sinf(player.rotation) * player.speed * fElapsedTime;
			if (map[(int)player.x][(int)player.y]) {
				player.x += sinf(player.rotation) * player.speed * fElapsedTime;
				player.y -= cosf(player.rotation) * player.speed * fElapsedTime;
			}
			if (map[(int)player.x][(int)player.y])
				player.y += cosf(player.rotation) * player.speed * fElapsedTime;
		}
	}
}

void Rendering::CalculatePosition(Player& player, Configs& conf, const float fElapsedTime, const std::vector<std::vector<bool>>& map, ClientGame* client, 
	std::chrono::system_clock::time_point& last_firing_time)
{
	// Handle CCW Rotation
	if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
		player.rotation -= (player.speed * 0.5f) * fElapsedTime;

	// Handle CW Rotation
	if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
		player.rotation += (player.speed * 0.5f) * fElapsedTime;

	// Handle Forwards movement & collision
	if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
	{
		player.x += sinf(player.rotation) * player.speed * fElapsedTime;
		player.y += cosf(player.rotation) * player.speed * fElapsedTime;

		// Collision
		if (map[(int)player.x][(int)player.y])
		{
			player.x -= sinf(player.rotation) * player.speed * fElapsedTime;
			player.y -= cosf(player.rotation) * player.speed * fElapsedTime;

			Collision(player, fElapsedTime, 'W', map);
		}
	}

	// Handle backwards movement & collision
	if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
	{
		player.x -= sinf(player.rotation) * player.speed * fElapsedTime;
		player.y -= cosf(player.rotation) * player.speed * fElapsedTime;

		// Collision
		if (map[(int)player.x][(int)player.y])
		{
			player.x += sinf(player.rotation) * player.speed * fElapsedTime;
			player.y += cosf(player.rotation) * player.speed * fElapsedTime;

			Collision(player, fElapsedTime, 'S', map);
		}
	}

	// Handle firing
	if (GetAsyncKeyState((unsigned short)VK_RETURN) & 0x8000)
	{
		std::chrono::duration<float> cur_reloading_time = std::chrono::system_clock::now() - last_firing_time;
		float fcur_reloading_time = cur_reloading_time.count();
		if (fcur_reloading_time > conf.gunReloading_seconds)
			last_firing_time = std::chrono::system_clock::now();
		if (client) {
			client->sendShootingInfo();
		}
	}
}

void Rendering::RenderFrame(const Configs& conf, Player& player,
	const std::vector<std::vector<bool>>& map, wchar_t* screen,
	HANDLE hConsole, DWORD dwBytesWritten,
	std::chrono::system_clock::time_point& tp1, std::chrono::system_clock::time_point& tp2,
	float fElapsedTime, Textures* textures, const std::chrono::system_clock::time_point& last_firing_time,
	const std::map<int, Player*>* other_players)
{

	for (int x = 0; x < conf.screenWidth; x++)
	{
		// For each column, calculate the projected ray angle into world space
		float fRayAngle = (player.rotation - player.FOV / 2.0f) + ((float)x / (float)conf.screenWidth) * player.FOV;

		// Find distance to wall
		float fStepSize = 0.1f;		      // Increment size for ray casting, decrease to increase resolution						
		float fDistanceToObstacle = 0.0f; //														

		bool bHitWall = false;			// Set when ray hits wall block
		bool bWallBoundary = false;		// Set when ray hits boundary between two wall blocks
		bool bHitPlayer = false;		// Set when ray hits player silhouette
		bool bPlayerBoundary = false;	// Set when ray hits boundary between two sides of player silhouette

		float fEyeX = sinf(fRayAngle); // Unit vector for ray in player space
		float fEyeY = cosf(fRayAngle);

		// Incrementally cast ray from player, along ray angle, testing for 
		// intersection with a block or a player
		while (!bHitWall && !bHitPlayer &&fDistanceToObstacle < player.depth)
		{
			fDistanceToObstacle += fStepSize;
			float fTestX = player.x + fEyeX * fDistanceToObstacle;
			float fTestY = player.y + fEyeY * fDistanceToObstacle;

			// Test if ray is out of bounds
			if (fTestX < 0 || fTestX >= conf.mapWidth || fTestY < 0 || fTestY >= conf.mapHeight)
			{
				bHitWall = true;			// Just set distance to maximum depth
				fDistanceToObstacle = player.depth;
			}
			else 
				// Ray is inbounds so test to see if the ray cell is a wall block
				if (map[(int)fTestX][(int)fTestY])
				{
					// Ray has hit wall
					bHitWall = true;

					// To highlight tile boundaries, cast a ray from each corner
					// of the tile, to the player. The more coincident this ray
					// is to the rendering ray, the closer we are to a tile 
					// boundary, which we'll shade to add detail to the walls
					std::vector<std::pair<float, float>> p;

					// Test each corner of hit tile, storing the distance from
					// the player, and the calculated dot product of the two rays
					for (int tx = 0; tx < 2; tx++)
						for (int ty = 0; ty < 2; ty++)
						{
							// Angle of corner to eye
							float vy = (int)fTestY + ty - player.y;
							float vx = (int)fTestX + tx - player.x;
							float d = sqrt(vx * vx + vy * vy);
							float dot = (fEyeX * vx / d) + (fEyeY * vy / d);
							p.push_back(std::make_pair(d, dot));
						}

					// Sort Pairs from closest to farthest
					sort(p.begin(), p.end(), [](const std::pair<float, float>& left, const std::pair<float, float>& right) {return left.first < right.first; });

					// First two/three are closest (we will never see all four)
					float fBound = 0.01;
					if (acos(p.at(0).second) < fBound) bWallBoundary = true;
					if (acos(p.at(1).second) < fBound) bWallBoundary = true;
					if (acos(p.at(2).second) < fBound) bWallBoundary = true;
				}
				else // Ray is inbounds so test to see if the ray cell is a player
				{
					if (other_players)
					for (auto iter : *other_players) 
					{
						Player* cur_player = iter.second;
						if (distance(cur_player->x, cur_player->y, player.x, player.y) > conf.depth)	// if player isn't in the reach zone
							continue;
						else 
						{
							cur_player->CaclulatePositions();
							// Test to see if the ray cell is in the player 
							// Check all sides of the player
							if (cross(player.x, player.y, fTestX, fTestY,
								cur_player->leftFront_pos.x, cur_player->leftFront_pos.y,
								cur_player->rightFront_pos.x, cur_player->rightFront_pos.y, fTestX, fTestY) ||

								cross(player.x, player.y, fTestX, fTestY,
									cur_player->leftFront_pos.x, cur_player->leftFront_pos.y,
									cur_player->leftBack_pos.x, cur_player->leftBack_pos.y, fTestX, fTestY) ||

								cross(player.x, player.y, fTestX, fTestY,
									cur_player->leftBack_pos.x, cur_player->leftBack_pos.y,
									cur_player->rightBack_pos.x, cur_player->rightBack_pos.y, fTestX, fTestY) ||

								cross(player.x, player.y, fTestX, fTestY,
									cur_player->rightBack_pos.x, cur_player->rightBack_pos.y,
									cur_player->rightFront_pos.x, cur_player->rightFront_pos.y, fTestX, fTestY))
							{
								bHitPlayer = true;
								break;
							}
						}
					}
					if (bHitPlayer) 
					{
						// To highlight tile boundaries, cast a ray from each corner
						// of the player, to the camera. The more coincident this ray
						// is to the rendering ray, the closer we are to a player 
						// boundary, which we'll shade to add detail to the silhouette
						std::vector<std::pair<float, float>> p;

						// Test each corner of hit player, storing the distance from
						// the camera, and the calculated dot product of the two rays
						for (int tx = 0; tx < 2; tx++)
							for (int ty = 0; ty < 2; ty++)
							{
								// Angle of corner to eye
								float vy = fTestY + ty - player.y;
								float vx = fTestX + tx - player.x;
								float d = sqrt(vx * vx + vy * vy);
								float dot = (fEyeX * vx / d) + (fEyeY * vy / d);
								p.push_back(std::make_pair(d, dot));
							}

						// Sort Pairs from closest to farthest
						sort(p.begin(), p.end(), [](const std::pair<float, float>& left, const std::pair<float, float>& right) {return left.first < right.first; });

						// First two/three are closest (we will never see all four)
						float fBound = 0.01;
						if (acos(p.at(0).second) < fBound) bPlayerBoundary = true;
						if (acos(p.at(1).second) < fBound) bPlayerBoundary = true;
						if (acos(p.at(2).second) < fBound) bPlayerBoundary = true;
					}
				}
		}

		// Calculate distance to ceiling and floor
		int nCeiling = (float)(conf.screenHeight / 2.0) - conf.screenHeight / ((float)fDistanceToObstacle);
		int nFloor = conf.screenHeight - nCeiling;

		short nShade = ' ';

		if (!bHitPlayer)
		{
			// Shader walls based on distance
			if (fDistanceToObstacle <= player.depth / 4.0f)			nShade = 0x2588;	// Very close	
			else if (fDistanceToObstacle < player.depth / 3.0f)		nShade = 0x2593;
			else if (fDistanceToObstacle < player.depth / 2.0f)		nShade = 0x2592;
			else if (fDistanceToObstacle < player.depth)			nShade = 0x2591;
			else													nShade = ' ';		// Too far away

			if (bWallBoundary)		nShade = ' '; // Black it out
		}
		else
		{
			// Shader player based on distance
			if (fDistanceToObstacle <= player.depth / 4.0f)			nShade = 0x2584;	// Very close	
			else if (fDistanceToObstacle < player.depth / 3.0f)		nShade = 0x2663;
			else if (fDistanceToObstacle < player.depth / 2.0f)		nShade = 0x25BC;
			else if (fDistanceToObstacle < player.depth)			nShade = 0x2642;
			else													nShade = ' ';		// Too far away

			if (bPlayerBoundary)		nShade = ' '; // make border
		}

		for (int y = 0; y < conf.screenHeight; y++)
		{
			// Each Row
			if (y <= nCeiling)
				screen[y * conf.screenWidth + x] = ' ';
			else if (y > nCeiling && y <= nFloor)
				screen[y * conf.screenWidth + x] = nShade;
			else // Floor
			{
				// Shade floor based on distance
				float b = 1.0f - (((float)y - conf.screenHeight / 2.0f) / ((float)conf.screenHeight / 2.0f));
				if (b < 0.25)		nShade = '#';
				else if (b < 0.5)	nShade = 'x';
				else if (b < 0.75)	nShade = '.';
				else if (b < 0.9)	nShade = '-';
				else				nShade = ' ';
				screen[y * conf.screenWidth + x] = nShade;
			}
		}
	}

	// Display the sight
	size_t map_center_x = conf.screenWidth / 2,
		map_center_y = conf.screenHeight / 2;

	size_t sight_thickness_v = conf.screenWidth / 100,
		sight_thickness_h = conf.screenHeight / 50;

	size_t left_top_vline_corner_x = map_center_x - sight_thickness_v / 2,
		left_top_vline_corner_y = map_center_y - conf.screenHeight / 40,
		left_bottom_vline_corner_y = map_center_y + conf.screenHeight / 40,

		left_top_hline_corner_x = map_center_x - conf.screenWidth / 80,
		left_top_hline_corner_y = map_center_y - sight_thickness_h / 2,
		right_top_hline_corner_x = map_center_x + conf.screenWidth / 80;
	
	for (int ny = left_top_vline_corner_y; ny < left_bottom_vline_corner_y; ny++)
		for (int nx = left_top_vline_corner_x; nx < map_center_x + sight_thickness_v / 2; nx++)
		{
			screen[ny * conf.screenWidth + nx] = 0x263B;
		}

	for (int ny = left_top_hline_corner_y; ny < map_center_y + sight_thickness_h / 2; ny++)
		for (int nx = left_top_hline_corner_x; nx < right_top_hline_corner_x; nx++)
		{
			screen[ny * conf.screenWidth + nx] = 0x263B;
		}


	// Display the health line
	size_t health_line_thickness = conf.screenHeight / 50;
	size_t health_line_width = conf.screenWidth * player.health / 100;
	for (int ny = conf.screenHeight - health_line_thickness - 1; ny < conf.screenHeight; ny++) {
		for (int nx = 0; nx < health_line_width; nx++)
			screen[ny * conf.screenWidth + nx] = 0x2588;
		for (int nx = health_line_width; nx < conf.screenWidth; nx++)
			screen[ny * conf.screenWidth + nx] = 0x2661;
	}

	
	// Display explosions
	std::chrono::duration<float> cur_reloading_time = std::chrono::system_clock::now() - last_firing_time;
	float fcur_reloading_time = cur_reloading_time.count();

	if (fcur_reloading_time > 0 && fcur_reloading_time < 0.1) // Small explosion
	{
		int expl_step_x = conf.screenWidth / 70,
			expl_step_y = conf.screenHeight / 40;

		for (int ty = textures->small_explosion_file_height, y = conf.screenHeight / 4 * 3; ty > 0; ty -= expl_step_y, y--)
			for (int tx = 1, x = map_center_x + conf.screenWidth / 8; tx < textures->small_explosion_file_width; tx += expl_step_x, x++) {
				if (textures->small_explosion[ty - 1][tx - 1] != ' ')
					screen[y * conf.screenWidth + x] = textures->small_explosion[ty - 1][tx - 1];
				else
					continue;
			}
	}
	else 
		if (fcur_reloading_time > 0.1 && fcur_reloading_time < 0.2) // Middle explosion
		{
			int expl_step_x = conf.screenWidth / 70,
				expl_step_y = conf.screenHeight / 40;

			for (int ty = textures->middle_explosion_file_height, y = conf.screenHeight / 4 * 3.2; ty > 0; ty -= expl_step_y, y--)
				for (int tx = 1, x = map_center_x + conf.screenWidth / 9; tx < textures->middle_explosion_file_width; tx += expl_step_x, x++) {
					if (textures->middle_explosion[ty - 1][tx - 1] != ' ')
						screen[y * conf.screenWidth + x] = textures->middle_explosion[ty - 1][tx - 1];
					else
						continue;
				}
		}
		else
			if (fcur_reloading_time > 0.2 && fcur_reloading_time < 0.3) // Big explosion
			{
				int expl_step_x = conf.screenWidth / 70,
					expl_step_y = conf.screenHeight / 40;

				for (int ty = textures->big_explosion_file_height, y = conf.screenHeight / 4 * 3.3; ty > 0; ty -= expl_step_y, y--)
					for (int tx = 1, x = map_center_x + conf.screenWidth / 10; tx < textures->big_explosion_file_width; tx += expl_step_x, x++) {
						if (textures->big_explosion[ty - 1][tx - 1] != ' ')
							screen[y * conf.screenWidth + x] = textures->big_explosion[ty - 1][tx - 1];
						else
							continue;
					}
			}

	// Distplay the gun
	int gun_step_x = conf.screenWidth / 90,
		gun_step_y = conf.screenHeight / 40;

	for (int ty = textures->gun_file_height - 1, y = conf.screenHeight - health_line_thickness*2; ty > 0; ty -= gun_step_y, y--)
		for (int tx = 1, x = map_center_x + conf.screenWidth / 10; tx < textures->gun_file_width; tx += gun_step_x, x++) {
			if (textures->gun[ty][tx-1] != '&')
				screen[y * conf.screenWidth + x] = textures->gun[ty][tx-1];
			else
				continue;
		}



	// Display Stats
	swprintf_s(screen, 50, L"X=%3.2f, Y=%3.2f, A=%3.2f FPS=%3.2f ", player.x, player.y, player.rotation, 1.0f / fElapsedTime);

	// Display Map
	for (int nx = 0; nx < conf.mapWidth; nx++)
		for (int ny = 0; ny < conf.mapWidth; ny++)
		{
			screen[(ny + 1) * conf.screenWidth + nx] = map[ny][nx] ? '#' : ' ';
		}
	screen[((int)player.x + 1) * conf.screenWidth + (int)player.y] = 'P';

	// Display Frame
	screen[conf.screenWidth * conf.screenHeight - 1] = '\0';
	WriteConsoleOutputCharacter(hConsole, screen, conf.screenWidth * conf.screenHeight, { 0,0 }, &dwBytesWritten);
}

float distance(float x1, float y1, float x2, float y2)
{
	return sqrtf(powf((x1 - x2), 2) + powf((y1 - y2), 2));
}

bool cross(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float& x, float& y)
{
	float Ua, Ub, numerator_a, numerator_b, denominator;

	denominator = (y4 - y3) * (x1 - x2) - (x4 - x3) * (y1 - y2);

	if (denominator == 0) {
		return false;
	}
	else {
		numerator_a = (x4 - x2) * (y4 - y3) - (x4 - x3) * (y4 - y2);
		numerator_b = (x1 - x2) * (y4 - y2) - (x4 - x2) * (y1 - y2);
		Ua = numerator_a / denominator;
		Ub = numerator_b / denominator;

		if (Ua >= 0 && Ua <= 1 && Ub >= 0 && Ub <= 1) {
			x = x1 + x2 * Ua;
			y = y1 + y2 * Ua;
			return true;
		}
		else
			return false;
	}
}