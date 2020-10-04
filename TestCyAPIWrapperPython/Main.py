from ctypes import *
import numpy as np
import matplotlib.pyplot as plt


class SnaptoCursor(object):
    """
    Like Cursor but the crosshair snaps to the nearest x, y point.
    For simplicity, this assumes that *x* is sorted.
    """

    def __init__(self, ax, x, y):
        self.ax = ax
        self.lx = ax.axhline(color='k')  # the horiz line
        self.ly = ax.axvline(color='k')  # the vert line
        self.x = x
        self.y = y
        # text location in axes coords
        self.txt = ax.text(0.7, 0.9, '', transform=ax.transAxes)

    def mouse_move(self, event):
        if not event.inaxes:
            return

        x, y = event.xdata, event.ydata
        indx = min(np.searchsorted(self.x, x), len(self.x) - 1)
        x = self.x[indx]
        y = self.y[indx]
        # update the line positions
        self.lx.set_ydata(y)
        self.ly.set_xdata(x)

        self.txt.set_text('x=%1.2f, y=%1.2f' % (x, y))
        print('x=%1.2f, y=%1.2f' % (x, y))
        self.ax.figure.canvas.draw()

mylibc = cdll.LoadLibrary('CyAPIWrapperDLL.dll')

mylibc.fibonacci_init(1,1)
print("QQ")


mylibc.init.argtypes = []
mylibc.GetReceiveData.argtypes = [POINTER(c_uint32) , c_int ]
mylibc.GetXferLength.argtypes = []
mylibc.GetReceiveTotalNumber.argtypes = []
mylibc.ConnectDevice.argtypes = []
mylibc.StartSampleData.argtypes = []
mylibc.EndSampleData.argtypes = []
mylibc.DeviceName.argtypes = []
mylibc.DeviceName.restype = c_char_p

mylibc.init()
mylibc.ConnectDevice()
print(mylibc.DeviceName())
mylibc.StartSampleData()
while(mylibc.GetReceiveTotalNumber() <160 ):
    i=0
mylibc.EndSampleData()

Num = mylibc.GetReceiveTotalNumber()
XferSize = mylibc.GetXferLength()
XferSize = int(XferSize/4);
x = (c_ulong*XferSize)();
cast(x, POINTER(c_ulong))

a = np.array([],dtype=np.uint32);
for i in range(Num):
    size = mylibc.GetReceiveData(x,i);
    print(i, end=" ")
    a = np.append(a,x);


print("end")

# Data for plotting
t = np.arange(0,a.size) # np.arange(0.0, a.size, 1)
#a = (a & 1)
fig, ax = plt.subplots()
ax.plot(t, a)

ax.set(xlabel='time (s)', ylabel='voltage (mV)',
       title='About as simple as it gets, folks')
ax.grid()
snap_cursor = SnaptoCursor(ax, t, a)
fig.canvas.mpl_connect('motion_notify_event', snap_cursor.mouse_move)
plt.show()
