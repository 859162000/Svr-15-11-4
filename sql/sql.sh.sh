echo "create database Zebra"
mysql -uroot -e "create database Zebra;"
mysql -uroot -e "grant all on Zebra.* to Zebra@`%` identified by 'Zebra';"
#mysql -uroot Zebra < BillServer.sql

echo "create database SuperServer"
mysql -uroot -e "create database SuperServer;"
mysql -uroot -e "grant all on SuperServer.* to Zebra@`%` identified by 'Zebra' ;"
#mysql -uroot -e SuperServer < SuperServer.sql

echo  "create database LoginServer"
mysql -uroot -e "create database LoginServer;"
mysql -uroot -e "grant all on LoginServer.* to Zebra@`%` identified by 'Zebra' ;"

echo "create database RecordServer"
mysql -uroot -e "create database RecordServer;"
mysql -uroot -e "grant all on RecordServer.* to Zebra@`%` identified by 'Zebra' ;"

echo "create database AccountServer"
mysql -uroot -e "create database AccountServer;"
mysql -uroot -e "grant all on AccountServer.* to Zebra@`%` identified by 'Zebra' ;"


