import socket
import threading

class Server:

    def __init__(self, port):
        self.port = port
        self.distance = 0
        self.distance_lock = threading.Lock()

    def run(self):
        server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server_socket.bind(('', self.port))
        server_socket.listen(2)
        print(f"Server listening on port {self.port}")

        while True:
            client_socket, addr = server_socket.accept()
            client_thread = threading.Thread(target=self.handle_client, args=(client_socket,))
            client_thread.start()

    def set_led_status(self, value_led1, value_led2, value_led3, value_led4):
        operator = "|"
        string = ""
        string = f"{value_led1}{operator}{value_led2}{operator}{value_led3}{operator}{value_led4}"
        return string

    def handle_client(self, client_socket):
        try:
            with client_socket:
                print(f"Connected to {client_socket.getpeername()}")
                while True:
                    data = client_socket.recv(1024)
                    if not data:
                        break
                    command = data.decode().strip()
                    if command.isdigit():
                        value = int(command)
                        with self.distance_lock:
                            self.distance = value
                            print(f"distance = {self.distance}")
                    elif command == "GET":
                        with self.distance_lock:
                            if self.distance == 0:
                                respuesta = self.set_led_status("HIGH", "HIGH", "HIGH", "HIGH")
                            elif self.distance < 5:
                                respuesta = self.set_led_status("HIGH", "LOW", "LOW", "LOW")
                            elif self.distance < 10:
                                respuesta = self.set_led_status("LOW", "HIGH", "LOW", "LOW")
                            elif self.distance < 15:
                                respuesta = self.set_led_status("LOW", "LOW", "HIGH", "LOW")
                            else:
                                respuesta = self.set_led_status("LOW", "LOW", "LOW", "HIGH")
                            client_socket.send(respuesta.encode() + b'\n')
        except Exception as e:
            print(f"Error: {e}")

if __name__ == "__main__":
    server = Server(1000)
    server.run()
