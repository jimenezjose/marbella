# import numpy as np
# import matplotlib.pyplot as plt
# from matplotlib.animation import FuncAnimation

# # 1. Setup the figure and axis
# fig, ax = plt.subplots()
# # x = np.linspace(0, 2 * np.pi, 100)
# # line, = ax.plot(x, np.sin(x))

# x = []
# y = []
# line, = ax.plot(x, y)

# # 2. Define the update function
# def update(frame):
#     # Update the y-data by adding a phase shift based on the frame number
#     # line.set_ydata(np.sin(x + frame / 10.0))
#     print((frame, np.sin(frame)))
#     x.append(frame)
#     y.append(np.sin(frame))
#     line.set_data(x, y)
#     return line,

# # 3. Create the animation
# # frames: how many frames to run; interval: delay between frames in ms
# ani = FuncAnimation(fig, update, frames=1000, interval=100, blit=False)

# ax.axis('equal')
# plt.show()

# Experiment II

import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

fig, ax = plt.subplots()

# Pre-set the limits so the data is visible
ax.set_xlim(0, 100)
ax.set_ylim(-1.1, 1.1)

x = []
y = []
line, = ax.plot([], [], lw=2) # Initialize empty

def update(frame):
    x.append(len(x))
    y.append(np.sin(frame))
    line.set_data(x, y)

    print((len(x), np.sin(frame)))
    
    # Optional: Make the window 'scroll' after 100 frames
    if len(x) > 100:
        ax.set_xlim(len(x) - 100, len(x))
        
    return line,

# Note: if you use ax.set_xlim inside update, you must set blit=False
ani = FuncAnimation(fig, update, frames=np.linspace(0, 100, 200), 
                    interval=20, blit=False)

plt.show()