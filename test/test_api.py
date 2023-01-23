import unittest
import requests

import os
FILE_DIR = os.path.dirname(os.path.realpath(__file__))

class TestResponseCode(unittest.TestCase):
    def test_valid_json_data(self):
        with open(FILE_DIR + '/data/contains_point.json', 'r') as file:
            txt = file.read()
            response = requests.post('http://localhost:5000/contains', data=txt)
            self.assertEqual(response.status_code, 200)

    def test_invalid_json_data(self):
        with open(FILE_DIR + '/data/invalid_json.json', 'r') as file:
            txt = file.read()
            response = requests.post('http://localhost:5000/contains', data=txt)
            self.assertEqual(response.status_code, 403)
            self.assertEqual(response.json()['status'], 'Error')

class TestContains(unittest.TestCase):
    def test_contains_point(self):
        with open(FILE_DIR + '/data/contains_point.json', 'r') as file:
            txt = file.read()
            response = requests.post('http://localhost:5000/contains', data=txt)
            self.assertEqual(response.json()['polygon_contains_point'], True)
    def test_does_not_contain_point(self):
        with open(FILE_DIR + '/data/not_contains_point.json', 'r') as file:
            txt = file.read()
            response = requests.post('http://localhost:5000/contains', data=txt)
            self.assertEqual(response.json()['polygon_contains_point'], False)

if __name__ == '__main__':
    unittest.main()
