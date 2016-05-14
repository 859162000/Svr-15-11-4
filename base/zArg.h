/**
 * \file
 * \version  $Id: zArg.h  $
 * \author  
 * \date 
 * \brief ����������Ķ��塣
 *
 * 
 */

#ifndef _ZARG_H_
#define _ZARG_H_

#include <argp.h>
#include "zProperties.h"
#include "zNoncopyable.h"

/**
 * \brief ����������������
 */
typedef error_t(* argsParser)(int key, char *arg, struct argp_state *state) ;

/**
 * \brief ������,һ������ֻ��һ��
 *
 * �������в���������zArg::args�У�����ʹ�òμ�#zProperties.
 *
 * �����ü̳�ʵ��ĳһ��ĳ����Ĭ�ϲ���.
 *
 * ���ڲ���������arg_options�ͷ����������μ�info argp_parse
 */
class zArg:private zNoncopyable
{
	friend error_t zparse_opt(int, char *, struct argp_state *);
	protected:
		/**
		 * \brief �����������ݽṹ
		 */
		struct argp argp;
		/**
		 * \brief �û�����Ĳ�����������
		 */
		argsParser user_parser;
		/**
		 * \brief ������������Ψһʵ��ָ��
		 */
		zArg();
		~zArg();
	private:
		static zArg * argInstance;
		void addOptions(const struct argp_option *options);
		/**
		 * \brief ���в���ѡ��ָ��
		 */
		struct argp_option *alloptions;

	public:
		static zArg *getArg();
		static void removeArg();

		bool add(const struct argp_option *options=0,argsParser func=0,const char *args_doc=0,const char *doc=0);
		bool parse(int argc ,char *argv[]);
};
#endif
