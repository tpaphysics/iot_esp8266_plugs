import pylab as plt
import numpy as np


time_max = 2 

x = np.linspace(0, time_max, 256, endpoint=True)
y = x**2

p = np.linspace(time_max, 2*time_max, 256, endpoint=True)
q = -(p-2*time_max)**2 + 2*time_max**2

plt.grid(True)
plt.title("Angle")
plt.xlabel("time (a.u)")
plt.ylabel("θ")
plt.xlim(0,2*time_max)
plt.ylim(0,2*time_max*time_max)

plt.plot(x,y)
plt.plot(p,q)
plt.plot(2,4,marker='s', color='red')

plt.plot([0, time_max], [2*time_max,2*time_max] , 'k--', color = 'red')
plt.plot([ time_max, time_max] ,[0, 2*time_max ], 'k--', color = 'red')

plt.legend(['θ1', 'θ2', 'tm'], loc=9)
plt.savefig('./oled_angle.png')
plt.show()