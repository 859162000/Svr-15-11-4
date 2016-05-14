#ifndef _WAYPOINT_H_
#define _WAYPOINT_H_
#include "zSceneEntry.h"
#include "zXMLParser.h"

/**
 * \brief ��ṹ
 */
struct Point
{
	/// ����
	char name[MAX_NAMESIZE];

	/// λ��
	zPos pos;

	/**
	 * \brief ���캯����ʼ��
	 */
	Point() : pos()
	{
		bzero(name, sizeof(name));
	}

	/**
	 * \brief ��������
	 * \param point ��
	 */
	Point(const Point &point)
	{
		bcopy(point.name, name, sizeof(name));
		pos = point.pos;
	}

	/**
	 * \brief ���ز�������������
	 * \param point �������ĵ�
	 */
	Point & operator= (const Point &point)
	{
		bcopy(point.name, name, sizeof(name));
		pos = point.pos;
		return *this;
	}
};

class WayPointM;

/**
 * \brief ���㶨��
 */
struct WayPoint
{
	friend class WayPointM;
	private:

		/// Ŀ������㼯��
		std::vector<Point> dest;

		/// Ŀ���������Ŀ
		WORD destC;
	public:
		/// ��������㼯��
		std::vector<zPos> point;
		/// �����������Ŀ
		WORD pointC;

		bool init(zXMLParser *parser,const xmlNodePtr node , DWORD countryid);
		WayPoint();
		const Point *getRandDest() const;
		const zPos *getRandPoint() const;
};

class WayPointM
{
	private:
		/// ���㼯��
		std::vector<WayPoint> wps;
	public:
		bool addWayPoint(const WayPoint &wp);
		const WayPoint *getWayPoint(const zPos &pos) const;
		const WayPoint *getWayPoint(const char *filename) const;
		const WayPoint *getRandWayPoint() const;
};

#endif
