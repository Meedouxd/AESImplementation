from process_manager import ProcessManager
import random
import time

"""
Main file for testing the AES encryption algorithm and graphing the results
Author: Chase Morgan 10/06/25
"""
 

input_file:str = ""
valid_file: bool = False

## Keep track of all key bits flipped
key_bit_flip_list: list[int] = []

## Keep track of all file bits flipped
file_bits_flip_list: list[int] = []

print("****** AES Testing Program ******")
input_file = input("Enter the location of the plain text file to encrypt:\n")


try:
    with open(input_file, "rb") as file:
        file.close()
        valid_file = True
        
except FileExistsError:
    print("Error: File does not exist. enter again")
except FileNotFoundError:
    print("Error: File Not Found")
except Exception as e:
    print("Error: general exception")
    print(e)

## Keep attempting to get valid file until it is found
while not valid_file:
    input_file = input("Enter the location of the plain text file to encrypt:\n")

## Creating a copy of the input file

with open(input_file, "rb") as read_file:
    contents = read_file.read()
    read_file.close()

    with open(read_file.name+"_copy", "wb") as write_file:
        write_file.write(contents)
        write_file.close()

process_manager = ProcessManager(prog_loc="aes.exe")


def next_random_bit(type:str):
    if(type == "key"):
        random_bit = random.randint(0, 127)
        while random_bit in key_bit_flip_list:
            random_bit = random.randint(0,127)
        key_bit_flip_list.append(random_bit)
        return random_bit

    ## TODO: calculate range of file based on byte count in file
    if(type == "file"):
        random_bit = random.randint(0, 127)
        while random_bit in key_bit_flip_list:
            random_bit = random.randint(0,127)
        file_bits_flip_list.append(random_bit)
        return random_bit
    
    print("Error: Wrong type inputted. must be key or file")


## Key Bit Flipping
for i in range(20):
    round = i+1
    bit_flip_index = next_random_bit("key")
    print(f"AES Encryption Algorithm Key Flipping Round {round}")
    print(f"Key Bit to Flip:{bit_flip_index}")

    output_file_name:str = "key_flip_" + str(bit_flip_index)

    # process_manager.start_aes_program(bit_flip_index)

    # while(process_manager.is_running):
    #     time.sleep(0.2)

## File Bit Flipping
for i in range(20):
    round = i+1
    bit_flip_index = next_random_bit("file")
    print(f"AES Encryption Algorithm File Flipping Round {round}")
    print(f"File Bit to Flip:{bit_flip_index}")

    output_file_name: str = "file_flip_" + str(bit_flip_index)
    # process_manager.start_aes_program(bit_flip_index)

