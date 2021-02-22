import numpy as np
import scipy.linalg

k = 12 * 70E9 / 0.165**3 * (0.0258*0.001**3/12)
M1 = 0.107
M2 = 0.119
M3 = 0.119
#M1 = 87.38/1000
#M2 = 94.76/1000
#M3 = 93.76/1000

K = k*np.array([
    [+2, -2,  0],
    [-2, +4, -2],
    [ 0, -2, +4]])
    
M = np.diag(np.array([M1, M2, M3]))

#w21, Phi1 = np.linalg.eig(np.linalg.inv(M).dot(K))
w21, Phi1 = scipy.linalg.eig(K, M)

iw = w21.argsort()
w21 = w21[iw]
Phi1 = Phi1[:, iw]
wk = np.sqrt(np.real(w21))
fk = wk/(2*np.pi)

print(fk)