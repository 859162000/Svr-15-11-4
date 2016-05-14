/**
 * \file
 * \version  $Id: Leechdom.h $
 * \author  
 * \date 
 * \brief ҩƷ,ʳ������Ʒʹ��
 *
 * �ָ�����ֵ,����ֵ,����ֵ����Ʒ
 * 
 */

#ifndef _LEECHDOM_H_
#define _LEECHDOM_H_
#include <vector>
#include "zType.h"
#include "zMutex.h"

#ifndef _LeechdomType_
#define _LeechdomType_
enum LeechdomType
{
	Leechdom_default,
	Leechdom_dam,//�����˺���
	Leechdom_def,//���ӷ�����
	Leechdom_poison,//ҩ��ʹ�˳����ж�
	Leechdom_sppersist,//��Ʒʹ������ֵ���ֵ�ǰֵ����
	Leechdom_spup,//ҩ������������ֵ�ٷֱ�
	Leechdom_spcostdown,//�������������ٶ�
	Leechdom_spresumeup,//�ӿ������ָ��ٶ�
	Leechdom_hp,//һ���Իָ�����
	Leechdom_hppersist,//�����ָ�����
	Leechdom_mp,//һ���Իָ�����ֵ
	Leechdom_mppersist,//�����ָ�����
	Leechdom_sp,//һ���Իָ�����ֵ
	Leechdom_hp5,//һ���Իָ�����
	Leechdom_hppersist5,//�����ָ�����
	Leechdom_hpmax,//�����;�һ�μ���HP
	Leechdom_mpmax,//�����;�һ�μ���MP
	Leechdom_chocolate,//�ɿ���
};
#endif

struct SceneUser;
struct LeechdomElement
{
	LeechdomElement()
	{
		type = Leechdom_default;
		value = 0;
		times = 0;
	}
	LeechdomType type;
	WORD value;
	WORD times;
};
/**
 * \brief ҩƷʹ��
 *
 */
struct Leechdom
{
	Leechdom()
	{
		damcooling = 0;
		defcooling = 0;
		hpcooling = 0;
		spcooling = 0;
		mpcooling = 0;
		sppersist = 0;
	}
	void add(LeechdomType type , WORD value ,WORD times);
	void clear();
	DWORD fresh(SceneUser *pUser , DWORD &update);
	bool isCooling(DWORD type);
	void checkCooling();
	private:
	typedef std::vector<LeechdomElement> Leechdom_vector;
	typedef Leechdom_vector::iterator Leechdom_iterator;
	Leechdom_vector element;
	zMutex mlock;
	/// �˺�����Ʒ��ȴʱ��
	WORD damcooling;
	/// ��������Ʒ��ȴʱ��
	WORD defcooling;
	/// �ָ�hp����Ʒ��ȴʱ��
	WORD hpcooling;
	/// �ָ�sp����Ʒ��ȴʱ��
	WORD spcooling;
	/// �ָ�mp����Ʒ��ȴʱ��
	WORD mpcooling;
	WORD sppersist;
};

#endif
