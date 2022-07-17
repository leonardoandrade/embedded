import sys
from influxdb import InfluxDBClient
client = InfluxDBClient(host='localhost', port=8086)


def fetch_values(measurement, sensor, value):
    print(f'getting records from  measurement={measurement}, sensor={sensor}, value={value}')
    #TODO: find how to filter by sensor
    results = client.query(f"select * from {measurement} where value={value}", epoch="ns")
    print(len(results))
    points = results.get_points(tags={"sensor":sensor})

    return [p["time"] for p in points]


def delete_by_timestamps(measurement, timestamps):
    print(f"deleting {len(timestamps)} records")
    for t in timestamps:
        print(t)
        client.query(f"delete from {measurement} where time = {t}", method="POST")


if __name__ == '__main__':
    if len(sys.argv) != 5:
        print(""""
        USAGE: 
         delete_by_value <database> <measurement> <sensor> <value> 
        """)
        sys.exit(0)

    print(f"using db {sys.argv[1]}")
    client.switch_database(sys.argv[1])
    values = fetch_values(*sys.argv[2:])
    delete_by_timestamps(sys.argv[2], values)