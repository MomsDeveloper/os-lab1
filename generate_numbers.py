from pathlib import Path
from random import randint


filename = Path('os-lab1/data/myfile.bin')

# max storage in MB
max_storage = 256
# symbol storage in bytes
symbol_storage = 4


with open(filename, 'wb') as f:
    for _ in range(max_storage * 1024 * 1024 // symbol_storage):
        random_numbers = randint(0, 2**32 - 1).to_bytes(symbol_storage, 'little')
        f.write(random_numbers)

# print(f'File {filename} was created')

with open(filename, 'rb') as f:
    print(int.from_bytes(f.read(symbol_storage), 'little'))
    print(int.from_bytes(f.read(symbol_storage), 'little'))
#     for _ in range(max_storage * 1024 * 1024 // symbol_storage):
#         print(int.from_bytes(f.read(symbol_storage), 'big'))
