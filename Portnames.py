from serial.tools import list_ports

def find_sprd_port():
    ports = list_ports.comports()
    for port in ports:
        if "SPRD U2S Diag" in port.description:
            return port.device
    return "NOT_FOUND"

# Main block
if __name__ == "__main__":
    port = find_sprd_port()
    print(port)