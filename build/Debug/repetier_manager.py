#!/usr/bin/env python3

""" Class for checking imported module and scanned Ip adresse"""
logFile = "LogFile.log"
WriteToLogFile = False
import logging

from requests import request
if WriteToLogFile is False:
    logging.basicConfig(format='%(asctime)s %(levelname)s:%(message)s', level=logging.DEBUG)
else:
    logging.basicConfig(format='%(asctime)s %(levelname)s:%(message)s', filename = logFile, filemode='w', level=logging.DEBUG)

class CheckPackage:
    """ Class for checking imported package"""
    import importlib

    #UseLogging()
    def __init__(self, package_name):
        self.__check_package_installed(package_name)


    def __check_package_installed(self, package_name):
        try:
            import package_name
        except ModuleNotFoundError:
            # Error handling
            self.__install(package_name)


    def __install(self, package):
        import subprocess
        import sys
        subprocess.check_call([sys.executable, "-m", "pip", "install", package])     

class CheckIP:
    """ Class for checking ip adress"""
    IP = ""

    def __init__(self, hostname = None, port=None):
        """constructor for ip port scanner

        Args:
            hostname (string, optional): select if hostname is know like localhost. Defaults to None.
            port (int, optional): [description]. select if port is know. Defaults to None.

        Returns:
            [srting]: the ip adress
        """
        if hostname is not None:
            self.__getip_by_hostname(hostname)
        if port is not None:
            self.__getip_by_port(port)

    def __getip_by_hostname(self, hostname):    
        import socket
        while True:
            try: 
                IP = socket.gethostbyname(hostname)
                logging.info(f'found ip: {str(IP)}')
                break
            except socket.error as err:
                logging.error("no ip found, check the server connection, retry..")

                continue
        self.IP = IP
        return IP

    def __getip_by_port(self, port): 
        pass   
    


