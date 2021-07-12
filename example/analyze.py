from subprocess import call

import numpy as np
import matplotlib.pyplot as plt
xi = 1e7

dx = 0.1/32

def lauchAndReturn(**kwargs):
    # launch
    baseargs = ["../finite_differences", "1D.in"]
    for key, val in kwargs.items():
        baseargs.append(f"{key}={val}")
    call(baseargs)
    
    
    t = np.loadtxt('time.txt')
    h = np.loadtxt('output_h.txt')
    u1 = np.loadtxt('output_U1.txt')
    u2 = np.loadtxt('output_U2.txt')
    ha = np.mean(h, axis=1)
    
    # grad2 = np.mean(np.diff(np.hstack((h, h[:,0].reshape((h.shape[0],1)))), axis=1)**2/dx**2, axis=1)
    grad2 = np.mean((0.5 * (u1 + u2))**2, axis=1)
    Egrad = 1/(1+xi*ha**2) * grad2
    return t, h, ha, u1, u2, Egrad

#%%
tMax = 0.05
dt   = 5e-7
N    = 256
eta  = 0.0/N
h0   = 0.003728643755643353
box_size = 0.10
amplitude = 1e-3
tcpp, h, ha, u1, u2, Egrad = lauchAndReturn(dt=dt, 
                                            write_every=int(500*1e-8/dt),
                                            eta=eta,
                                            N=N,
                                            amplitude=amplitude,
                                            tMax=tMax,
                                            h0=h0,
                                            box_size=box_size)
#%%
# """
from matplotlib.widgets import Slider
box_length = 0.1
x = np.linspace(0, box_length, N)



x = np.linspace(0, box_length, N)
h = np.array(h)
D = 0.5*(np.array(u1)+np.array(u2))
Pi = 0.5*(np.array(u1)-np.array(u2))
U1s = np.array(u1)
U2s = np.array(u2)
# h = h - np.mean(h, axis=1)[:,np.newaxis]
U1s = U1s - np.mean(U1s, axis=1)[:,np.newaxis]
U2s = U2s - np.mean(U2s, axis=1)[:,np.newaxis]
fig, (ax1, ax2, ax3, ax4, ax5) = plt.subplots(5, 1, sharex=True, figsize=(6,11))
default = len(h)//2
l1, = ax1.plot(x, h[default]); ax1.set_ylabel('h')
l2, = ax2.plot(x, D[default]); ax2.set_ylabel('D')
l3, = ax3.plot(x, Pi[default]); ax3.set_ylabel('Pi')
l4, = ax4.plot(x, U1s[default]); ax4.set_ylabel('U1')
l5, = ax5.plot(x, U2s[default]); ax5.set_ylabel('U2')
try:
    ax1.set_ylim((-0.005,0.005))
    ax2.set_ylim((np.nanmin(D),np.nanmax(D)))
    ax3.set_ylim((np.nanmin(Pi),np.nanmax(Pi)))
    ax4.set_ylim((np.nanmin(U1s),np.nanmax(U1s)))
    ax5.set_ylim((np.nanmin(U2s),np.nanmax(U2s)))
except ValueError:
    pass

axT = fig.add_axes([0.23, 0.97, 0.65, 0.03])


sliderT = Slider(axT, 'time', 0, U2s.shape[0]-1, valinit=default, valfmt='%i')
def update(val):
    i = int(sliderT.val)
    
    l1.set_data(x, h[i])
    l2.set_data(x, D[i])
    l3.set_data(x, Pi[i])
    l4.set_data(x, U1s[i])
    l5.set_data(x, U2s[i])
    
    delta = 0.00001
    # ax1.set_ylim((np.min(h[i])-delta, np.max(h[i])+delta))
    
    fig.canvas.draw_idle()

sliderT.on_changed(update)
plt.tight_layout(rect=(0,0,1,0.98))
plt.show()
#"""
#%%
"""
fig, (ax1, ax2) = plt.subplots(2, 1)
for dt in [5e-6]:#np.logspace(-7, -9, 8):
    tcpp, h, ha, u1, u2, Egrad = lauchAndReturn(dt=dt, write_every=int(2000*1e-8/dt))
    ax1.plot(tcpp, ha)
    ax1.set_ylim((-2*ha[0], 2*ha[0]))
    ax2.semilogy(tcpp, Egrad, label=f"{dt=:.02e}")
ax2.legend()
#"""
