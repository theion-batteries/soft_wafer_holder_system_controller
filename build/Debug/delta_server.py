#!/usr/bin/env python3
logFile = "LogFile.log"
WriteToLogFile = False
from email.policy import default
import logging
import argparse
import socket
import time
from click import command
from repetier_manager import DeltaControl
if WriteToLogFile is False:
    logging.basicConfig(format='%(asctime)s %(levelname)s:%(message)s', level=logging.DEBUG)
else:
    logging.basicConfig(format='%(asctime)s %(levelname)s:%(message)s', filename = logFile, filemode='w', level=logging.DEBUG)
""" Class for parsing commands"""
class commandParser:
    delta=DeltaControl
    delta_response = ""
    s = socket.socket
    parser = argparse.ArgumentParser(description='zeromq server repetier')
    default_server = {"ip": "127.0.0.1", "port": 6550 }
    def __init__(self):
        """constructor cmd parser 
        Args: delta control
        """
        logging.info('Creating server for parsing delta commands ')
    def __init__(self, ip=str, port=int):
        """constructor cmd parser 
        Args: delta control
        """
        logging.info('Creating delta server')
        self.default_server["ip"]=ip
        self.default_server["port"]=port
    def __init__(self, deltaObj=DeltaControl):
        """constructor cmd parser 
        Args: delta control
        """
        logging.info('Creating delta server')
        self.delta=deltaObj
    ########### callbacks ##############
    def ping_server_callback(self):
        print("execute ping callback ")
        self.res = self.delta.ping_server()
        print(f"full response: {self.res}")
        return self.res
    def get_pos_callback(self):
        print("execute get pos callback ")
        self.res = self.delta.get_current_pos()
        print(f"full response: {self.res}")
        self.res = str(self.res['z'])
        print(f"z position: {self.res}")
        return self.res
    def get_gcodes_callback(self):
        print("execute get gcodes callback ")
        self.res = self.delta.get_gcodes()
        print(f"full response: {self.res}")
        return self.res
    def moveTohome_callback(self):
        print("execute move home callback ")
        self.res = self.delta.moveTohome()
        print(f"full response: {self.res}")
        return self.res
    def move_up_by_callback(self, step):
        print("execute move up by callback ")
        self.res = self.delta.move_up_by(step)
        print(f"full response: {self.res}")
        return self.res
    def move_down_by_callback(self, mm):
        print("execute move down by callback ")
        self.res = self.delta.move_down_by(mm)
        print(f"full response: {self.res}")
        return self.res
    def moveTo_Z_pos_callback(self, mm):
        print("execute move to z callback ")
        self.res = self.delta.moveTo_Z_pos(mm)
        print(f"full response: {self.res}")
        return self.res
    def moveTo_xyz_pos_callback(self, args):
        print("execute move xyz pos callback ")
        self.res = self.delta.moveTo_Coordinates(args[0],args[1],args[2])
        print(f"full response: {self.res}")
        return self.res
    def close_server(self):
        print("closing server.. ")
        self.s.close(self)
    """command lookup"""
    cmdDict = {"ping": ping_server_callback,
              "get_pos": get_pos_callback,
              "get_gcode": get_gcodes_callback,
              "move_home": moveTohome_callback,
              "close": close_server,
              "move_up_by":{"arg": move_up_by_callback},
              "move_down_by":{"arg": move_down_by_callback},
              "move_to_z":{"arg": moveTo_Z_pos_callback},
              "move_to_xyz":{"arg": moveTo_xyz_pos_callback}
    }
    def parse_cmd(self, cmd, args=None):
        if not args:
            args = None
        """
        Args:
            cmd (_type_, optional): _description_. Defaults to str.
        """
        keys = self.cmdDict.keys()
        if cmd not in keys:
            print(f"command {cmd} not found in dict")
            return False
        print(f"parsing command {cmd} arg {args}")
        if args is None:
            print(f"found cmd: {cmd}")
            self.delta_response= self.cmdDict[cmd]
            if callable(self.delta_response):
                print(f"execute callback {self.delta_response}")
                self.res = str(self.delta_response(self))
                return True
        else:
            print(f"found arg: {args}")
            self.delta_response= self.cmdDict[cmd]["arg"]
            if callable(self.delta_response):
                print(f"execute callback {self.delta_response}")
                self.res = str(self.delta_response(self, float(args)))
                return True
        return False

    def parse_script_args(self):
        self.parser.add_argument('--ip')
        self.parser.add_argument('--port')
        self.args = self.parser.parse_args()
        if self.args.ip or self.args.port is None:
            self.args.ip = self.default_server["ip"]
            self.args.port = self.default_server["port"]

    def run_server(self):
        # socket
        s=self.s()
        print("Socket successfully created")
        s.bind((self.args.ip, self.args.port))
        print("socket binded to %s" % (self.args.port))
        # put the socket into listening mode
        s.listen(5)
        print("socket is listening")
        # a forever loop until we interrupt it or
        # an error occurs
        c, addr = s.accept()
        print('Got connection from', addr)
        self.delta=DeltaControl("lab")
        while True:
            #  Wait for next request from client
            message =  c.recv(1024)
            msg_str = message.decode('utf-8')
            print("Received request: %s" % msg_str)
            if ' ' in msg_str:
                index =  msg_str.index(' ')
                print(f"index {index}")
                cmd= msg_str[0:index]
                print(f"cmd {cmd}")
                cmd_arg= msg_str[index+1::]
                print(f"args {cmd_arg}")
                print(f"Received command: {cmd} with args {cmd_arg} ")
                retVal= self.parse_cmd(cmd, cmd_arg)
            retVal= self.parse_cmd(msg_str)

            if retVal is True:
                print(f'response received from delta: {self.res} type: {type(self.res)} forward it to controller ')
                c.send(bytes(self.res,'utf-8'))
            else:
                c.send(b'command error')
            



if __name__ == "__main__":
    # create a cmd parser for repetier
    cmdParser = commandParser()
    # parse script args and socket msg
    cmdParser.parse_script_args()
    # run server
    cmdParser.run_server()

