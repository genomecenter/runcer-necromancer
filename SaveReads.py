import sys, os
from subprocess import check_call

if __name__ == "__main__":
    files_list = []
    for filename in os.listdir():
        if filename.endswith("_1.fq.gz"):
            files_list.append(filename)

    exec_path = os.path.join(os.path.abspath(os.path.dirname(__file__)), "SaveReads")
    check_call([exec_path, sys.argv[1]]+files_list)