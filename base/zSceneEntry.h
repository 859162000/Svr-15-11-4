/**
 * \file
 * \version  $Id: zSceneEntry.h  $
 * \author  
 * \date 
 * \brief �������������
 */

#ifndef _ZSCENEENTRY_H_
#define _ZSCENEENTRY_H_
#include <vector>
#include <list>
#include <ext/pool_allocator.h>
#include "zEntry.h"
#include "Command.h"

#pragma pack(1)
/**
 * \brief ����ƫ�Ƽ��������ֵ
 */
struct zAdjust
{
	int x;		/**< ������*/
	int y;		/**< ������*/
};
/**
 * \brief ��������
 */
struct zPos
{
	DWORD x;		/**< ������*/
	DWORD y;		/**< ������*/
	/**
	 * \brief ���캯��
	 *
	 */
	zPos()
	{
		x = 0;
		y = 0;
	}
	/**
	 * \brief ���캯��
	 *
	 */
	zPos(const DWORD x, const DWORD y)
	{
		this->x = x;
		this->y = y;
	}
	/**
	 * \brief �������캯��
	 *
	 */
	zPos(const zPos &pos)
	{
		x = pos.x;
		y = pos.y;
	}
	/**
	 * \brief ��ֵ��������
	 *
	 */
	zPos & operator= (const zPos &pos)
	{
		x = pos.x;
		y = pos.y;
		return *this;
	}
	/**
	 * \brief ����+�������
	 *
	 */
	const zPos & operator+ (const zPos &pos)
	{
		x += pos.x;
		y += pos.y;
		return *this;
	}
	/**
	 * \brief ����+�������
	 * �������������
	 */
	const zPos & operator+ (const zAdjust &adjust)
	{
		x += adjust.x;
		y += adjust.y;
		return *this;
	}
	/**
	 * \brief ����+=�������
	 *
	 */
	const zPos & operator+= (const zPos &pos)
	{
		x += pos.x;
		y += pos.y;
		return *this;
	}
	/**
	 * \brief ����+=�������
	 * �������������
	 */
	const zPos & operator+= (const zAdjust &adjust)
	{
		x += adjust.x;
		y += adjust.y;
		return *this;
	}
	/**
	 * \brief ����-�������
	 *
	 */
	const zPos & operator- (const zPos &pos)
	{
		x -= pos.x;
		y -= pos.y;
		return *this;
	}
	/**
	 * \brief ����-�������
	 * �������������
	 */
	const zPos & operator- (const zAdjust &adjust)
	{
		x -= adjust.x;
		y -= adjust.y;
		return *this;
	}
	/**
	 * \brief ����-=�������
	 *
	 */
	const zPos & operator-= (const zPos &pos)
	{
		x -= pos.x;
		y -= pos.y;
		return *this;
	}
	/**
	 * \brief ����-=�������
	 * �������������
	 */
	const zPos & operator-= (const zAdjust &adjust)
	{
		x -= adjust.x;
		y -= adjust.y;
		return *this;
	}
	/**
	 * \brief ����==�߼��������
	 *
	 */
	const bool operator== (const zPos &pos) const
	{
		return (x == pos.x && y == pos.y);
	}
	/**
	 * \brief ����>�߼��������
	 *
	 */
	const bool operator> (const zPos &pos) const
	{
		return (x > pos.x && y > pos.y);
	}
	/**
	 * \brief ����>=�߼��������
	 *
	 */
	const bool operator>= (const zPos &pos) const
	{
		return (x >= pos.x && y >= pos.y);
	}
	/**
	 * \brief ����<�߼��������
	 *
	 */
	const bool operator< (const zPos &pos) const
	{
		return (x < pos.x && y < pos.y);
	}
	/**
	 * \brief ����<=�߼��������
	 *
	 */
	const bool operator<= (const zPos &pos) const
	{
		return (x <= pos.x && y <= pos.y);
	}
	/**
	 * \brief ������Ϊ���ĵ㣬��ȡ������һ������ķ���
	 * \param pos ����һ�������
	 * \return ����
	 */
	const int getDirect(const zPos &pos) const
	{
		using namespace Cmd;
		if (x == pos.x && y > pos.y)
		{
			return _DIR_UP;
		}
		else if (x < pos.x && y > pos.y)
		{
			return _DIR_UPRIGHT;
		}
		else if (x < pos.x && y == pos.y)
		{
			return _DIR_RIGHT;
		}
		else if (x < pos.x && y < pos.y)
		{
			return _DIR_RIGHTDOWN;
		}
		else if (x == pos.x && y < pos.y)
		{
			return _DIR_DOWN;
		}
		else if (x > pos.x && y < pos.y)
		{
			return _DIR_DOWNLEFT;
		}
		else if (x > pos.x && y == pos.y)
		{
			return _DIR_LEFT;
		}
		else if (x > pos.x && y > pos.y)
		{
			return _DIR_LEFTUP;
		}

		return _DIR_WRONG;
	}
};
/**
 * \brief ��������
 *
 */
