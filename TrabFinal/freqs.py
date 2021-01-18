import numpy as np

K = 9 * 12*9.115E6/3**3
MA = 75859.375
MB = 77968.75

mK = np.array([
    [K,  -K,   0,   0,   0],
    [-K, 2*K,  -K,   0,   0],
    [0,  -K, 2*K,  -K,   0],
    [0,   0,  -K, 2*K,  -K],
    [0,   0,   0,  -K, 2*K],
])

mM = np.diag(np.array([MA, MB, MB, MB, MB]))

w21, Phi1 = np.linalg.eig(np.linalg.inv(mM).dot(mK))
iw = w21.argsort()
w21 = w21[iw]
Phi1 = Phi1[:, iw]
wr = np.real(w21)
wk = np.sqrt(w21)
fk = wk/(2*np.pi)

print(fk)
