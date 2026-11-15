#pragma once

// Game resource registration (textures, sprites, flipbooks, sounds).
// ResMgr only loads/caches; what the game uses is decided here.
namespace GameResources
{
	void LoadAll();

	// Textures
	void LoadTextures();

	// Sprites
	void LoadSprites();

	// Flipbooks
	void LoadPlayer();
	void LoadNpc();
	void LoadMonster();
	void LoadObjects();
	void LoadProjectiles();
	void LoadEffect();

	// Sounds
	void LoadSounds();
}
