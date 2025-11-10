import socket
import threading

HOST = '127.0.0.1'  # localhost
PORT = 65432        # any free port (above 1024)

def handle_client(conn, addr):
    print(f"[+] Connected: {addr}")
    try:
        while True:
            data = conn.recv(1024)
            if not data:
                break
            print(f"[Client {addr}] {data.decode()}")
            conn.sendall(data)  # echo back to client
    except ConnectionResetError:
        print(f"[!] Connection reset by {addr}")
    finally:
        print(f"[-] Disconnected: {addr}")
        conn.close()

def main():
    server = socket.socket() #by default it uses tcp and ipv4 protocol
    server.bind((HOST, PORT))
    server.listen()

    print(f"[*] Echo server listening on {HOST}:{PORT}")

    try:
        while True:
            conn, addr = server.accept()
            client_thread = threading.Thread(target=handle_client, args=(conn, addr))
            client_thread.daemon = True
            client_thread.start()
    except KeyboardInterrupt:
        print("\n[!] Server shutting down...")
    finally:
        server.close()

if __name__ == "__main__":
    main()
