#ifndef _WORD_FILTER_H
#define _WORD_FILTER_H

#include <fstream>
#include "zRegex.h"

/**
 * \brief ���Թ�����
 *
 */
class wordFilter
{
	private:
		static wordFilter *instance;
		std::map<std::string, std::string> forbidWords;
		wordFilter()
		{
			init();
		}
		void init();
	public:
		static wordFilter &getInstance()
		{
			if (0==instance)
				instance = new wordFilter();
			return *instance;
		}
		static void delInstance()
		{
			SAFE_DELETE(instance);
		}
		bool doFilter(char *text, unsigned int cmdLen);
};

wordFilter *wordFilter::instance = 0;
/**
 * \brief ��ȡ���Թ��˱��
 *
 *
 */
void wordFilter::init()
{
	//��ֹ�ʻ�
	std::string f = Zebra::global["forbidWordsFile"];
	if (""==f)
		f = "forbidWords";
	std::ifstream src(f.c_str());
	if (!src)
	{
		Zebra::logger->warn("�򿪴ʻ�����б�ʧ�ܣ�file=%s", Zebra::global["forbidWordsFile"].c_str());
		return;
	}

	char buf[256];
	bzero(buf,sizeof(buf));
	std::vector<std::string> vs;
	std::vector<std::string>::iterator word,replacer;
	while (src.getline(buf, sizeof(buf)))
	{
		vs.clear();
		Zebra::stringtok(vs, buf, " \t\r");
		if (vs.size()==3)
		{
			replacer = vs.begin();
			replacer++;
			word = replacer++;

			//ת����Сд
			bzero(buf,sizeof(buf));
			word->copy(buf, word->length(), 0);
			for (unsigned int i=0; i<word->length(); i++)
				buf[i] = tolower(buf[i]);
			word->assign(buf);

			forbidWords[*word] = *replacer;
			Zebra::logger->debug("%u\t%s\t%s", forbidWords.size(), (*word).c_str(), (*replacer).c_str());
		}
	}

	Zebra::logger->debug("���شʻ�����б� %d", forbidWords.size());
}

/**
 * \brief ���Թ���
 *
 *
 * \param text: ��������
 * \param len: ���ݳ���
 * \return 
 */
bool wordFilter::doFilter(char *text, unsigned int len)
{
	//���дʻ����
	zRegex regex;
	bool ret = true;
	
	char copy[MAX_CHATINFO];
	strncpy(copy, text, sizeof(copy));
	for (unsigned int i=0; i<strlen(copy); i++)
		copy[i] = tolower(copy[i]);
	
	std::string content(text);
	std::string content_copy(copy);
	std::string::size_type pos=0;
	
	for (std::map<std::string, std::string>::iterator it=forbidWords.begin(); it!=forbidWords.end(); ++it)
	{
		pos = content_copy.find(it->first.c_str(), 0);
		while (pos!=std::string::npos)
		{
			BYTE val = 0;
			if (pos)
				val = content_copy.c_str()[pos-1];//�Ƿ��ֵĺ���һ��

			if (val>=0x80)
				pos = content_copy.find(it->first.c_str(), pos+1);
			else
			{
				content.replace(pos, it->first.length(), it->second.c_str());
				content_copy.replace(pos, it->first.length(), it->second.c_str());

				pos = content_copy.find(it->first.c_str(), pos+it->first.length());
				ret = false;
			}
		}
	}
	strncpy(text, content.c_str(), len);

	return ret;
}

#endif
