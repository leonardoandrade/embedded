# InfluxDB 

A dump of commands to use with influx, kinda cheatsheet

# Read operations

show databases
use house_sensors
show measurements
precision rfc3339
select *  from outdoor

# write operations
It is not possible to delete by value in influxDB!!

# backup / restore
influxd backup --portable ./influxdb_backup_27_10_2019/
influxd restore --portable --db house_sensors --newdb bak ./influxdb_backup_27_10_2019/
SELECT * INTO house_sensors..:outdoor FROM outdoor GROUP BY *

