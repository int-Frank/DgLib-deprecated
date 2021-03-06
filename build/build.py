#!/usr/bin/python

import time
import sys
import os
import shutil
import subprocess

#--- Settings -----------------------------------------------------

#You will need to set the path to these
MSBuildPath         = os.path.abspath("C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/MSBuild/15.0/Bin/MSBuild.exe")
LibEXEPath64        = os.path.abspath("C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/14.16.27023/bin/HostX64/x64/lib.exe")
LibEXEPath32        = os.path.abspath("C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/14.16.27023/bin/HostX86/x86/lib.exe")
DoxygenEXEPath      = os.path.abspath("D:/Programs/doxygen/bin/doxygen.exe")

FailOnBadDocs       = False #Fail the build if there are errors in doc compilation

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

# These paths shouldn't need to change
LogDir              = os.path.abspath("./logs/")
UnitTestResultsDir  = os.path.abspath("./test_results/")
OutputPath          = os.path.abspath("../output/") #build output
SrcPath             = os.path.abspath("../src/core/public/")
SamplesPath         = os.path.abspath("../src/samples/")
IPCSrcPath          = os.path.abspath(SamplesPath + "/IPC_TCP_Common/")
AppLibSrcPath       = os.path.abspath(SamplesPath + "/AppLib/")
ImGuiPath           = os.path.abspath("../3rd_party/imgui/")
DoxygenOutPath      = os.path.abspath("./doxygen/")
DeployDir           = os.path.abspath("../deploy/")
DoxygenFilePath     = os.path.abspath(DoxygenOutPath + "/Doxyfile")
DoxygenErrorLog     = os.path.abspath(DoxygenOutPath + "/doxygen-error-log.txt")
DgLibFilePath       = os.path.abspath("../DgLib.sln")
SamplesFilePath     = os.path.abspath("../src/samples/Samples.sln")

Libs                = ["Engine", "Math", "Utility", "Containers"]
AppLibName          = "AppLib"

FinalLibName        = "DgLib"
LogFileName         = "log__" + time.strftime("%Y-%m-%d__%I-%M-%S.txt")

#platforms = ["Win32", "x64"]
#LibEXEPaths = [LibEXEPath32, LibEXEPath64]
platforms = ["x64"]
LibEXEPaths = [LibEXEPath64]
configurations = ["Debug", "Release"]

#--- Main ---------------------------------------------------------

#Make sure we have all the correct output dirs
if not os.path.isdir(LogDir):
    os.makedirs(LogDir)
if not os.path.isdir(UnitTestResultsDir):
    os.makedirs(UnitTestResultsDir)
    
logger = Logger(LogDir + "/" + LogFileName)

logger.write("Build Started!\n")

if FinalLibName == "":
    logger.write("No Lib name\n")
    Exit()

logger.write("\nRemoving old deployment dirs...\n")
if os.path.isdir(DeployDir):
    shutil.rmtree(DeployDir)
logger.write("Done!\n")

logger.write("\nCreating new deployment dir structure...\n")
os.makedirs(DeployDir)
os.makedirs(DeployDir + "/" + FinalLibName + "/")
os.makedirs(DeployDir + "/" + FinalLibName + "/docs")
os.makedirs(DeployDir + "/" + FinalLibName + "/lib")
os.makedirs(DeployDir + "/" + FinalLibName + "/IPC")
os.makedirs(DeployDir + "/" + FinalLibName + "/IPC/lib")
os.makedirs(DeployDir + "/" + FinalLibName + "/IPC/include")
logger.write("Done!\n")

