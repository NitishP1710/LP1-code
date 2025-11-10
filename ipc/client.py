import socket

HOST = '127.0.0.1'  # same as server
PORT = 65432        # same port as server

def main():
    client = socket.socket()
    client.connect((HOST, PORT))

    try:
        while True:
            msg = input("You: ")
            if msg.lower() in ("exit", "quit"):
                break
            client.sendall(msg.encode())
            data = client.recv(1024)
            print("Echo:", data.decode())
    except KeyboardInterrupt:
        print("\n[!] Client closed.")
    finally:
        client.close()

if __name__ == "__main__":
    main()
