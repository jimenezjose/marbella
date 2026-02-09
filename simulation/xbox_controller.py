import pygame
import numpy as np


def control(joystick: pygame.joystick.Joystick):
  DEADZONE = 0.1
  throttle = 0.0
  rumble = False
  connected = True
  while connected:
    for event in pygame.event.get():
      if event.type == pygame.QUIT:
        connected = False
      elif event.type == pygame.JOYDEVICEREMOVED:
        print("Joystick disconnected.")
        connected = False
      # Handle joystick events
      elif event.type == pygame.JOYBUTTONDOWN:
        print(f"Button {event.button} pressed") 
      elif event.type == pygame.JOYBUTTONUP:
        print(f"Button {event.button} released")
      elif event.type == pygame.JOYAXISMOTION:
        print(f"Axis {event.axis} moved to {event.value}")
        # Left Joystick. 
        # Horizontal Axis: 0 | Vertical Axis: 1
        #          1
        #          |
        #          |
        # -1 ------+------ 1
        #          |
        #          |
        #         -1
        if event.axis == 1:
          throttle = -event.value
          if abs(throttle) < DEADZONE:
            throttle = 0.0
          rumble = True if throttle > 0 else False
          # send this to the bluetoothe device.
          print(f"Throttle: {throttle}")
      elif event.type == pygame.JOYHATMOTION:
        # event.hat is the hat number, event.value is a tuple (x, y)
        print(f"Hat {event.hat} moved to {event.value}")

    if rumble:
      joystick.rumble(throttle, throttle, 100)

  pygame.joystick.quit()


def connect_joystick() -> pygame.joystick.Joystick:
  pygame.joystick.init()
  joystick_count = pygame.joystick.get_count()
  if joystick_count == 0:
    print("No joystick connected. Please connect your Xbox controller.")
  while joystick_count == 0:
    pygame.joystick.quit()
    pygame.joystick.init()
    joystick_count = pygame.joystick.get_count()
  if joystick_count > 1:
    raise NotImplementedError("Multiple joysticks connected not supported yet.")
  joystick = pygame.joystick.Joystick(0)
  joystick.init()
  print(f"Connected joystick: '{joystick.get_name()}'")
  return joystick


def main():
  pygame.init()
  joystick = connect_joystick()
  control(joystick)
  pygame.quit()


if __name__ == "__main__":
  main()