for i in range(0, len(platforms)):

    platform = platforms[i]
    LibPlatformDir = DeployDir + "/" + FinalLibName + "/lib/" + platform
    os.makedirs(LibPlatformDir)

    AppLibPlatformDir = DeployDir + "/" + FinalLibName + "/" + AppLibName + "/lib/" + platform
    os.makedirs(AppLibPlatformDir)
    
    for configuration in configurations:
    
        #Build the main libs
        logger.write("\nBuilding " + platform + "...\n\n")
        args = [MSBuildPath, DgLibFilePath, "/property:Configuration=" + configuration, "/property:Platform=" + platform, "/t:Rebuild"]

        if not Execute(args):
            logger.write("\nBuilding failed! Exiting...\n")
            Exit()
        logger.write("\nBuild complete!\n")

        #Run tests
        logger.write("\nRunning tests...\n")
        TestsPath = os.path.abspath("../output/Tests/" + platform + "/" + configuration)
        TestsEXEPath = os.path.abspath(TestsPath + "/Tests.exe")
        TestsOutputFilePath = os.path.abspath(UnitTestResultsDir + "/unit-test-results-" + platform + "/" + configuration + "/" + ".txt")
        args = [TestsEXEPath, "-out", TestsOutputFilePath]
        if not Execute(args, TestsPath):
            logger.write("\nOne or more tests failed! See test results in build/unit-test-results.txt. Exiting...\n")
            Exit()
        logger.write("All tests passed!\n")

        #Combine libraries and output to deploy dir
        logger.write("\nCombining libraries...\n\n")
        OutLibPath = LibPlatformDir + "/" + configuration
        os.makedirs(OutLibPath)
        OutLibFilePath = os.path.abspath(OutLibPath + "/" + FinalLibName + ".lib")
        LibPaths = [];
        for name in Libs:
            LibPaths.append(os.path.abspath(OutputPath + "/" + name + "/" + platform + "/" + configuration + "/" + name + ".lib"))
        args = [LibEXEPaths[i], "/OUT:" + OutLibFilePath]
        args.extend(LibPaths)
        if not Execute(args):
            logger.write("\nFailed to create lib file. Exiting...\n")
            Exit()
        logger.write("Finished combining libs!\n")

        #Make sure samples build
        logger.write("\nBuilding samples. Platform: " + platform + "\n\n")
        args = [MSBuildPath, SamplesFilePath, "/property:Configuration=" + configuration, "/property:Platform=" + platform, "/t:Rebuild"]
        if not Execute(args):
            logger.write("\nBuild failed! Exiting...\n")
            Exit()
        logger.write("\nBuild complete!\n")

        #Package the App library
        AppLibConfigurationDir = AppLibPlatformDir + "/" + configuration
        os.makedirs(AppLibConfigurationDir)
        AppLibFile = os.path.abspath(OutputPath + "/" + AppLibName + "/" + platform + "/" + configuration + "/" + AppLibName + ".lib")
        shutil.copy(AppLibFile, AppLibConfigurationDir)

        #Copy IPC_TCP plugin and headers
        IPCLibFile = OutputPath + "/IPC_TCP_Plugin/"+ platform + "/" + configuration + "/" + "IPC_TCP_Plugin.dll"
        IPCLibOutDir = DeployDir + "/" + FinalLibName + "/IPC/lib/" + platform + "/" + configuration
        os.makedirs(IPCLibOutDir)
        shutil.copy(IPCLibFile, IPCLibOutDir)
        old_file = IPCLibOutDir + "/IPC_TCP_Plugin.dll"
        new_file = IPCLibOutDir + "/ipc.dll"
        os.rename(old_file, new_file)
        
# Copy public headers
logger.write("\nCopying header files...\n")
shutil.copytree(SrcPath, DeployDir + "/DgLib/include/")

# Copy AppLib headers
AppLibIncludeDir = DeployDir + "/" + FinalLibName + "/" + AppLibName + "/include/"
os.makedirs(AppLibIncludeDir)

shutil.copy(os.path.abspath(AppLibSrcPath + "/config.ini"), AppLibIncludeDir)
shutil.copy(os.path.abspath(AppLibSrcPath + "/DgApp.h"), AppLibIncludeDir)
shutil.copy(os.path.abspath(AppLibSrcPath + "/Event.h"), AppLibIncludeDir)
shutil.copy(os.path.abspath(ImGuiPath + "/imconfig.h"), AppLibIncludeDir)
shutil.copy(os.path.abspath(ImGuiPath + "/imgui.h"), AppLibIncludeDir)

shutil.copy(os.path.abspath(AppLibSrcPath + "/Mod_Context.h"), AppLibIncludeDir)
shutil.copy(os.path.abspath(AppLibSrcPath + "/Mod_Context_Line.h"), AppLibIncludeDir)
shutil.copy(os.path.abspath(AppLibSrcPath + "/Mod_Context_Triangle.h"), AppLibIncludeDir)
shutil.copy(os.path.abspath(AppLibSrcPath + "/Mod_Window.h"), AppLibIncludeDir)

IPCLibIncludeDir = DeployDir + "/" + FinalLibName + "/IPC/include"
shutil.copy(os.path.abspath(IPCSrcPath + "/IPC_TCP_PluginInterface.h"), IPCLibIncludeDir)

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

# Done!
logger.write("\nBuild Succeeded!")
