#server ip
GM_IP=10.174.148.151
#充值ip
FL_IP=10.174.148.151
#db ip
DB_IP=10.174.148.151

DB_USER=ztgame
DB_PASSWD=123456
DB_NAME=Zebra
mysql=/usr/local/mysql/bin/mysql

create_db_sql="
DROP DATABASE IF EXISTS ${DB_NAME};
CREATE DATABASE ${DB_NAME}
"

echo $create_db_sql
$mysql -u $DB_USER -h ${DB_IP} -p${DB_PASSWD} -e "${create_db_sql}"

$mysql -u $DB_USER -h ${DB_IP} -p${DB_PASSWD} $DB_NAME < LoginServer.sql
$mysql -u $DB_USER -h ${DB_IP} -p${DB_PASSWD} $DB_NAME < BillServer.sql
$mysql -u $DB_USER -h ${DB_IP} -p${DB_PASSWD} $DB_NAME < RecordServer.sql
$mysql -u $DB_USER -h ${DB_IP} -p${DB_PASSWD} $DB_NAME < SuperServer.sql
$mysql -u $DB_USER -h ${DB_IP} -p${DB_PASSWD} $DB_NAME < SessionServer.sql

#内网登陆用
#loginsql="INSERT INTO \`ZONELIST\` (\`GAME\`,\`ZONE\`,\`NAME\`,\`PORT\`) VALUES(1,1,"游戏",${GM_IP},10000);"

gamesql="
INSERT INTO \`SERVERLIST\` (\`TYPE\`,\`ID\`,\`NAME\`,\`IP\`,\`PORT\`,\`EXTIP\`,\`EXTPORT\`) VALUES(1,1,'mg','$GM_IP',10000,'$GM_IP',10000);
INSERT INTO \`SERVERLIST\` (\`TYPE\`,\`ID\`,\`NAME\`,\`IP\`,\`PORT\`,\`EXTIP\`,\`EXTPORT\`) VALUES(11,2,'rec','$GM_IP',5002,'$GM_IP',5002);
INSERT INTO \`SERVERLIST\` (\`TYPE\`,\`ID\`,\`NAME\`,\`IP\`,\`PORT\`,\`EXTIP\`,\`EXTPORT\`) VALUES(12,3,'bill','$GM_IP',5003,'$GM_IP',5003);
INSERT INTO \`SERVERLIST\` (\`TYPE\`,\`ID\`,\`NAME\`,\`IP\`,\`PORT\`,\`EXTIP\`,\`EXTPORT\`) VALUES(20,4,'sess','$GM_IP',5004,'$GM_IP',5004);
INSERT INTO \`SERVERLIST\` (\`TYPE\`,\`ID\`,\`NAME\`,\`IP\`,\`PORT\`,\`EXTIP\`,\`EXTPORT\`) VALUES(21,5,'scene','$GM_IP',5005,'$GM_IP',5005);
INSERT INTO \`SERVERLIST\` (\`TYPE\`,\`ID\`,\`NAME\`,\`IP\`,\`PORT\`,\`EXTIP\`,\`EXTPORT\`) VALUES(22,6,'gate','$GM_IP',5007,'$GM_IP',5007);
"

echo "配置服务列表"

echo $loginsql
#$mysql -u $DB_USER -h ${DB_IP} -p${DB_PASSWD} $DB_NAME -e "$loginsql"
echo $gamesql
$mysql -u $DB_USER -h ${DB_IP} -p${DB_PASSWD} $DB_NAME -e "$gamesql"

echo "配置成功"


