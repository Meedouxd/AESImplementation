import subprocess
from subprocess import Popen
import time

"""
Process Manager to keep track of current process (.exe) file 
keeps track of:

the start of the process
the time it has been running
whenever it terminates
exception handling

"""

class ProcessManager():

    is_running: bool
    aes_process: Popen | None = None
    aes_program_location: str | None = "aes.exe"
    time_started: int = -1

    def __init__(self, *, prog_loc="aes.exe"):
        self.is_running = False
        self.aes_program_locaiton = prog_loc
    
    def start_aes_program(self, key: str, input_file: str, output_file:str):
        if self.is_running:
            print("Error: Cannot start a program whenever one is currently running")
            return
    
    def get_elapsed_time(self):
        if not self.is_running:
            print("Error: cannot get elapsed time when we are not running a process")
            return
        return time.time() - self.time_started


    
