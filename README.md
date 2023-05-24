# Testing socket with C++

This is a simple project to test how socket works with C++. At the same time I'm testing how socket can be implemented with theards.

## Useful commands

```
# redirect all traffic to the application
sudo iptables -t nat -A OUTPUT -j REDIRECT -p tcp --to-port 4333
```
