
//{{BLOCK(Background)

//======================================================================
//
//	Background, 256x256@4, 
//	+ palette 256 entries, not compressed
//	+ 8 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 256 + 2048 = 2816
//
//	Time-stamp: 2016-05-25, 17:37:28
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_BACKGROUND_H
#define GRIT_BACKGROUND_H

#define BackgroundTilesLen 256
extern const unsigned int BackgroundTiles[64];

#define BackgroundMapLen 2048
extern const unsigned int BackgroundMap[512];

#define BackgroundPalLen 512
extern const unsigned int BackgroundPal[128];

#endif // GRIT_BACKGROUND_H

//}}BLOCK(Background)
