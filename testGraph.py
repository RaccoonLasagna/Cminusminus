import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from scipy.interpolate import griddata
from random import randint

# สร้างข้อมูลสำหรับ interpolation
x = np.random.rand(100) * 10
y = np.random.rand(100) * 10
z = np.random.rand(100) * 10

# สร้างกริดของจุดที่จะทำการ interpolate บน
xi = np.linspace(0, 10, 100)
yi = np.linspace(0, 10, 100)
xi, yi = np.meshgrid(xi, yi)

# ทำการ interpolate ข้อมูล
zi = griddata((x, y), z, (xi, yi), method='cubic')

# พล็อตกราฟ
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# พล็อตจุดข้อมูล
ax.scatter(x, y, z, c='r', marker='o')

# พล็อตพื้นผิวที่ interpolate ได้
ax.plot_surface(xi, yi, zi, alpha=0.2)

ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
plt.show()
