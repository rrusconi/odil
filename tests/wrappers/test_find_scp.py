import glob
import multiprocessing
import os
import subprocess
import time
import unittest

import odil

class Generator(object):
    def __init__(self):
        self._responses = []
        self._response_index = None
        
    def initialize(self, message):
        data_set_1 = odil.DataSet()
        data_set_1.add(
            odil.registry.PatientName, odil.Value.Strings(["Hello^World"]))
        data_set_1.add(odil.registry.PatientID, odil.Value.Strings(["1234"]))
        self._responses.append(data_set_1)
        
        data_set_2 = odil.DataSet()
        data_set_2.add(
            odil.registry.PatientName, odil.Value.Strings(["Doe^John"]))
        data_set_2.add(odil.registry.PatientID, odil.Value.Strings(["5678"]))
        self._responses.append(data_set_2)
        
        self._response_index = 0
    
    def done(self):
        return (self._response_index == len(self._responses))
    
    def next(self):
        self._response_index += 1
    
    def get(self):
        return self._responses[self._response_index]

class TestFindSCP(unittest.TestCase):
    def test_find_scp_release(self):
        process = multiprocessing.Process(target=self.run_server)
        process.start()
        time.sleep(0.5)
        data_sets = self.run_client()
        process.join(2)
        server_code = process.exitcode
        
        self.assertEqual(server_code, 0)
        
        self.assertEqual(len(data_sets), 2)
        
        self.assertEqual(len(data_sets[0]), 2)
        
        self.assertEqual(
            [x for x in data_sets[0].as_string(odil.registry.PatientName)], 
            ["Hello^World"])
        self.assertEqual(
            [x for x in data_sets[0].as_string(odil.registry.PatientID)], 
            ["1234"])
        
        self.assertEqual(
            [x for x in data_sets[1].as_string(odil.registry.PatientName)], 
            ["Doe^John"])
        self.assertEqual(
            [x for x in data_sets[1].as_string(odil.registry.PatientID)], 
            ["5678"])
    
    def run_client(self):
        command = [
            "findscu",
            "-P", "-k", "QueryRetrieveLevel=PATIENT",
            "-k", "PatientID=*", "-k", "PatientName",
            "-q", "-X",
            "localhost", "11113"]
        
        retcode = subprocess.call(command)
        if retcode != 0:
            return []
        
        files = sorted(glob.glob("rsp*"))
        data_sets = [odil.read(x)[1] for x in files]
        for file_ in files:
            os.remove(file_)
        
        return data_sets

    def run_server(self):
        called = False

        association = odil.Association()
        association.set_tcp_timeout(1)
        association.receive_association("v4", 11113)

        find_scp = odil.FindSCP(association)
        generator = Generator()
        find_scp.set_generator(generator)

        message = association.receive_message()
        find_scp(message)
        
        termination_ok = False

        try:
            association.receive_message()
        except odil.AssociationReleased:
            termination_ok = True
        except odil.AssociationAborted:
            pass
        
        if called and termination_ok:
            return 0
        else:
            return 1

if __name__ == "__main__":
    unittest.main()
