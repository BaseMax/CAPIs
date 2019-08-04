# C APIs

A sample and tiny RESTful API based on C programming and a fastcgi library.

## Depends

- CMake
- GNU Make
- C
- Pthread
- ...

## Testing

My current webserver: `nginx/1.15.9 (Ubuntu)`

http://localhost/?token=1&session=2

```
HI!

token = 1
session = 2
```

## Performance

$ ab -c 100 -n 10000 http://localhost/?token=1&session=2

```
This is ApacheBench, Version 2.3 <$Revision: 1843412 $>
Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
Licensed to The Apache Software Foundation, http://www.apache.org/

Benchmarking localhost (be patient)
Completed 1000 requests
Completed 2000 requests
Completed 3000 requests
Completed 4000 requests
Completed 5000 requests
Completed 6000 requests
Completed 7000 requests
Completed 8000 requests
Completed 9000 requests
Completed 10000 requests
Finished 10000 requests


Server Software:        nginx/1.15.9
Server Hostname:        localhost
Server Port:            80

Document Path:          /?token=1&session=2
Document Length:        27 bytes

Concurrency Level:      100
Time taken for tests:   0.508 seconds
Complete requests:      10000
Failed requests:        0
Total transferred:      1830000 bytes
HTML transferred:       270000 bytes
Requests per second:    19697.76 [#/sec] (mean)
Time per request:       5.077 [ms] (mean)
Time per request:       0.051 [ms] (mean, across all concurrent requests)
Transfer rate:          3520.20 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    1   0.6      0       5
Processing:     1    4   2.1      4      16
Waiting:        0    4   2.0      4      15
Total:          2    5   2.1      4      16
WARNING: The median and mean for the initial connection time are not within a normal deviation
        These results are probably not that reliable.

Percentage of the requests served within a certain time (ms)
  50%      4
  66%      5
  75%      6
  80%      7
  90%      8
  95%      9
  98%     11
  99%     12
 100%     16 (longest request)
```

## More Similar Projects

- https://github.com/kristapsdz/kcgi
- https://github.com/dmitigr/fcgi
- https://github.com/XamanSoft/CPP-FastCGI
- https://github.com/cutelyst/cutelyst
- https://github.com/tarunkant/Gopherus
- https://github.com/hollodotme/fast-cgi-client
- https://github.com/eddic/fastcgipp
- https://github.com/rtCamp/nginx-helper
- https://github.com/makasim/php-fpm-queue
- https://github.com/yookoala/gofast
- https://github.com/nginx-modules/ngx_cache_purge
- https://github.com/FastCGI-Archives/fcgi2
- https://github.com/slowriot/libtelegram
- https://github.com/mzabani/Fos
- https://github.com/hoaproject/Fastcgi
- https://github.com/wizaplace/php-fpm-status-cli
- https://github.com/alash3al/http2fcgi
- https://github.com/fbbdev/node-fastcgi
- https://github.com/wuyunfeng/Python-FastCGI-Client
- https://github.com/LukasBoersma/FastCGI

---------

Thank you from Taymindis.
