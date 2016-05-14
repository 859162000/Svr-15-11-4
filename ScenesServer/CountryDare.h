/**
 * \file
 * \version  $Id: CountryDare.h  $
 * \author  
 * \date 
 * \brief ��ս������
 *
 * 
 */

#ifndef _COUNTRY_DARE
#define _COUNTRY_DARE
#include "zTime.h"
#include <time.h>       
#include <sys/time.h> 
#include <vector>
#include "zRWLock.h"

class Scene;

class CountryDare
{
	public:
		CountryDare()
		{
			last_attack_time=0;
			id=0;last_gen_attack_time=0;
			last_king_attack_time=0;
		};
		
		~CountryDare(){};
		
		DWORD id;
		time_t last_attack_time;
		time_t last_gen_attack_time;
		time_t last_att_gen_attack_time;
		time_t last_king_attack_time;
};

class CountryDareM
{
	public:
		static CountryDareM& getMe()
		{
			if (!_instance)
			{
				_instance = new CountryDareM();
			}

			return *_instance;
		}
		
		~CountryDareM(){}

		/**
		  * \brief ʱ��ص�����
		  */
		void timer();


		/**
		 * \brief �ж��Ƿ���Թ�������
		 *
		 */
		bool isAttackMainFlag(Scene* scene, DWORD dwNpcID);

		/**
		 * \brief �ж��Ƿ���Թ����󽫾�
		 *
		 */
		bool isAttackMainGen(Scene* scene);

		
		/**
		  * \brief �����
		  *
		  */
		void reliveSecondFlag(Scene* scene, DWORD dwNpcID);

		/**
		  * \brief �ӳ����츴��ʱ��
		  *
		  */
		void delaySecondFlag(Scene* scene);

		/**
		 * \brief ����ɧ�Ź�ս������ʱ��
		 *
		 */
		void updateAttackTime(DWORD countryid, time_t uptime);
		void updateKingAttackTime(DWORD countryid, time_t uptime);
		void setCountryDare(DWORD dwCountryID, DWORD dwDareCountryID, bool isAtt);

		/**
		 * \brief 
		 *
		 */
		void updateGenAttackTime(DWORD countryid, time_t uptime);

		void updateAttGenAttackTime(DWORD countryid, time_t uptime);

		/**
		 * \brief ��ʼ��
		 *
		 * ��ʼ���ó������й��ҵĹ�ս��Ϣ
		 *
		 */
		void init();
		
	protected:
		std::vector<CountryDare> dares;
		zRWLock rwlock;	
		CountryDareM(){}
		static CountryDareM* _instance;
};

#endif