const zPos zPosHalfScreen(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
#pragma pack()

/**
 * \brief ����������
 */
typedef DWORD zPosI;

/**
 * \brief �������ŵ�����
 *
 */
//#ifdef _POOL_ALLOC_
typedef std::vector<zPosI, __gnu_cxx::__pool_alloc<zPosI> > zPosIVector;
typedef std::vector<zPos, __gnu_cxx::__pool_alloc<zPos> > zPosVector;
//#else
//typedef std::vector<zPosI> zPosIVector;
//typedef std::vector<zPos> zPosVector;
//#endif

typedef std::list<WORD, __gnu_cxx::__pool_alloc<WORD> > SceneEntryStateList;

class zSceneEntryIndex;
/**
 * \brief �������������������,NPC,������������Ʒ��
 *
 * ����������
 *
 * 1.����������
 * 2.�����赲
 */
struct zSceneEntry:public zEntry
{
	friend class zSceneEntryIndex;
	public:
	/**
	 * \brief �������
	 */
	enum SceneEntryType
	{
		SceneEntry_Player,	/**< ��ҽ�ɫ*/
		SceneEntry_NPC,		/**< NPC*/
		SceneEntry_Build,	/**< ����*/
		SceneEntry_Object,	/**< ������Ʒ*/
		SceneEntry_Pet,		/**< ����*/
		SceneEntry_MAX
	};
	/**
	 * \brief ���״̬
	 *
	 */
	enum SceneEntryState
	{
		SceneEntry_Normal,		/**< ��ͨ״̬ */
		SceneEntry_Death,		/**< ����״̬ */
		SceneEntry_Hide			/**< ����״̬ */
	};
	/**
	 * \brief ����ת��
	 * \param screenWH ��ͼ��͸�
	 * \param pos Դ����
	 * \param posi Ŀ������
	 */
	static void zPos2zPosI(const zPos &screenWH, const zPos &pos, zPosI &posi)
	{
		posi=((screenWH.x+SCREEN_WIDTH-1)/SCREEN_WIDTH) * (pos.y/SCREEN_HEIGHT) + (pos.x/SCREEN_WIDTH);
	}
	protected:
		/**
	 	 * \brief ���캯��
	 	 */
		zSceneEntry(SceneEntryType type, const SceneEntryState state = SceneEntry_Normal):sceneentrytype(type),sceneentrystate(state)
		{
			bzero(byState, sizeof(byState));
			dir = Cmd::_DIR_DOWN;
			inserted=false;
		}

		/**
	 	 * \brief ����
	 	 */
		zPos pos;
		/**
	 	 * \brief ������
	 	 */
		zPosI posi;
		/**
		 * \brief ����
		 *
		 */
		BYTE dir;

		zPos lastPos1;
		zPos lastPos2;

	private:
		/**
	 	 * \brief �������
	 	 */
		const SceneEntryType sceneentrytype;
		/**
		 * \brief ���״̬
		 *
		 */
		SceneEntryState sceneentrystate;
		/**
	 	 * \brief ����Ƿ��ڳ�����
	 	 */
		bool inserted;

		/**
	 	 * \brief �����������
		 * \param screenWH �����Ŀ��
		 * \param newPos �����������
		 * \return ���곬��������߷���false,���򷵻�true
	 	 */
		bool setPos(const zPos &screenWH,const zPos &newPos)
		{
			if(screenWH.x>newPos.x && screenWH.y>newPos.y)
			{
				pos=newPos;
				zPos2zPosI(screenWH, newPos, posi);
				return true;
			}
			else
				return false;
		}

	private:
		/**
		 * \brief ���״̬����ħ������ص�
		 * ����״̬����ۿ��Ա��ֵģ�����ĳ��״̬�ͻ��˾Ϳ�����һ�ַ�ʽ������
		 * ��ϸ��״̬�μ�Command.h��
		 */
		BYTE byState[(Cmd::MAX_STATE + 7) / 8];
	protected:
		SceneEntryStateList stateList;
	public:
		/**
	 	 * \brief ������״̬
		 * \param state ���λ��
		 * \return ״̬����
	 	 */
		inline BYTE full_UState(WORD *state)
		{
			BYTE ret = stateList.size();
			SceneEntryStateList::iterator iter = stateList.begin();
			for(int i=0 ; i < ret ; i ++)
			{
				state[i] = *iter;
				iter ++;
			}
			return ret;
		}
		/**
	 	 * \brief ����������״̬
		 * \param state ���λ��
	 	 */
		inline void full_all_UState(void *state)
		{
			bcopy(byState,state,sizeof(byState));
		}

		/**
	 	 * \brief �õ��������
		 * \return �������
	 	 */
		inline const zPos &getPos() const
		{
			return pos;
		}

		/**
	 	 * \brief �õ�����ղŵ�����
		 * \return �������
	 	 */
		inline const zPos &getOldPos1() const
		{
			return lastPos1;
		}

		/**
	 	 * \brief �õ�����ղŵ�����
		 * \return �������
	 	 */
		inline const zPos &getOldPos2() const
		{
			return lastPos2;
		}

		/**
	 	 * \brief �õ����������
		 * \return ���������
	 	 */
		inline const zPosI &getPosI() const
		{ 
			return posi;
		}
		/**
	 	 * \brief ��������Ƿ��ڳ�����
		 * \return ����ڳ����з���true,���򷵻�false
	 	 */
		inline bool hasInScene() const
		{ 
			return inserted;
		}

		/**
	 	 * \brief �õ��������
		 * \return �������
	 	 */
		inline const SceneEntryType & getType() const
		{
			return sceneentrytype;
		}

		/**
		 * \brief ��ȡ���״̬
		 * \return ״̬
		 */
		inline const SceneEntryState & getState() const
		{
			return sceneentrystate;
		}

		/**
		 * \brief �������״̬
		 * \param state ��Ҫ���õ�״̬
		 */
		void setState(const SceneEntryState & state)
		{
			sceneentrystate = state;
		}

		/**
		 * \brief ��ȡ����
		 * \return ����
		 */
		inline const BYTE getDir() const
		{
			return dir % 8;
		}

		/**
		 * \brief ���÷���
		 * \param dir ����
		 */
		void setDir(const BYTE dir)
		{
			this->dir = dir % 8;
		}

		/**
		 * \brief ���ĳ��״̬�Ƿ�����
		 * \param state ������״̬
		 * \return ����״̬�Ƿ��Ѿ�����
		 */
		inline bool issetUState(const int state) const
		{
			return Cmd::isset_state(byState, state);
		}

		/**
		 * \brief ����ĳ��״̬
		 * \param state �����õ�״̬
		 * \return ����Ѿ����ø�״̬����false,���򷵻�true
		 */
		inline bool setUState(const int state)
		{
			if(!issetUState(state))
			{
				stateList.push_back(state);
				Cmd::set_state(byState, state);
				return true;
			}
			return false;
		}

		/**
		 * \brief ���ĳ��״̬
		 * \param state �������״̬
		 * \return ����Ѿ����ø�״̬����true,���򷵻�false
		 */
		inline bool clearUState(const int state)
		{
			Cmd::clear_state(byState, state);
			SceneEntryStateList::iterator iter = stateList.begin();
			for( ; iter != stateList.end() ; ++iter)
			{
				if(*iter == state)
				{
					stateList.erase(iter);
					return true;
				}
			}
			return false;
		}
};
#endif
