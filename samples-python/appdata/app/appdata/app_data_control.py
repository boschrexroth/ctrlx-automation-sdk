# SPDX-FileCopyrightText: Bosch Rexroth AG
#
# SPDX-License-Identifier: MIT

import os
import platform
import datetime
import json
import time


class AppDataControl():
    """AppDataControl
    """
    def __init__(self, storage_folder_name="AppDataSamplePy", storage_file_name="appdata.json"):
        """__init__
        """
        # The name of the application storage folder
        # MUST be same as specified in your *.package-manifest.json file
        self.storage_folder_name = storage_folder_name

        # The name of the storage file
        self.storage_file_name = storage_file_name

        # Gets the base storage location for all applications
        if 'SNAP' in os.environ:
            self.common_path = os.getenv("SNAP_COMMON")
        else:
            self.common_path = os.getcwd()
        self.base_storage_location = os.path.join(
            self.common_path, "solutions", "activeConfiguration")

        # Gets the application data storage location
        self.storage_location = os.path.join(
            self.base_storage_location, self.storage_folder_name)

        # Gets the application data storage file
        self.storage_file = os.path.join(
            self.storage_location, self.storage_file_name)

        self.appdata = {
            "hostname": "",
            "os": "",
            "osarch": "",
            "timestamp": "",
            "secretNumber": ""
        }

    def load(self):
        """load
        """
        print("INFO Starting load routine", flush=True)
        path = self.storage_file

        # Check if storagefile exist, if not reset with inital data
        if os.path.isfile(path) is False:
            print("INFO No application file exist", flush=True)
            return AppDataControl.set_default(self)

        # Open JSON AppData file
        json_file = open(path)
        json_data = json.load(json_file)

        # Check if loaded JSON is type dict
        if type(json_data) is dict:

            # Set appdata
            result = AppDataControl.set_appdata(self, json_data)

            if result is True:
                print("INFO Loaded application data from file",
                      path, "successfully", flush=True)
                return True
            else:
                print("ERROR Loading application data from file",
                      path, "failed! Data type missmatch", flush=True)
                return False
        else:
            print("ERROR Loading application data from file",
                  path, "failed! Data type is not dict", flush=True)
            return False

    def save(self):
        """save
        """
        print("INFO Starting save routine", flush=True)

        # Check if storage location exists
        result = AppDataControl.ensure_storage_location(self)

        # If storage location ensured, save appdata to file
        if result is True:
            path = self.storage_file
            with open(path, 'w') as filewrite:
                json.dump(AppDataControl.get_appdata(self), filewrite)

            print("INFO Saved application data to file: '", path, flush=True)
            return True

        print("ERROR Saving application data not possible", flush=True)
        return False

    def set_default(self):
        """set_default
        """
        
        # Set default application data
        data = {
            "hostname": platform.node(),
            "os": platform.system(),
            "osarch": platform.machine(),
            "timestamp": datetime.datetime.strftime(datetime.datetime.now(), '%Y-%m-%d %H:%M:%S.%f'),
            "secretNumber": "1811"
        }

        result = AppDataControl.set_appdata(self, data)

        print("INFO Created default application file", flush=True)
        return AppDataControl.save(self)

    def ensure_storage_location(self):
        """ensure_storage_location
        """
        # Check if storage location exist
        path = self.storage_location

        # If app is running as snap, check if content interface was mounted successfully
        if 'SNAP' in os.environ:
            solutions_path = os.path.join(self.common_path, "solutions")
            for i in range(4):
                print("INFO Check if content interface is mounted", flush=True)
                if os.path.isdir(solutions_path) is False:
                    print("ERROR Content interface is not mounted: Attempt", i+1, flush=True)
                    time.sleep(1.0)
                    if i >= 4:
                        return False
                else:
                    print("INFO Content interface is mounted", flush=True)

        if os.path.isdir(path) is False:
            try:
                print("INFO Creating storage location: ", path, flush=True)
                os.makedirs(path)
            except OSError:
                print("ERROR Creating storage location", path, "failed!", flush=True)
                return False

        return True

    def set_appdata(self, data):
        """set_appdata
        """
        # Check if present values are a subset of new values
        if self.appdata.keys() <= data.keys():
            self.appdata = data
            return True
        print("ERROR Setting application data failed! Present data is not a subset of new data", flush=True)
        return False

    def get_appdata(self):
        """get_appdata
        """
        return self.appdata
