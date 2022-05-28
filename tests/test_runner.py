import tarfile
import urllib.parse
import requests
import tempfile
import shutil
import os
from os import walk, listdir
import sys
import multiprocessing
import subprocess
import itertools
import config
from typing import List


def download_archive(url: str) -> str:
    """
    :param url: url of .tar.gz file to download and unpack
    :return: temporary directory with unpacked tar. Caller is responsible for deleting the directory when done with it.
    """

    try:
        with requests.get(url, stream=True) as response:
            with tarfile.open(fileobj=response.raw, mode="r:gz") as tar:
                tempdir = tempfile.mkdtemp()
                tar.extractall(path=tempdir)
                return tempdir
    except Exception as e:
        print(f"Couldn't download archive {url}: ", e)
        if tempdir is not None:
            shutil.rmtree(tempdir)


def get_files_in_dir(dir_path: str) -> List[str]:
    res = list([])
    for f in listdir(dir_path):
        if os.path.isfile(os.path.join(dir_path, f)):
            res.append(os.path.join(dir_path, f))
        elif os.path.isdir(os.path.join(dir_path, f)):
            res.extend(get_files_in_dir(os.path.join(dir_path, f)))
    return res


def run_sat_solver(sat_solver_path: str, cnf_file: str) -> bool:
    stdout = subprocess.run([sat_solver_path, cnf_file], capture_output=True).stdout
    return stdout == b'SAT\r\n'


def run_tests_for_archive(archive_uri: str, sat_solver_path: str, expected_result: True):
    tempdir = download_archive(urllib.parse.urljoin(config.base_url, archive_uri))
    file_paths = get_files_in_dir(tempdir)

    for file_path in file_paths:
        result = run_sat_solver(sat_solver_path, file_path)
        if result != expected_result:
            expected_text = "SATISFIABLE" if expected_result else "UNSATISFIABLE"
            result_text = "SATISFIABLE" if result else "UNSATISFIABLE"
            print(f"Wrong result for file {file_path} from {archive_uri}:\n"
                  f"Expected {expected_text}, got {result_text}", file=sys.stderr)


def run_tests(sat_solver_path: str):
    for archive in config.satisfiable_cnf_archives:
        run_tests_for_archive(archive, sat_solver_path, True)

    for archive in config.unsatisfiable_cnf_archives:
        run_tests_for_archive(archive, sat_solver_path, False)


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: ./test_runner.py path_to_sat_solver")
    run_tests(sys.argv[1])
