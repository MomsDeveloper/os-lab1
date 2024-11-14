from pathlib import Path
from random import randint
import sys

filename = Path(sys.argv[2])

# max storage in MB
max_storage = int(sys.argv[1])
# symbol storage in bytes
symbol_storage = 4


with open(filename, 'wb+') as f:
    for _ in range(max_storage * 1024 // symbol_storage):
        random_numbers = randint(0, 2**16 - 1).to_bytes(symbol_storage, 'little')
        f.write(random_numbers)

print(f'File {filename} was created')

# with open(filename, 'rb') as f:
#     print(int.from_bytes(f.read(symbol_storage), 'little'))
#     print(int.from_bytes(f.read(symbol_storage), 'little'))
