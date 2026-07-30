import can

def device_id(PCAN_DEVICE):

    try:
        bus = can.interface.Bus(interface='pcan', channel=PCAN_DEVICE)
        
        # Retrieve the unique device number
        dev_num = bus.get_device_number()
        
        #print(f"Connected to PCAN! Device Number: {dev_num}")
        return dev_num

    except can.CanError as e:
        #print(f"Failed to connect: {e}")
        return 0

    finally:
        # Ensure the bus is closed properly
        if 'bus' in locals():
            bus.shutdown()

if __name__ == "__main__" :

    dev_n = device_id("PCAN_USBBUS2")
    print(dev_n)
