/**
 * \file
 * \version  $Id: zSceneEntryIndex.h  $
 * \author  
 * \date 
 * \brief ��ͼ�������������
 */

#ifndef _ZSCENEENTRYINDEX_H_
#define _ZSCENEENTRYINDEX_H_
#include <set>
#include <map>
#include <ext/hash_map>
#include <ext/pool_allocator.h>
#include "zSceneEntry.h"

#define MAX_NPC_GROUP 5
class SceneUser;
class SceneNpc;
/**
 * \brief ��������Ļص���
 *
 * ֻҪ�����ڱ���һ�����,����ĳЩ�������
 *
 * ʹ���߱���Ҫʵ��exec����
 */
struct zSceneEntryCallBack
{
	virtual bool exec(zSceneEntry *entry)=0;
	virtual ~zSceneEntryCallBack(){};
};

/**
 * \brief �������������
 */
class zSceneEntryIndex : private zNoncopyable
{

	protected:

		///������͸�
		zPos sceneWH;
		///������������
		DWORD screenMax;
		///���������Ļ
		DWORD screenx;
		///���������Ļ
		DWORD screeny;

	private:

#ifdef _POOL_ALLOC_
		typedef std::set<zSceneEntry *, std::less<zSceneEntry *>, __gnu_cxx::__pool_alloc<zSceneEntry *> > SceneEntry_SET;
		typedef std::set<SceneNpc *, std::less<SceneNpc *>, __gnu_cxx::__pool_alloc<SceneNpc *> > Npc_Index;
#else
		typedef std::set<zSceneEntry *, std::less<zSceneEntry *> > SceneEntry_SET;
		typedef std::set<SceneNpc *, std::less<SceneNpc *> > Npc_Index;
#endif


		typedef __gnu_cxx::hash_map<zPosI, SceneEntry_SET> PosIMapIndex;
		typedef __gnu_cxx::hash_map<DWORD, Npc_Index> SpecialNpc_Index;

		/**
		 * \brief map��������
		 */
		PosIMapIndex index[zSceneEntry::SceneEntry_MAX];
		/// �������������
		SceneEntry_SET all[zSceneEntry::SceneEntry_MAX];
		/// ��������npc������
		SpecialNpc_Index special_index;
		/// ���ڹ���npc������
		Npc_Index functionNpc;

#ifdef _POOL_ALLOC_
		typedef std::map<DWORD, int, std::less<DWORD>, __gnu_cxx::__pool_alloc<std::pair<const DWORD, int> > > PosiEffectMap;
#else
		typedef std::map<DWORD, int, std::less<DWORD> > PosiEffectMap;
#endif

		typedef PosiEffectMap::iterator PosiEffectMap_iter;
		PosiEffectMap posiEffect[MAX_NPC_GROUP];

		//�ڼ��ص�ʱ����������ϵ������
		typedef __gnu_cxx::hash_map<DWORD, zPosIVector> NineScreen_map;
		typedef NineScreen_map::iterator NineScreen_map_iter;
		typedef NineScreen_map::const_iterator NineScreen_map_const_iter;
		typedef NineScreen_map::value_type NineScreen_map_value_type;
		NineScreen_map ninescreen;
		NineScreen_map direct_screen[8];
		NineScreen_map reversedirect_screen[8];

		void freshEffectPosi(const zPosI oldposi, const zPosI newposi);

	public:

		/**
		 * \brief ���캯��
		 */
		zSceneEntryIndex() {}
		/**
		 * \brief ��������
		 */
		virtual ~zSceneEntryIndex() {}

		virtual void freshGateScreenIndex(SceneUser *pUser , const DWORD screen) = 0;

		bool refresh(zSceneEntry *e,const zPos & newPos);
		bool removeSceneEntry(zSceneEntry *e);
		void execAllOfScreen(const zPosI screen,zSceneEntryCallBack &callback);
		void execAllOfScreen(const zSceneEntry::SceneEntryType type,const zPosI screen,zSceneEntryCallBack &callback);
		void execAllOfScene(zSceneEntryCallBack &callback);
		void execAllOfScene(const zSceneEntry::SceneEntryType type,zSceneEntryCallBack &callback);
		void execAllOfScene_npc(const DWORD id, zSceneEntryCallBack &callback);
		void execAllOfScene_functionNpc(zSceneEntryCallBack &callback);

