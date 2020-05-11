from Complexity import *

a = Complexity(2, 4)
b = Complexity(Re=10, Im=20)
print('a: ', a)
print('b: ', b)
print('')

print('a + b: ', a + b)
print('a - b: ', a - b)
print('a * b: ', a * b)
print('a / b: ', a / b)
print('')

print('abs(a): ', abs(a))
print('')

print('-a: ', -a)
print('~a: ', ~a)
print('')

print('a.sin(): ', a.sin())
print('a.cos(): ', a.cos())
print('a.tan(): ', a.tan())
print('a.cot(): ', a.cot())
print('a.arg(): ', a.arg())
print('a.exp(): ', a.exp())
print('a.log(): ', a.log())
print('')


print('a: ', a)
print('b: ', b)
print('')

print('b += a: ')
b += a
print('a: ', a)
print('b: ', b)
print('')

print('b -= a')
b -= a
print('a: ', a)
print('b: ', b)
print('')

print('b *= a')
b *= a
print('a: ', a)
print('b: ', b)
print('')

print('b /= a')
b /= a
print('a: ', a)
print('b: ', b)
