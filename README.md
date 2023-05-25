# Testing socket with C++

This is a simple project to test how socket works with C++. At the same time I'm testing how socket can be implemented with theards.

As part of the project I implement a library to encode/decode text in base64.

## Useful commands

This commands are used to redirect all the OUTGOING traffic to the server application.

```
# redirect all traffic to the application
sudo iptables -t nat -A OUTPUT -j REDIRECT -p tcp --to-port 4333

# clear iptables
sudo iptables -t nat -F
```

## Communication protocol

**Message with network information**

| s_addr        | sin_port      | message                 |
|---------------|---------------|-------------------------|
| 32 bit number | 16 bit number | base68::encode(message) |
| 6 bytes       | 3 bytes       | tbd                     |

**Message without network information**

| message                 |
|-------------------------|
| base68::encode(message) |