		void setSceneWH(const zPos sceneWH, const DWORD screenx, const DWORD screeny, const DWORD screenMax);
		zSceneEntry *getSceneEntryByPos(zSceneEntry::SceneEntryType type, const zPos &pos, const bool bState = true, const zSceneEntry::SceneEntryState byState = zSceneEntry::SceneEntry_Normal);
		void execAllOfEffectNpcScreen(const DWORD group, zSceneEntryCallBack &callback);

		/**
		 * \brief ��ȡ��ͼ��
		 * ��λ�Ǹ��
		 * \return ��ͼ��
		 */
		const DWORD width() const { return sceneWH.x; }
		/**
		 * \brief ��ȡ��ͼ��
		 * ��λ�Ǹ��
		 * \return ��ͼ��
		 */
		const DWORD height() const { return sceneWH.y; }
		/**
		 * \brief ��ȡ��ͼ��
		 * ��λ����
		 * \return ��ͼ��
		 */
		const DWORD getScreenX() const { return screenx; }
		/**
		 * \brief ��ȡ��ͼ��
		 * ��λ����
		 * \return ��ͼ��
		 */
		const DWORD getScreenY() const { return screeny; }
		/**
		 * \brief ����ת��
		 * ��������ת��Ϊ������������
		 * \param p ��������ϵͳ 
		 * \param pi �������ϵͳ
		 * \return ����ת������������ϵͳ
		 */
		zPosI &zPos2zPosI(const zPos &p, zPosI &pi) const
		{
			zSceneEntry::zPos2zPosI(sceneWH,p,pi);
			return pi;
		}
		/**
		 * \brief ���¼������꣬��֤��Խ��
		 * \param pos ����֤������
		 */
		const void zPosRevaluate(zPos &pos) const
		{
			if (pos.x & 0x80000000)
				pos.x = 0;
			if (pos.y & 0x80000000)
				pos.y = 0;
			if (pos.x >= sceneWH.x)
				pos.x = sceneWH.x - 1;
			if (pos.y >= sceneWH.y)
				pos.y = sceneWH.y - 1;
		}
		/**
		 * \brief ��֤�����Ƿ�Ϸ����Ƿ�Խ��
		 * \param pos ����
		 * \return �Ƿ�Ϸ�
		 */
		const bool zPosValidate(const zPos &pos) const { return pos.x < sceneWH.x && pos.y < sceneWH.y; }
		/**
		 * \brief ��֤�����Ƿ�Ϸ����Ƿ�Խ��
		 * \param posi ����
		 * \return �Ƿ�Ϸ�
		 */
		const bool zPosIValidate(const zPosI &posi) const { return posi < screenMax; }
		/**
		 * \brief ��������������Ƿ���9����ϵ
		 * \param one ��һ�����
		 * \param two �ڶ������
		 * \return �Ƿ���9����ϵ
		 */
		bool checkTwoPosIInNine(const zPosI one , const zPosI two) const
		{
			int oneX, oneY , twoX ,twoY;
			oneX = one % screenx;
			oneY = one / screenx;
			twoX = two % screenx;
			twoY = two / screenx;
			if(abs(oneX - twoX) <=1 && abs(oneY - twoY) <=1)
			{
				return true;
			}
			return false;
		}
		/**
		 * \brief �ж�����֮������Ƿ񳬳��˰뾶
		 * \param pos1 ���
		 * \param pos2 �յ�
		 * \param radius �뾶
		 * \return �Ƿ񳬳���Χ
		 */
		const bool zPosShortRange(const zPos &pos1, const zPos &pos2, const int radius) const
		{
			if (abs(pos1.x - pos2.x) <= radius
					&& abs(pos1.y - pos2.y) <= radius)
				return true;
			else
				return false;
		}
		/**
		 * \brief �ж�����֮������Ƿ񳬳��˾��η�Χ
		 * \param pos1 ���
		 * \param pos2 �յ�
		 * \param wide ���ο�
		 * \param height ���θ�
		 * \return �Ƿ񳬳���Χ
		 */
		const bool zPosShortRange(const zPos &pos1, const zPos &pos2, const int wide, const int height) const
		{
			if (abs(pos1.x - pos2.x) <= wide
					&& abs(pos1.y - pos2.y) <= height)
				return true;
			else
				return false;
		}
		/*
		 * \brief �õ�����ľ���
		 * �������ֻ����Ϊ�Ƚ��жϣ�������Ϊʵ�ʵľ���
		 * \param pos1 λ��1
		 * \param pos2 λ��2
		 * \return ����
		 */
		unsigned int getDistance(zPos pos1, zPos pos2) const { return abs(pos1.x-pos2.x)+abs(pos1.y-pos2.y); }
		/**
		 * \brief ����ȡ��
		 * \param direct ����
		 * \return �෴�ķ���
		 */
		int getReverseDirect(const int direct) const { return (direct + 4) % 8; }
		/**
		 * \brief ���������жϾŸ�����
		 * \param posiorg ԭ�������
		 * \param posinew �����������
		 * \return �����ţ����Ϊ-1��ʾ���ǾŸ�����֮һ
		 */
		int getScreenDirect(const zPosI posiorg,const zPosI posinew) const
		{
			using namespace Cmd;
			unsigned int orgX = posiorg % screenx;
			unsigned int orgY = posiorg / screenx;
			unsigned int newX = posinew % screenx;
			unsigned int newY = posinew / screenx;
			if (orgX == newX && orgY == newY+1)
			{
				return _DIR_UP;
			}
			else if (orgX + 1 == newX && orgY == newY +1)
			{
				return _DIR_UPRIGHT;
			}
			else if (orgX +1 == newX && orgY == newY)
			{
				return _DIR_RIGHT;
			}
			else if (orgX +1 == newX && orgY +1 == newY)
			{
				return _DIR_RIGHTDOWN;
			}
			else if (orgX == newX && orgY +1 == newY)
			{
				return _DIR_DOWN;
			}
			else if (orgX == 1 + newX && orgY +1 == newY)
			{
				return _DIR_DOWNLEFT;
			}
			else if (orgX == 1 + newX && orgY == newY)
			{
				return _DIR_LEFT;
			}
			else if (orgX == 1 + newX && orgY == 1 + newY)
			{
				return _DIR_LEFTUP;
			}
			else
				return _DIR_WRONG;
		}

