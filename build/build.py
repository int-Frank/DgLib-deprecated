#!/usr/bin/python

import time
import sys
import os
import shutil
import subprocess
import ctypes

#--- Classes ------------------------------------------------------

class Logger(object):
    
    def __init__(self, a_filePath):
        self.m_file = open(a_filePath, "a")
        self.m_stdout = sys.stdout
        sys.stdout = self
        
    def __del__(self):
        sys.stdout = self.m_stdout
        self.m_file.close()
        
    def write(self, a_str):
        self.m_file.write(a_str)
        self.m_stdout.write(a_str)

    def flush(self):
        pass

#--- Functions ----------------------------------------------------

def Exit():
    ctypes.windll.user32.MessageBoxW(0, "Build Failed", "DgLib build completed", 1)
    sys.exit()


def ExecuteGenerator(a_cmd, a_workingDir = None):
    print (a_cmd)
    popen = subprocess.Popen(a_cmd, stdout=subprocess.PIPE, universal_newlines=True, cwd = a_workingDir)
    stdout_lines = iter(popen.stdout.readline, "")
    for stdout_line in stdout_lines:
        yield stdout_line

    popen.stdout.close()
    return_code = popen.wait()
    if return_code != 0:
        raise subprocess.CalledProcessError(return_code, cmd)

def Execute(a_cmd, a_workingDir = None):
    try:
        for line in ExecuteGenerator(a_cmd, a_workingDir):
            logger.write(line)
        return True
    except OSError:
        logger.write("OSError\n")
        return False
    except:
        return False
    
#--- Setup --------------------------------------------------------

LogDir              = os.path.abspath("./logs/")
UnitTestResultsDir  = os.path.abspath("./test_results/")
DeployDir           = os.path.abspath("../deploy/")
DgLibOutPath        = os.path.abspath("../bin/")
SrcPath             = os.path.abspath("../src/core/public/")
DoxygenEXEPath      = os.path.abspath("../3rd_party/doxygen/doxygen.exe")
DoxygenOutPath      = os.path.abspath("./doxygen/")
DoxygenFilePath     = os.path.abspath(DoxygenOutPath + "/Doxyfile")
DoxygenErrorLog     = os.path.abspath(DoxygenOutPath + "/doxygen-error-log.txt")
MSBuildPath         = os.path.abspath("../3rd_party/MSBuild/MSBuild.exe")
LibEXEPath64        = os.path.abspath("../3rd_party/lib.exe/x64/lib.exe")
LibEXEPath32        = os.path.abspath("../3rd_party/lib.exe/x86/lib.exe")
DoxygenPath         = os.path.abspath("../3rd_party/doxygen/doxygen.exe")
DgLibFilePath       = os.path.abspath("../DgLib.sln")

OutputLibName = "DgLib.lib"
LogFileName = "log__" + time.strftime("%Y-%m-%d__%I-%M-%S.txt")

#Do we want the build to fail if there is an error when compiling the docs?
FailOnBadDocs = True

#--- Main ---------------------------------------------------------

#Make sure we have all the correct output dirs
if not os.path.isdir(LogDir):
    os.makedirs(LogDir)
if not os.path.isdir(UnitTestResultsDir):
    os.makedirs(UnitTestResultsDir)
    
logger = Logger(LogDir + "/" + LogFileName)

logger.write("Build Started!\n")

logger.write("Removing old deployment dir...\n")
if os.path.isdir(DeployDir):
    shutil.rmtree(DeployDir)
logger.write("Done!\n")

logger.write("Creating new deployment dir structure...\n")

os.makedirs(DeployDir)
os.makedirs(DeployDir + "/DgLib")
os.makedirs(DeployDir + "/DgLib/3rd party")
os.makedirs(DeployDir + "/DgLib/docs")
os.makedirs(DeployDir + "/DgLib/lib")
os.makedirs(DeployDir + "/DgLib/samples")

logger.write("Done!\n")

platforms = ["Win32", "x64"]
LibEXEPaths = [LibEXEPath32, LibEXEPath64]

for i in range(0,len(platforms)):
    
    platform = platforms[i]
    logger.write("Building " + platform + "\n")

    args = [MSBuildPath, DgLibFilePath, "/property:Configuration=Release", "/property:Platform=" + platform, "/t:Rebuild"]

    if not Execute(args):
        logger.write("Build failed! Exiting...\n")
        Exit()

    logger.write("Build complete!\n")
    logger.write("Running tests...\n")

    TestsPath = os.path.abspath("../output/Tests/" + platform + "/Release")
    TestsEXEPath = os.path.abspath(TestsPath + "/Tests.exe")
    TestsOutputFilePath = os.path.abspath(UnitTestResultsDir + "/unit-test-results-" + platform + ".txt")
    args = [TestsEXEPath, "-out", TestsOutputFilePath]
    if not Execute(args, TestsPath):
        logger.write("One or more tests failed! See test results in build/unit-test-results.txt. Exiting...\n")
        Exit()
    
    logger.write("All tests passed!\n")
    logger.write("Combining libraries...\n")

    os.makedirs(DeployDir + "/DgLib/lib/" + platforms[i])
    OutLibPath = os.path.abspath(DeployDir + "/DgLib/lib/" + platform + "/" + OutputLibName)
    InLibPath = os.path.abspath(DgLibOutPath + "/" + platform + "/Release/")
    args = [LibEXEPaths[i], "/OUT:" + OutLibPath, "Engine.lib", "Math.lib", "Utility.lib"]
    if not Execute(args, InLibPath):
        logger.write("Failed to create lib file. Exiting...\n")
        Exit()

    logger.write("Finished combining libs!\n")

# Copy public headers
logger.write("Copying source code...\n")
shutil.copytree(SrcPath, DeployDir + "/DgLib/include/")
logger.write("Done!\n")

# Create docs
logger.write("Creating documentation...\n")
args = [DoxygenEXEPath, DoxygenFilePath]
if not Execute(args, DoxygenOutPath):
    logger.write("Failed to run Doxygen. Exiting...\n")
    Exit()
if (FailOnBadDocs and os.stat(DoxygenErrorLog).st_size != 0):
    logger.write("Documentation contains errors. Exiting...\n")
    Exit()
logger.write("Done!\n")
    
# Check samples build

# Copy samples to package

# Check samples projects build with CMake

# Done!
logger.write("Build Succeeded!")
