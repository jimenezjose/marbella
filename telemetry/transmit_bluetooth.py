import asyncio
from bleak import BleakClient

# The UUID of the characteristic you want to write to
CHARACTERISTIC_UUID = "0000ffe1-0000-1000-8000-00805f9b34fb"
DEVICE_ADDRESS = "2A563824-39AA-91D7-666D-58044BA02ABA" # Your device's MAC or UUID

async def send_data():
    async with BleakClient(DEVICE_ADDRESS) as client:
        if client.is_connected:
            # Data must be in bytes, bytearray, or memoryview
            message = "Hello BLE".encode('utf-8')
            
            # response=True waits for an acknowledgment from the device
            await client.write_gatt_char(CHARACTERISTIC_UUID, message, response=False)
            print("Data sent successfully!")

asyncio.run(send_data())