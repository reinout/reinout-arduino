import os
import glob

LIB_SOURCES = ['src/arduinoServoWrapper/testServo/']
LIB_DIR = 'lib'
PATTERNS = ['*.h', '*.cpp']


def main():
    """update-libraries program: make symlinks to the libraries in lib/"""
    # Assumption: we're in the main buildout directory.
    cwd = os.getcwd()  # Current working directory
    lib_dir = os.path.abspath(LIB_DIR)
    for lib_source in LIB_SOURCES:
        source_dir = os.path.abspath(os.path.join(cwd, lib_source))
        for pattern in PATTERNS:
            for pathname in glob.glob(os.path.join(source_dir, pattern)):
                filename = os.path.basename(pathname)
                target = os.path.join(lib_dir, filename)
                if not os.path.exists(target):
                    os.symlink(pathname, target)
                    print("Symlinked %s" % target)
