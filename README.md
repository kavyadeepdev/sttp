# Simple Text Transfer Protocol (STTP)

A simple client-server application that communicates over TCP using STTP (Simple Text Transfer Protocol). All messages are encrypted with a Caesar cipher before being sent over the network.

## Project Structure

```
spc-aat/
├── include/
│   ├── constants.h      # Port, buffer size, and cipher shift
│   └── crypto.h         # Encryption function declarations
├── src/
│   ├── server.c         # Server that listens for and handles commands
│   ├── client.c         # Interactive client that connects to the server
│   └── crypto.c         # Caesar cipher encrypt/decrypt + send helper
├── bin/                  # Compiled binaries (created by make)
├── Makefile
└── README.md
```

## How It Works

1. The **server** listens on port 9090 and waits for a client to connect.
2. The **client** connects to the server and lets you type commands.
3. Every message sent between client and server is **encrypted** using a Caesar cipher (shift of 3). The receiver decrypts it before processing.

## Building

Make sure you have `gcc` and `make` installed, then run:

```bash
make
```

This creates `bin/server` and `bin/client`.

## Running

You need **two terminal windows** — one for the server and one for the client.

**Terminal 1 — Start the server:**

```bash
./bin/server
```

**Terminal 2 — Start the client:**

```bash
./bin/client
```

## Client Commands

Once the client is running, you can type these commands:

| Command | What it does |
|---------|-------------|
| `CONNECT` | Connect to the server |
| `PING` | Server replies with `PONG` |
| `HELP` | Lists all available commands |
| `VERSION` | Shows the protocol version |
| `IDENTIFY` | Shows the server name |
| `TIME` | Shows the current server time |
| `UPTIME` | Shows how long the server has been running |
| `STATUS` | Server replies with `OK` |
| `ECHO <text>` | Server echoes back whatever you type after ECHO |
| `INFO` | Shows server description |
| `DISCONNECT` | Disconnect from the server (client stays open) |
| `EXIT` | Disconnect and quit the client |

## Example Session

```
> CONNECT
Connected to server!
> PING
Server: PONG
> TIME
Server: 14:30:22
> ECHO hello world
Server: hello world
> DISCONNECT
Server: CONNECTION_CLOSED
Disconnected.
> EXIT
```

## Cleaning Up

To delete the compiled binaries:

```bash
make clean
```
