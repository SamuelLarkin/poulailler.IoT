# Node-Red

http://192.168.X.Y:1880
http://192.168.X.Y:1880/ui

* [Get Started](https://nodered.org/#get-started)

```
sudo bash <(curl -sL https://raw.githubusercontent.com/node-red/linux-installers/master/deb/update-nodejs-and-nodered)
sudo systemctl enable nodered.service
```

If msg.payload is an object containing multiple properties, the fields will be written to the measurement.

For example, the following flow injects three fields, numValue, randomValue and strValue into the same measurement with the current timestamp.

```
[ 
   { 
      "id":"eba91e98.1456e",
      "type":"influxdb",
      "z":"b061b303.4f9e5",
      "hostname":"127.0.0.1",
      "port":"8086",
      "database":"aTimeSeries",
      "name":"aTimeSeries"
   },
   { 
      "id":"baee675c.451198",
      "type":"inject",
      "z":"b061b303.4f9e5",
      "name":"",
      "topic":"",
      "payload":"",
      "payloadType":"date",
      "repeat":"",
      "crontab":"",
      "once":false,
      "x":103,
      "y":177,
      "wires":[ 
         [ 
            "827180cf.7d8e8"
         ]
      ]
   },
   { 
      "id":"827180cf.7d8e8",
      "type":"function",
      "z":"b061b303.4f9e5",
      "name":"Fields",
      "func":"msg.payload = {\n    numValue: 123.0,\n    strValue: \"message\",\n    randomValue: Math.random()*10\n}\nreturn msg;",
      "outputs":1,
      "noerr":0,
      "x":251,
      "y":177,
      "wires":[ 
         [ 
            "c36cb4d6.3c9348"
         ]
      ]
   },
   { 
      "id":"c36cb4d6.3c9348",
      "type":"influxdb out",
      "z":"b061b303.4f9e5",
      "influxdb":"eba91e98.1456e",
      "name":"",
      "measurement":"test",
      "x":421,
      "y":177,
      "wires":[ 

      ]
   }
]
```

The function node in the flow above consists of the following:

```
msg.payload = {
  numValue: 123.0,
  strValue: "message",
  randomValue: Math.random()*10
}
return msg;
```

If msg.payload is an array containing two objects, the first object will be written as the set of named fields, the second is the set of named tags.

For example, the following simple flow injects three fields as above, along with two tags, tag1 and tag2:

```
[ 
   { 
      "id":"eba91e98.1456e",
      "type":"influxdb",
      "z":"b061b303.4f9e5",
      "hostname":"127.0.0.1",
      "port":"8086",
      "database":"aTimeSeries",
      "name":"aTimeSeries"
   },
   { 
      "id":"7f25337e.80dacc",
      "type":"inject",
      "z":"b061b303.4f9e5",
      "name":"",
      "topic":"",
      "payload":"",
      "payloadType":"date",
      "repeat":"",
      "crontab":"",
      "once":false,
      "x":101,
      "y":248,
      "wires":[ 
         [ 
            "bb0ff0.ff44f01"
         ]
      ]
   },
   { 
      "id":"bb0ff0.ff44f01",
      "type":"function",
      "z":"b061b303.4f9e5",
      "name":"Fields and Tags",
      "func":"msg.payload = [{\n    numValue: 12,\n    randomValue: Math.random()*10,\n    strValue: \"message2\"\n},\n{\n    tag1:\"sensor1\",\n    tag2:\"device2\"\n}];\nreturn msg;",
      "outputs":1,
      "noerr":0,
      "x":272,
      "y":248,
      "wires":[ 
         [ 
            "8e2713fa.71d8f"
         ]
      ]
   },
   { 
      "id":"8e2713fa.71d8f",
      "type":"influxdb out",
      "z":"b061b303.4f9e5",
      "influxdb":"eba91e98.1456e",
      "name":"",
      "measurement":"test",
      "x":460,
      "y":248,
      "wires":[ 

      ]
   }
]
```

The function node consists of the following code:

```
msg.payload = [{
  numValue: 12,
  randomValue: Math.random()*10,
  strValue: "message2"
},
{
  tag1:"sensor1",
  tag2:"device2"
}];
return msg;
```

Finally, if msg.payload is an array of arrays, it will be written as a series of points containing fields and tags.

For example, the following flow injects two points with timestamps specified.

```
[ 
   { 
      "id":"eba91e98.1456e",
      "type":"influxdb",
      "z":"b061b303.4f9e5",
      "hostname":"127.0.0.1",
      "port":"8086",
      "database":"aTimeSeries",
      "name":"aTimeSeries"
   },
   { 
      "id":"9555a67c.6aaa58",
      "type":"function",
      "z":"b061b303.4f9e5",
      "name":"multiple readings",
      "func":"msg.payload = [\n    [{\n        numValue: 10,\n        randomValue: Math.random()*10,\n        strValue: \"message1\",\n        time: new Date(\"2015-12-28T19:41:13Z\").getTime()\n    },\n    {\n        tag1:\"sensor1\",\n        tag2:\"device2\"\n    }],\n    [{\n        numValue: 20,\n        randomValue: Math.random()*10,\n        strValue: \"message2\",\n        time: new Date(\"2015-12-28T19:41:14Z\").getTime()\n    },\n    {\n        tag1:\"sensor1\",\n        tag2:\"device2\"\n    }]\n];\nreturn msg;",
      "outputs":1,
      "noerr":0,
      "x":278,
      "y":335,
      "wires":[ 
         [ 
            "f485378d.0b7ac8"
         ]
      ]
   },
   { 
      "id":"68b911d9.9746f",
      "type":"inject",
      "z":"b061b303.4f9e5",
      "name":"",
      "topic":"",
      "payload":"",
      "payloadType":"date",
      "repeat":"",
      "crontab":"",
      "once":false,
      "x":104,
      "y":335,
      "wires":[ 
         [ 
            "9555a67c.6aaa58"
         ]
      ]
   },
   { 
      "id":"f485378d.0b7ac8",
      "type":"influxdb out",
      "z":"b061b303.4f9e5",
      "influxdb":"eba91e98.1456e",
      "name":"",
      "measurement":"test",
      "x":479,
      "y":334,
      "wires":[ 

      ]
   }
]
```

The function node in the above flow looks as follows:

```
msg.payload = [
[{
    numValue: 10,
    randomValue: Math.random()*10,
    strValue: "message1",
    time: new Date("2015-12-28T19:41:13Z").getTime()
},
{
    tag1:"sensor1",
    tag2:"device2"
}],
[{
    numValue: 20,
    randomValue: Math.random()*10,
    strValue: "message2",
    time: new Date("2015-12-28T19:41:14Z").getTime()
},
{
    tag1:"sensor1",
    tag2:"device2"
}]
];
return msg;
```

Note how timestamps are specified - the number of milliseconds since 1 January 1970 00:00:00 UTC.

[Post data to InfluxDB](https://flows.nodered.org/flow/1d06cc6dbb57544f2d8b)

`network,device={{topic}} value={{payload}}`

```
  {
    "id": "f9c2968b.063d68",
    "type": "template",
    "z": "106a5b82.ef95a4",
    "name": "Fmt Influx Output",
    "field": "payload",
    "fieldType": "msg",
    "format": "handlebars",
    "syntax": "mustache",
    "template": "network,device={{topic}} value={{payload}}",
    "x": 470,
    "y": 640,
    "wires": [
      [
        "fae034e2.051fc8"
      ]
    ]
  },
```

[source](https://flows.nodered.org/node/node-red-contrib-influxdb)
```
msg.payload = [
    [{
        numValue: 10,
        randomValue: Math.random()*10,
        strValue: "message1",
        time: new Date("2015-12-28T19:41:13Z").getTime()
    },
    {
        tag1:"sensor1",
        tag2:"device2"
    }],
    [{
        numValue: 20,
        randomValue: Math.random()*10,
        strValue: "message2",
        time: new Date("2015-12-28T19:41:14Z").getTime()
    },
    {
        tag1:"sensor1",
        tag2:"device2"
    }]
];
return msg;
```
