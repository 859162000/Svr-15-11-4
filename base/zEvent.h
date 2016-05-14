/**
 * \file	zEvent.h
 * \version  	$Id$
 * \author  	
 * \date 	
 * \brief 	����FSM���¼��ӿ�
 *
 * 
 */

#ifndef _EVENT_H
#define _EVENT_H

#include <string>

/**
 * \brief ����״̬�����¼��ӿ�
 *
 */
class Event 
{
	public:

		explicit Event (const std::string ename)
		{
			size = 0;
			data = NULL;
			name = ename;
		}

		/**
		 * \brief �������캯��
		 *
		 */
		Event(Event& ref)
		{
			bcopy(ref.data, data, ref.size);
			this->size = ref.size;
			name = ref.name;
		}

		virtual ~Event()
		{
			if (data) 
			{
				delete[] data;
				data = NULL;
			}
		}

	/**
	 * �����¼�������
	 */

	/**
	 * \brief �����¼�������
	 *
	 */
	virtual std::string get_name()
	{
		return this->name;
	}

	/**
	 * \brief ���ø��¼���ص����� 
	 *
	 *
	 * \param ptNullCmd ����ָ��
	 * \return 
	 */
	virtual void set_data(const unsigned char *buf, const unsigned int buf_size)
	{
		this->size = buf_size;
		this->data = new unsigned char [this->size];
		bcopy(buf, this->data, this->size);
	}

	/**
	 * \brief  ��ȡ���¼���ص�����
	 *
	 *
	 * \param buf ���ݱ����ڴ�
	 * \param size ���ݴ�С
	 *
	 * \return �з���TRUE,�޷���FALSE
	 */
	virtual void get_data(unsigned char * buf, int * size)
	{
		bcopy(this->data, buf, this->size);
		*size = this->size;
	}
	
	/**
	 * \brief  ��ȡ���¼���ص�����
	 *
	 * \return ��������
	 */
	virtual unsigned char* get_data()
	{
		return data;
	}
	
	/**
	 * \brief  ��ȡ���¼���ص����ݴ�С
	 *
	 * \return �������ݴ�С
	 */
	virtual unsigned int get_size()
	{
		return size;
	}

	/**
	 * \brief ���¼��Ƿ���Ч
	 *
	 * \return ��Ч,����TRUE, ����,����FALSE
	 */
	bool is_valid()
	{
		return valid;
	}

	/**
	 * \brief ���ظ�ֵ�����
	 *
	 *
	 * \param 
	 * \return 
	 */
	Event& operator= (Event& ref)
	{
		bcopy(ref.data, data, ref.size);
		this->size = ref.size;
		this->name = ref.name;
		return *this;
	}
	
	protected:
		unsigned char* data;
		unsigned int   size;
		std::string    name;

	private:
		bool valid;
};

#endif
