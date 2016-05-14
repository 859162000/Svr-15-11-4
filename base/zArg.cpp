/**
 * \file
 * \version  $Id: zArg.cpp  $
 * \author  
 * \date 
 * \brief ����������Ķ��塣
 *
 * 
 */

#include "zArg.h"
#include "Zebra.h"
#include <string.h>

/**
 * \brief ����BUG�����ַ
 */
const char *argp_program_bug_address = "<okyhc@263.sina.com, songsiliang@netease.com>";

/**
 * \brief Ĭ�ϲ�������
 */
static const char zebra_args_doc[] = "";
/**
 * \brief Ĭ�ϲ�������
 */
static const char zebra_doc[] = "this is default argument document.";
/**
 * \brief Ĭ�Ͽ���ѡ��
 */
static struct argp_option zebra_options[] =
{
	{0,	0,	0,	0,	0, 0}
};
/**
 * \brief Ĭ�ϵķ�������,�μ�info argp_parse;
 */
error_t zparse_opt(int key, char *arg, struct argp_state *state)
{
	switch (key)
	{
		default:
			if(zArg::getArg()->user_parser!=0)
				return zArg::getArg()->user_parser(key,arg,state);
			else
				return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

zArg *zArg::argInstance(0);

/**
 * \brief ���캯������ʼ��Ĭ�ϲ�����������
 */
zArg::zArg()
{
		user_parser=0;
		alloptions=0;

		argp.children=0;
		argp.help_filter=0;
		argp.argp_domain=0;

		argp.parser=zparse_opt;
		argp.args_doc=zebra_args_doc;
		argp.doc=zebra_doc;
		addOptions(zebra_options);
}

/**
 * \brief ��������
 */
zArg::~zArg()
{
	SAFE_DELETE_VEC(alloptions);
}

/**
 * \brief �õ�����������
 * \return ����������ָ��
 */
zArg *zArg::getArg()
{
	if(argInstance==0)
		argInstance=new zArg();
	return argInstance;
}

/**
 * \brief ɾ������������
 */
void zArg::removeArg()
{
	SAFE_DELETE(argInstance);
}

/**
 * \brief ��Ӳ���ѡ��
 * \param options Ҫ��ӵĲ���ѡ������
 */
void zArg::addOptions(const struct argp_option *options)
{
	if(options==0) return;

	int ucount=0;
	while(options[ucount].name!=0)
		ucount++;

	if(alloptions==0)
	{
		alloptions=new struct argp_option[ucount+1];
		memcpy(alloptions,options,sizeof(argp_option)*(ucount+1));

	}
	else
	{
		int ocount=0;
		while(alloptions[ocount].name!=0)
			ocount++;

		//std::cout << "ucount:" << ucount << std::endl;
		//std::cout << "ocount:" << ocount << std::endl;
		//std::cout << "allcount" << ucount+ocount+1 << std::endl;
		struct argp_option *otemp=alloptions;
		alloptions=new argp_option[ucount+ocount+1];
		if (ocount > 0) memcpy(alloptions,otemp,sizeof(argp_option)*ocount);
		memcpy(alloptions+ocount,options,sizeof(argp_option)*(ucount+1));
		SAFE_DELETE_VEC(otemp);
	}
	argp.options=alloptions;
}

/**
 * \brief ����Լ��Ĳ���ѡ��ͷ��������������ĵ������ʡ����Ĭ��ֵ
 * \param options �Լ��Ĳ���ѡ��
 * \param func �Լ�����ѡ��ķ�������
 * \param args_doc ����ѡ����ϸ�ĵ�
 * \param doc ��Ҫ����ѡ���ĵ�
 * \return ʼ�շ���true
 */
bool zArg::add(const struct argp_option *options,argsParser func,const char *args_doc,const char *doc)
{
	if(func!=0)
		user_parser=func;
	if(options!=0)
		addOptions(options);
	if(args_doc!=0)
		argp.args_doc=args_doc;
	if(doc!=0)
		argp.doc=doc;
	return true;
}

/**
 * \brief ��������
 * \param argc ��������
 * \param argv �����б�
 * \return ʼ�շ���true
 */
bool zArg::parse(int argc ,char *argv[])
{
	argp_parse(&argp, argc, argv, 0, 0, 0);
	return true;
}
