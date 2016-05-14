/**
 * \file
 * \version  $Id: zString.h $
 * \author  
 * \date 
 * \brief ��װһЩ���õ��ַ�������
 *
 * 
 */


#ifndef _zString_h_
#define _zString_h_

#include <string>
#include <algorithm>
#include <cctype>


namespace Zebra
{

	/**
	 * \brief ���ַ�������tokenת��Ϊ����ַ���
	 *
	 * ������ʹ�����ӳ���
	 *    <pre>
	 *    std::list<string> ls;
	 *    stringtok (ls, " this  \t is\t\n  a test  ");
	 *    for(std::list<string>const_iterator i = ls.begin(); i != ls.end(); ++i)
	 *        std::cerr << ':' << (*i) << ":\n";
	 *     </pre>
	 *
	 * \param container ���������ڴ���ַ���
	 * \param in �����ַ���
	 * \param delimiters �ָ�����
	 * \param deep ��ȣ��ָ����ȣ�ȱʡû������
	 */
	template <typename Container>
		inline void
		stringtok(Container &container, std::string const &in,
				const char * const delimiters = " \t\n",
				const int deep = 0)
		{
			const std::string::size_type len = in.length();
			std::string::size_type i = 0;
			int count = 0;

			while(i < len)
			{
				i = in.find_first_not_of (delimiters, i);
				if (i == std::string::npos)
					return;   // nothing left

				// find the end of the token
				std::string::size_type j = in.find_first_of (delimiters, i);

				count++;
				// push token
				if (j == std::string::npos
						|| (deep > 0 && count > deep)) {
					container.push_back (in.substr(i));
					return;
				}
				else
					container.push_back (in.substr(i, j-i));

				// set up for next loop
				i = j + 1;

			}
		}

	/**
	 * \brief ���ַ�ת��ΪСд�ĺ�������
	 *
	 * ���磺
	 * <pre>
	 * std::string  s ("Some Kind Of Initial Input Goes Here");
	 * std::transform (s.begin(), s.end(), s.begin(), ToLower());
	 * </pre>
	 */
	struct ToLower
	{
		char operator() (char c) const
		{
			return std::tolower(c);
		}
	};

	/**
	 * \brief ���ַ���ת��ΪСд
	 * 
	 * ��������ַ���ת��ΪСд
	 *
	 * \param s ��Ҫת�����ַ���
	 */
	inline void to_lower(std::string &s)
	{
		std::transform(s.begin(), s.end(), s.begin(), ToLower());
	}

	/**
	 * \brief ���ַ�ת��Ϊ��д�ĺ�������
	 *
	 * ���磺
	 * <pre>
	 * std::string  s ("Some Kind Of Initial Input Goes Here");
	 * std::transform (s.begin(), s.end(), s.begin(), ToUpper());
	 * </pre>
	 */
	struct ToUpper
	{
		char operator() (char c) const
		{
			return std::toupper(c);
		}
	};

	/**
	 * \brief ���ַ���ת��Ϊ��д
	 * 
	 * ��������ַ���ת��Ϊ��д
	 *
	 * \param s ��Ҫת�����ַ���
	 */
	inline void to_upper(std::string &s)
	{
		std::transform(s.begin(), s.end(), s.begin(), ToUpper());
	}

}

#endif

