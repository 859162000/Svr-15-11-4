/**
 * \file
 * \version  $Id: zNoncopyable.h  $
 * \author  
 * \date 
 * \brief ����zNoncopyable�࣬ʹ���е�������ÿ������캯���͸�ֵ����
 *
 * 
 */


#ifndef _zNoncopyable_h_
#define _zNoncopyable_h_

/**
 * \brief ʹ���е�������ÿ������캯���͸�ֵ����
 *
 */
class zNoncopyable
{

	protected:

		/**
		 * \brief ȱʡ���캯��
		 *
		 */
		zNoncopyable() {};

		/**
		 * \brief ȱʡ��������
		 *
		 */
		~zNoncopyable() {};

	private:

		/**
		 * \brief �������캯����û��ʵ�֣����õ���
		 *
		 */
		zNoncopyable(const zNoncopyable&);

		/**
		 * \brief ��ֵ�������ţ�û��ʵ�֣����õ���
		 *
		 */
		const zNoncopyable & operator= (const zNoncopyable &);

};

#endif

