# InfluxDB

influxdb:8086

* [Download](https://www.influxdata.com/get-influxdb/)
* [Database management using InfluxQL](https://docs.influxdata.com/influxdb/v1.7/query_language/database_management/)
* [influx retention](https://bl.ocks.org/nl-hugo/acf9ceabb9a813d067484d9723ca3f77)
* [InfluxDB plugin for Node-Red](https://www.npmjs.com/package/node-red-contrib-influxdb)

## CLI / SHELL

Run the influx client in this container:

`docker container exec -it influxdb influx`

```
precision rfc3339
SHOW DATABASES
USE poulailler
SELECT * FROM metrics GROUP BY * ORDER BY ASC LIMIT 1
SELECT * FROM temperature GROUP BY * ORDER BY DESC LIMIT 10
SELECT * FROM poulailler.annee.metrics LIMIT 10
```

### Examples

#### temperature

```
SELECT sensor_id, sensor_name, temperature FROM temperature  ORDER BY DESC LIMIT 10
name: temperature
time                 sensor_id        sensor_name temperature
----                 ---------        ----------- -----------
2020-08-16T20:32:28Z 28d79af039190174 DS18B20     21.13
2020-08-16T20:32:28Z 28d52c183a190178 DS18B20     20.94
2020-08-16T20:32:28Z 28c1ce233a19017a DS18B20     21
2020-08-16T20:32:28Z 289e7f193a1901a1 DS18B20     22.5
2020-08-16T20:32:28Z 2871d30d3a190188 DS18B20     21
2020-08-16T20:32:26Z 0                DHT22       -200
2020-08-16T20:32:18Z 28d79af039190174 DS18B20     21.06
2020-08-16T20:32:18Z 28d52c183a190178 DS18B20     20.94
2020-08-16T20:32:18Z 28c1ce233a19017a DS18B20     21
2020-08-16T20:32:18Z 289e7f193a1901a1 DS18B20     22.5
```

#### humidity

```
> SELECT sensor_id, sensor_name, humidity FROM humidity  ORDER BY DESC LIMIT 10
name: humidity
time                 sensor_id sensor_name humidity
----                 --------- ----------- --------
2020-08-16T20:33:36Z 0         DHT22       -200
2020-08-16T20:33:26Z 0         DHT22       -200
2020-08-16T20:33:16Z 0         DHT22       -200
2020-08-16T20:33:06Z 0         DHT22       -200
2020-08-16T20:32:56Z 0         DHT22       -200
2020-08-16T20:32:46Z 0         DHT22       -200
2020-08-16T20:32:36Z 0         DHT22       -200
2020-08-16T20:32:26Z 0         DHT22       -200
2020-08-16T20:32:16Z 0         DHT22       -200
2020-08-16T20:32:06Z 0         DHT22       -200
```


#### Create a Database

[How to set default retention policy and duration for InfluxDB via configuration](https://stackoverflow.com/a/41640770)

```
CREATE DATABASE <database_name>
[WITH [DURATION <duration>] [REPLICATION <n>]
      [SHARD DURATION <duration>] [NAME <retention-policy-name>]]
```


#### Retention Policies

```
CREATE RETENTION POLICY <retention_policy_name> ON <database_name> DURATION <duration> REPLICATION <n> [SHARD DURATION <duration>] [DEFAULT]
```

```
# every 10s
# 24h * 60min/h * 6/min = 8640
CREATE RETENTION POLICY "jour" ON "poulailler" DURATION 1d REPLICATION 1 DEFAULT
# every 2 mins
# 7d/w 24h/d * 12/h = 5040
CREATE RETENTION POLICY "semaine" ON "poulailler" DURATION 1w REPLICATION 1
# every 15 mins
# 365d/y * 24h/d * 4/h = 35040
CREATE RETENTION POLICY "annee" ON "poulailler" DURATION 52w REPLICATION 1
```


#### Continuous Queries

[Continuous Queries Advanced Syntax](https://docs.influxdata.com/influxdb/v1.7/query_language/continuous_queries/#advanced-syntax)

```
CREATE CONTINUOUS QUERY <cq_name> ON <database_name>
RESAMPLE EVERY <interval> FOR <interval>
BEGIN
  <cq_query>
END
```

```
cq_query
SELECT <function[s]> INTO <destination_measurement> FROM <measurement> [WHERE <stuff>] GROUP BY time(<interval>)[,<tag_key[s]>]
```

Automatically downsampling data into another retention policy

Fully qualify the destination measurement to store the downsampled data in a non-DEFAULT retention policy (RP).

Automatically downsampling a database with backreferencing

Use a function with a wildcard (*) and INTO query’s backreferencing syntax to automatically downsample data from all measurements and numerical fields in a database.

```
CREATE CONTINUOUS QUERY "cq_basic_br" ON "transportation"
BEGIN
  SELECT mean(*) INTO "downsampled_transportation"."autogen".:MEASUREMENT FROM /.*/ GROUP BY time(30m),*
END
```

[How to make Continuous Query inherit tags from the source time series? #126](https://github.com/influxdata/influxdb.com/issues/126)



#### Create the Database

```
#CREATE DATABASE poulailler
CREATE DATABASE poulailler
  WITH DURATION 1d
  REPLICATION 1
  NAME jour
```

```
#CREATE RETENTION POLICY jour ON "poulailler" DURATION 1d REPLICATION 1 DEFAULT
CREATE RETENTION POLICY semaine ON "poulailler" DURATION 1w REPLICATION 1
CREATE RETENTION POLICY annee ON "poulailler" DURATION 52w REPLICATION 1
```

```
CREATE CONTINUOUS QUERY cq_2m ON poulailler BEGIN
  SELECT mean(temperature) AS temperature, mean(humidite) AS humidite
  INTO poulailler.semaine.metrics
  FROM metrics
  GROUP BY time(2m),*
END

CREATE CONTINUOUS QUERY cq_15m ON poulailler BEGIN
  SELECT mean(temperature) AS temperature, mean(humidite) AS humidite
  INTO poulailler.annee.metrics
  FROM metrics
  GROUP BY time(15m),*
END
```
