import shutil
import sys


def zip(zip_file, src_dir):
    shutil.make_archive(zip_file, "zip", src_dir)



if __name__=="__main__":
    if len(sys.argv) != 3:
        print("Usage: %s zip_file src_dir")
        sys.exit(20)
    else:
        zip(sys.argv[1], sys.argv[2])
