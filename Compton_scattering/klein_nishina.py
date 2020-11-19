

import numpy as np
import matplotlib.pyplot as plt



# print("hello world")
#
# Eg = np.linspace(0,5000,100)
# me = 511
# Egp = Eg/(1+2*Eg/me)
#
#
# plt.plot(Eg,Egp,'-')
# plt.show()

r0 = 2.817908e-15 #Classical electron radius

#dSigma/dCos(theta)
costheta = np.linspace(-1,1,100)
Eg = 600
me = 511

P = 1/(1+Eg/me*(1-costheta))

dSigmadCostheta = np.pi*r0**2*P**2*(P+1/P+costheta**2-1);

fig0 = plt.figure(0)
ax = fig0.gca()
plt.plot(costheta,dSigmadCostheta)
plt.xlabel('cos(theta)')
plt.ylabel('dsigma\dcos(theta)')
ax.set_xticks(np.arange(-1, 1.25, 0.25))
plt.grid(b=True, which='major', color='#666666', linestyle='-')
plt.show()

#dSigma/dtheta
theta = np.linspace(0,np.pi,181)
P2 = 1/(1+Eg/me*np.cos(theta))

dSigmadTheta= np.pi*r0**2*P2**2*(P2+1/P2+np.sin(theta)**2-1)*np.sin(theta)

print("cos(180): ", np.cos(180))

fig1 = plt.figure(1)
ax = fig1.gca()
plt.plot(theta,dSigmadTheta)
plt.xlabel('theta')
plt.ylabel('dsigma\dcos(theta)')
ax.set_xticks(np.arange(0, 3.2, 20))
plt.grid(b=True, which='major', color='#666666', linestyle='-')
plt.show()