		/**
		 * \brief ����������һ�������Ĵ��·���
		 * \param myPos �ҵ�λ��
		 * \param otherPos �Է���λ��
		 * \return ����otherPos����Է���
		 */
		static int getCompDir(const zPos &myPos, const zPos &otherPos)
		{
			int x = otherPos.x - myPos.x;
			int y = otherPos.y - myPos.y;
			int absx = abs(x);
			int absy = abs(y);
			if (absx > absy && absy < absx/2)
				y = 0;
			else if (absx < absy && absx < absy/2)
				x = 0;

			int bkDir = 0;

			if (x < 0 &&  y < 0)
				bkDir = 7;
			else if (x < 0 && 0 == y)
				bkDir = 6;
			else if (x < 0 && y > 0)
				bkDir = 5;
			else if (0 == x && y > 0)
				bkDir = 4;
			else if (x > 0 && y > 0)
				bkDir = 3;
			else if (x > 0 && 0 == y)
				bkDir = 2;
			else if (x > 0 && y < 0)
				bkDir = 1;
			else if (0 == x && y < 0)
				bkDir = 0;
			return bkDir;
		}
		const zPosIVector &getScreenByRange(const zPos &pos, const int range);
		/**
		 * \brief ����������ȡ��Χ9������Ļ���
		 * \param posi ������
		 * \param pv ��������������
		 */
		const zPosIVector &getNineScreen(const zPosI &posi)
		{
			NineScreen_map_const_iter iter = ninescreen.find((DWORD)posi);
			if( iter != ninescreen.end())
			{
				return iter->second;
			}
			//��������쳣����0��ŵ�������
			return ninescreen[(DWORD)-1];
		}

		/**
		 * \brief ����������ȡ��ǰ��3������5������Ļ���
		 * \param posi ������
		 * \param direct ����
		 * \return ��������������
		 */
		const zPosIVector &getDirectScreen(const zPosI &posi, const int dir)
		{
			NineScreen_map_const_iter iter = direct_screen[dir].find((DWORD)posi);
			if( iter != direct_screen[dir].end())
			{
				return iter->second;
			}
			//��������쳣����0��ŵ�������
			return direct_screen[dir][(DWORD)-1];
		}

		/**
		 * \brief ����������ȡ����3������5������Ļ���
		 * \param posi ������
		 * \param direct ����
		 * \return ��������������
		 */
		const zPosIVector &getReverseDirectScreen(const zPosI &posi, const int dir)
		{
			NineScreen_map_const_iter iter = reversedirect_screen[dir].find((DWORD)posi);
			if( iter != reversedirect_screen[dir].end())
			{
				return iter->second;
			}
			//��������쳣����0��ŵ�������
			return reversedirect_screen[dir][(DWORD)-1];
		}

};

#endif

