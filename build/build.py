#!/usr/bin/python

import time
import sys
import os
import shutil
import subprocess

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
    logger.write("Build Failed")
    sys.exit()


def ExecuteGenerator(a_cmd, a_workingDir = None):
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
OutputPath          = os.path.abspath("../output/")
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
SamplesFilePath     = os.path.abspath("../src/samples/Samples.sln")

Libs                = ["Engine", "Math", "Utility"]

FinalLibName        = "DgLib"
LogFileName         = "log__" + time.strftime("%Y-%m-%d__%I-%M-%S.txt")

#Flags
FailOnBadDocs = True #Fail the build if there are errors in doc compilation
CheckSamples = True  #Check to see if the samples build   

#--- Main ---------------------------------------------------------

#Make sure we have all the correct output dirs
if not os.path.isdir(LogDir):
    os.makedirs(LogDir)
if not os.path.isdir(UnitTestResultsDir):
    os.makedirs(UnitTestResultsDir)
    
logger = Logger(LogDir + "/" + LogFileName)

logger.write("Build Started!\n")

logger.write("\nRemoving old deployment dir...\n")
if os.path.isdir(DeployDir):
    shutil.rmtree(DeployDir)
logger.write("Done!\n")

logger.write("\nCreating new deployment dir structure...\n")
os.makedirs(DeployDir)
os.makedirs(DeployDir + "/" + FinalLibName + "/")
os.makedirs(DeployDir + "/" + FinalLibName + "/3rd party")
os.makedirs(DeployDir + "/" + FinalLibName + "/docs")
os.makedirs(DeployDir + "/" + FinalLibName + "/lib")
os.makedirs(DeployDir + "/" + FinalLibName + "/samples")
logger.write("Done!\n")

platforms = ["Win32", "x64"]
LibEXEPaths = [LibEXEPath32, LibEXEPath64]

for i in range(0,len(platforms)):
    
    platform = platforms[i]

    #Build the main libs
    logger.write("\nBuilding " + platform + "...\n\n")
    args = [MSBuildPath, DgLibFilePath, "/property:Configuration=Release", "/property:Platform=" + platform, "/t:Rebuild"]
    if not Execute(args):
        logger.write("\nBuild failed! Exiting...\n")
        Exit()
    logger.write("\nBuild complete!\n")

    #Run tests
    logger.write("\nRunning tests...\n")
    TestsPath = os.path.abspath("../output/Tests/" + platform + "/Release")
    TestsEXEPath = os.path.abspath(TestsPath + "/Tests.exe")
    TestsOutputFilePath = os.path.abspath(UnitTestResultsDir + "/unit-test-results-" + platform + ".txt")
    args = [TestsEXEPath, "-out", TestsOutputFilePath]
    if not Execute(args, TestsPath):
        logger.write("\nOne or more tests failed! See test results in build/unit-test-results.txt. Exiting...\n")
        Exit()
    logger.write("All tests passed!\n")

    #Combine libraries and output to deploy dir
    logger.write("\nCombining libraries...\n\n")
    os.makedirs(DeployDir + "/" + FinalLibName + "/lib/" + platforms[i])
    OutLibFilePath = os.path.abspath(DeployDir + "/" + FinalLibName + "/lib/" + platform + "/" + FinalLibName + ".lib")
    LibPaths = [];
    for name in Libs:
        LibPaths.append(os.path.abspath(OutputPath + "/" + name + "/" + platform + "/Release/" + name + ".lib"))
    args = [LibEXEPaths[i], "/OUT:" + OutLibFilePath]
    args.extend(LibPaths)
    if not Execute(args):
        logger.write("\nFailed to create lib file. Exiting...\n")
        Exit()
    logger.write("Finished combining libs!\n")

    #Make sure samples build
    if CheckSamples:
        logger.write("\nBuilding samples. Platform: " + platform + "\n\n")
        args = [MSBuildPath, SamplesFilePath, "/property:Configuration=Release", "/property:Platform=" + platform, "/t:Rebuild"]
        if not Execute(args):
            logger.write("\nBuild failed! Exiting...\n")
            Exit()
        logger.write("\nBuild complete!\n")
    

# Copy public headers
logger.write("\nCopying source code...\n")
shutil.copytree(SrcPath, DeployDir + "/DgLib/include/")
logger.write("Done!\n")

# Create docs
logger.write("\nCreating documentation...\n\n")
args = [DoxygenEXEPath, DoxygenFilePath]
if not Execute(args, DoxygenOutPath):
    logger.write("\nFailed to run Doxygen. Exiting...\n")
    Exit()
if (FailOnBadDocs and os.stat(DoxygenErrorLog).st_size != 0):
    logger.write("\nDocumentation contains errors. Exiting...\n")
    Exit()
    
# Copy samples to package

# Check samples projects build with CMake

# Done!
logger.write("\nBuild Succeeded!")
