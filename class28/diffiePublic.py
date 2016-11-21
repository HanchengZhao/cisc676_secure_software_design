from Crypto.Util.number import *

p = getStrongPrime(512)
a = getRandomRange(2, p-2) 
print a
print GCD(a, p-1)
#loop until you get a GCD of 1 with p-1

base = 2
A = pow(base, a, p)
print p, base, A