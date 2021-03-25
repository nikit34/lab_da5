from random import *
from string import ascii_letters


NUMBERS_LINES = 100000

with open('tests', 'w') as f:
    f.write(''.join(choice('abcdef') for _ in range(randint(100,1000))) + '\n')
    for i in range(1, NUMBERS_LINES):
        f.write(''.join(choice('abcdef') for _ in range(randint(1,10))) + '\n')
