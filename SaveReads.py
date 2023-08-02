import argparse
import os
from subprocess import check_call


def main(args):
    files_list = []
    if args.run_type == "SE":
        file_ending = ".fq.gz"
    elif args.run_type == "PE":
        file_ending = "_1.fq.gz"
    
    for filename in os.listdir():
        if filename.endswith(file_ending):
            files_list.append(filename)

    exec_path = os.path.join(os.path.abspath(os.path.dirname(__file__)), "SaveReads")
    check_call([exec_path, args.fq_file]+files_list)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "run_type",
        metavar="run_type", 
        choices=["SE", "PE"],
        help="Define run type"
    )
    parser.add_argument(
        "fq_file", 
        help="Path to non-demultiplexed fastq-file (must be ungzipped)"
    )
    args = parser.parse_args()
    main(args)
