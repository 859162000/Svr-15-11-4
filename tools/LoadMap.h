#ifndef _LOADMAP_H_
#define _LOADMAP_H_

#include <vector>
#include "zType.h"

#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
	((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |       \
	 ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))
#endif /* defined(MAKEFOURCC) */

#define MAP_MAGIC (MAKEFOURCC('X','M','P','S'))
#define MAP_VERSION 6 

struct stMapFileHeader
{
	DWORD magic;			/**< �ļ���ʶ  MAP_MAGIC */
	DWORD ver;				/**< �汾 MAP_VERSION */
	DWORD width;			/**< ��� */
	DWORD height;			/**< �߶� */
};

/**
 * \brief ���Ӷ���
 */
#define TILE_BLOCK			0x01	// �赲��
#define TILE_MAGIC_BLOCK	0x02	// ħ���赲��
#define TILE_NOCREATE		0x04	// ���ܽ����
#define TILE_DOOR			0x08	// ��
#define TILE_ENTRY_BLOCK	0x10	// �������Npc�赲
#define TILE_OBJECT_BLOCK	0x20	// ��Ʒ�赲
#pragma  pack(1)
struct stSrvMapTile
{
	BYTE	flags;	// ��������
	BYTE	type;	// ��������
};
#pragma pack()

typedef std::vector<stSrvMapTile> zTiles;

bool LoadMap(const char* pszFileName,zTiles & aTiles,DWORD & width,DWORD & height);
#endif
