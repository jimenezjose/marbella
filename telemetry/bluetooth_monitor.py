# monitor data on a bluetooth ble device.

import asyncio
import bleak
import questionary
import termcolor
# import matplotlib.pyplot as plt
# from matplotlib.animation import FuncAnimation

# key = 'Current: '

# import numpy as np
# import matplotlib.pyplot as plt
# from matplotlib.animation import FuncAnimation

# # 1. Setup the figure and axis
# fig, ax = plt.subplots()
# x = np.linspace(0, 2 * np.pi, 100)
# line, = ax.plot(x, np.sin(x))

# # 2. Define the update function
# def update(frame):
#     # Update the y-data by adding a phase shift based on the frame number
#     line.set_ydata(np.sin(x + frame / 10.0))
#     return line,

# # 3. Create the animation
# # frames: how many frames to run; interval: delay between frames in ms
# ani = FuncAnimation(fig, update, frames=100, interval=20, blit=True)

# plt.show()

def notification_handler(sender: int, data: bytearray):
  """
  Callback function that runs every time the device sends new data.
  'sender' is the characteristic handle.
  'data' is a bytearray.
  """
  del sender  # Unused variable
  try:
    print(data.decode('utf-8'), end='')
  except UnicodeDecodeError:
    print(f"Received non-UTF-8 data: {data}")
  # visualize in a plot

async def monitor(device: bleak.BleakScanner, visualize: bool = False):
  async with bleak.BleakClient(device.address) as client: # everything is running on a single thread.
    if client.is_connected:
      # print(f"\nConnected to {device.name}, device_address={device.address}\n")
      print(termcolor.colored(f"\nConnected to {device.name}, device_address={device.address}\n", "green"))

      for service in client.services:
        for characteristic in service.characteristics:
          if 'notify' in characteristic.properties:
            # Subscribe to the device's notifications.
            print(termcolor.colored(f'Subscribing to characteristic_uuid={characteristic.uuid}', "yellow"))
            await client.start_notify(characteristic.uuid, notification_handler)
      # Keep the connection open.
      while client.is_connected:
        # asyncio, is non-blocking, and allows the event loop to respond to incoming message events.
        await asyncio.sleep(1.0)

async def ask_user_to_select_device(device_names: list[str]) -> str | None:
  if not device_names:
    return
  return await questionary.select(
      "Select a device:",
      choices=sorted([device_name for device_name in device_names]),
      instruction="↑ ↓ to navigate • Enter to select • Ctrl-C to cancel",
      use_shortcuts=True,
      style=questionary.Style([
          ("qmark", "fg:#ff9d00 bold"),
          ("question", "bold"),
          ("highlighted", "bg:#3a3a3a fg:#ffffff"),
          ("instruction", "italic fg:#888888"),
      ]),
  ).ask_async()
  

async def main():
  print(f'Scanning...')
  devices = await bleak.BleakScanner.discover()
  device_name = await ask_user_to_select_device([device.name for device in devices if device.name])
  bluetooth_device = None
  for device in devices:
    if device.name == device_name:
      bluetooth_device = device
      break

  while bluetooth_device:
    try:
      await monitor(bluetooth_device, visualize=True)
      print(f'Device {bluetooth_device.name} disconnected.')
      print('Attempting to reconnect...')
    # except bleak.exc.BleakDeviceNotFoundError as e:
      # print(f"Device not found: {e}")
    except bleak.exc.BleakError as e:
      print(f"Bluetooth error: {e}")
      # print('Attempting to reconnect...')
      # await asyncio.sleep(1.0)

  if not bluetooth_device:
    print("Bluetooth device not found. Please ensure your device is on and in range.")

if __name__ == '__main__':
  try:
    # creates and runs the event loop.
    asyncio.run(main())
  except KeyboardInterrupt:
    print('\n\nAbort.')