""" Class implementation of delta control module"""
class DeltaControl:
    """ class to control the delta robot remotely via web api """
    import requests
    import json

    
    __HOSTNAME = {"office":"theion-dms01.local","lab":"192.168.0.201"}
    __IP = ""
    __PORT = "3344"
    _SLUG = ""                                                           #private printer slug
    _API_KEY = ""                                                        #private printer apikey
    _URL = ""                                                            #private URL for api requests
    _printer_name = ""
    _GCODES = {}
    _x, _y, _z = 0, 0, 0
    Response=str
    def __init__(self, location = "office" or "lab"):
        """constructor delta repetier 

        Args:
            location ([string]): location of work
        """

        logging.info('Creating object for controlling delta robot remotely')

        CheckPackage("requests")  
        IP = CheckIP(self.__HOSTNAME[location])
        self.__IP = getattr(IP, "IP")
        self.__get_server_info()
        logging.info("Configuration saved succesffully")

    def setter(self, old_att, new_att):
        """setter method

        Args:
            old_att ([type]): [description]
            new_att ([type]): [description]

        Returns:
            [type]: [description]
        """
        old_att = new_att
        return new_att

    def getter(self, attribute):  # not pythonic way, use builtin getattr and setattr
        """getter method

        Args:
            attribute ([type]): [description]

        Returns:
            [type]: [description]
        """
        return attribute 

    def __get_server_info(self):
        """get_server_info: get internal private data from server and save it to self class
        """
        info_url = "http://{}:{}/printer/info".format(self.__IP, self.__PORT)
        #logging.INFO("info_url") 
        response = self.requests.get(info_url)
        output = response.json()
        logging.info(f'{output}')
        logging.info(f'content type: {response.headers["Content-Type"]}')
        self._API_KEY = output['apikey']
        logging.info(f"apikey: {self._API_KEY}")
        self._SLUG = output['printers'][0]['slug']
        logging.info(f"slug: {self._SLUG}")
        logging.info(f"http response: {response.status_code}")
        self._printer_name = output['printers'][0]['name']
        logging.info(f"name: {self._printer_name}")
        self._URL = f'http://{self.__IP}:{self.__PORT}/printer/api/{self._SLUG}'
        logging.info(f"Api URl: {self._URL}")

    def execute_command(self, command, data=None, response_text_print = False):
        """execute_command: method for executing the commands of the web api:
        https://prgdoc.repetier-server.com/v1/docs/index.html#/en/web-api/websocket/
        Args:
        command (string): the command to execute. e.g: 'send'
        data (json) : data for specific commands. e.g: '{'cmd': 'G1 Z100'}'
        Defaults to None.
        response_text_print: to print response text, default to false
        Returns:
        string: response from requests.get
        """
        data = data or {}

        params = {
            'a': command,
            'data': self.json.dumps(data),
            'apikey': self._API_KEY,
        }

        resp = self.requests.get(self._URL, params=params)

        logging.info(f'Requested: {resp.url}')
        logging.info(f'Got: {resp.status_code}')
        if response_text_print is True:
            logging.info(resp.json())
        print(f"full response: {resp}")
        Response=resp
        return resp

    def ping_server(self):
        """ping_server: check server if responding with http code 200

        Returns:
            bool: true if yes, false if not
        """
        if self.execute_command("ping").status_code == 200:
            logging.info("Server is responding")
            return True
        else:
            logging.info("server is not responding")
            return False


    def move_up_by(self, step):
        """move_up_by: move delta down by the given difference step in mm
        Args:
            step (float): step in millimiters, between 0 and 315 mm  
        """
        current_pos = self.get_current_pos()
        current_z = current_pos["z"]
        logging.info(f"current z pos: {current_z}")

        if current_z - step > 0 and current_z + step < 315:    
            new_z = current_z + step
            self.execute_command("send", {'cmd': f'G1 Z{new_z}'})
            
            logging.info(f"moving to: {new_z}")
        else:
            raise Exception("choose value between 0 and 315")
        return True
    def move_down_by(self, step):
        """move_down_by: move delta down by the given difference step in mm
        Args:
            step (float): step in millimiters, between 0 and 315 mm  
        """
        current_pos = self.get_current_pos()
        current_z = current_pos["z"]
        logging.info(f"current z pos: {current_z}")
        if current_z - step > 0 and current_z - step < 315:
            new_z = current_z - step
            self.execute_command("send", {'cmd': f'G1 Z{new_z}'})
            
            logging.info(f"moving to: {new_z}")
        else:
            raise Exception("choose value between 0 and 315")
        return True

    def moveTo_Z_pos(self, Z_pos):
        """moveTo_Z_pos: move delta down to given z position
        Args:
            Z_pos (float): z position in millimiters, between 0 and 315 mm  while 315 is max up and 0 is max down
        """
        if Z_pos >= 0 and Z_pos <= 315:
            self.execute_command("send", {'cmd': f'G1 Z{Z_pos}'})
            logging.info({'cmd': f'G1 Z{Z_pos}'})
            logging.info(f"moving to: {Z_pos}")
        else:
            raise Exception("choose value between 0 and 315")
        return True
  
    def  moveTo_Coordinates(self, x,y,z):
        """moveTo_Coordinates: move delta to given coordinates
        Args:
            coordinates (float): coordinates in millimiters, x,y = {-7,5..7,5} mm , z = {0..3015}  while 315 is max up and 0 is max down
        """
        self.execute_command("send", {'cmd': f'G1 X{x} Y{y} Z{z}'})
        logging.info({'cmd': f'G1 X{x} Y{y} Z{z}'})
        logging.info(f"moving to: x = {x}, y = {y}, z = {z}")


    def moveTohome(self):
        """moveTohome: move all axis home
        """
        self.execute_command("send", {'cmd': 'G28'}) #home all axis
        logging.info("moving to home all axis")
        return True

    def run_gcode_by_id(self, gcode_id):
        """run_gcode: run gcode by id

        Args:
            gcode_id (int): gcode id, get it from model list function
        """
        self.execute_command("copyModel", {"id" : gcode_id, "autostart" : True})      
        logging.info(f"printing gcode with id: {gcode_id}")

    def run_gcode_by_name(self, gcode_name):
        """run_gcode: run gcode by name, 

        Args:
            gcode_name (string): gcode name, get it from get gcode names function
        """
        gcodes_dict = self.get_gcodes()
        g_id = gcodes_dict.get(gcode_name)
        self.execute_command("copyModel", {"id" : g_id, "autostart" : True})      
        logging.info(f"printing gcode with id: {g_id}")

    def list_all_gcode(self):
        """list_all_gcode: list all gcode uploaded
        """
        logging.info("listing gcodes: ")
        ret = self.execute_command("listModels")
        return ret
        

    def list_all_printer(self):
        """list_all_printer: list all printer configured
        """
        logging.info("listing printers: ")
        ret = self.execute_command("listPrinter")
        
    
    def get_gcodes(self):
        """get_gcodes: create a dict with name and id

        Returns:
            dict: gcodes dict
        """
        gcodes = {"name": "id"}
        output = self.list_all_gcode()
        data = output.json()
        i = 0
        for ID in data['data']:
            ID = data['data'][i]['id']
            #logging.info("id: ", ID)
            for NAME in data['data']:
                NAME = data['data'][i]['name']
                #logging.info("name: ", NAME)
                i = i + 1
                gcodes.update({NAME:ID})
                break
        logging.info(gcodes)
        self._GCODES = gcodes
        return gcodes

    def get_current_pos(self):
        print("get position X,Y and Z")
        output = self.execute_command("stateList")
        data = output.json()
        logging.info(data)
        x_pos = data['TheionDMS']['x']
        y_pos = data['TheionDMS']['y']
        z_pos = data['TheionDMS']['z']

        self._x = x_pos
        self._y = y_pos
        self._z = z_pos
        POS = {"x":x_pos, "y":y_pos, "z":z_pos}
        logging.info(POS)
        return POS

if __name__ == "__main__":
 
    # connect to repetier server
    delta_control = DeltaControl(location="lab")
    #delta_control.moveTohome()
    #pos = delta_control.get_current_pos()
    x=0
    while True: 
        x=x+1
        delta_control.move_down_by(1)
        if x is 100:
            break
    #delta_control.moveTo_Z_pos(250)
    #pos = delta_control.get_current_pos()
    #logging.info("actual pos:", pos["z"])
    #gcodes = delta_control.get_gcodes()
    #delta_control.run_gcode_by_name("gcode_sami")
    #pos = delta_control.get_current_pos()
    #logging.info("actual pos:", pos["z"])
    